// vnctp.h是vnctp中的通用组件，负责定义任务结构体，线程安全队列，字典取值，字符编码转换四个功能


// include<string.h>表示引入的是旧版的string头文件，新版c++不需要加入.h
#include <string> //引入string标准库头文件
#include <queue> // 引入queue标准库头文件
#include <thread> // 引入线程thread标准库头文件
#include <mutex>  //引入互斥锁标准库头文件和thread配合
#include <iostream> //引入输入输出流标准库头文件
#include <codecvt> //引入字符编码转换头文件
#include <condition_variable> //引入处理并发问题的标准库和线程标准库配合
#include <locale> //引入系统区域设置标准库

#include "pybind11/pybind11.h"

//using namespace 一般来说有两个作用：控制重名和少打字符，这里的目的是少打字符
// 等同于python from xx import *
using namespace std;
using namespace pybind11;

// 定义任务结构体；该任务结构体的作用是跨线程传递数据
/*
   任务结构体的字段设置是有相应业务逻辑：
	1. CTP中的回调函数都有整数常量一一对应，因此设置了task_name字段
	2. Task这个结构体设计的最终目的是能支持CTP中的所有回调函数：
		OnRsp类回调函数： 在使用前需要调用过相应的前置函数，因此该函数需要上一个函数的返回值比如：

			CThostFtdcRsqUserLoginField(登录函数成功登录的返回值）
			CThostFtdcRsqUserLoginField(登录函数的报错信息）
	        nRequestID（请求号，因为CTP是异步请求，因此需要缓存一个请求号）
			bIsLast（对于有些函数的返回值并不是一次全部返回的，比如查询合约价格，可能是一个合约一个合约的返回，因此，当返回的合约信息仍不是最后一个时，该函数的返回值的bIsLast是False)
		
		上面的四个参数就和Task结构体的字段呈一一对应
		总的来说，Task结构体的字段是在分析完CTP中所有回调函数，取的最小公倍数来实现对所有回调函数的统一支持



*/
struct Task
{   int task_name;                                 // 回调函数名；跨线程传递数据在某种意义上可以理解为在队列中不断的调用回调函数，因此需要回调函数的函数名
    void *task_data;                               // 主数据指;跨线程传递数需要拿到要传递数据的指针
    void *task_error;                              // 错误指针;C++程序需要对异常进行处理，不然程序会崩溃
    int task_id;                                   // 整数请求号
    bool task_last;                                // 布尔值最终值; 用来确定是否是队列中最后一个任务
};

// 错误异常抛出类，继承C++标准异常类的基类
class TerminatedError: std::exception
{};

class TaskQueue
/* 队列class的设置：
   该class是抽象程度比较高的，基本上适用于各类接口。
	 1. 基于线程安全的角度分别设置了private和public成员属性；private属性只有在class内的代码才可以访问使用下划线_来区分
	 2. public下有三个函数分别是push();pop();terminate()
	    push()函数：
			参数： 结构体Task的指针
			函数逻辑：
			        1. 拿锁，如果没有拿到锁会一直等待
					2. 拿到锁之后将Task推入队列
					3. 解锁
					4. 通知其它线程（读取线程）
	    pop()函数：
			参数： 无参数
			函数逻辑：
					1. 拿锁，如果没有拿到锁会一直等待
					2. 拿到锁之后开始等待		    
					3. 如果队列不为空，则获取队列中最后一个任务（按照先进先出的原则）
					4. 在队列中删除对应任务
					5. 返回该任务
	
	    terminate()函数：
			参数：  无参数
			函数逻辑：
				    1.使得线程能够安全退出


*/

{
 private:
	queue<Task> queue_;                              //标准库队列；该队列中放置的是前面定义的任务结构体
    mutex mutex_;                                    //互斥锁，目的是保证线程安全
    condition_variable cond_;                        //condition_variable条件变量可以实现生产者-消费者模型
	bool _terminate = false;                         //缓存队列的状态，是否取出队列中的所有任务
	
public:
    // 存入新的任务 push()
    void push(const Task &task)                      //c++定义函数需要声明函数的返回类型；当函数不返回数据的时候，使用void
    {
        unique_lock<mutex> mlock(mutex_);           //上锁 mlock应该是标准库mutex中的函数
        queue_.push(task);                          //把任务存入队列,push应该是class queue的方法
        mlock.unlock();                             //解锁
        cond_.notify_one();                         //通知正在阻塞等待的线程，通过condition_variable实现

    }

    // 取出老的任务 pop()
    Task pop()                                      //定义取出任务的函数，该函数返回的是队列中的task
    {
        unique_lock<mutex> mlock(mutex_);           //加锁
        cond_.wait(mlock,[&](){                     //该函数的命令是等待条件变量通知
            return !queue_.empty() || _terminate;

        });
        if (_terminate)
            throw TerminatedError();           
        Task task = queue_.front();                //如果队列不为空，取出队列中的最后一个任务
        queue_.pop();                              //将任务从队列中取出
        return task;
    }
    void terminate()
    {
        _terminate = true;
        cond_.notify_all();
    }
};

//从字典中获取某个键值对应的浮点数，并赋值到请求结构对象的值上
// 从字典获取Pyobject；解包Pyobject拿到底层对象；通过指针进行赋值



/*
 get类函数的设置思路：
	1. get类函数的目的是将python中的dict数据类型传递到C++中 (从python中传入到C++中使用的是字典数据类型）
	2. get类函数的目的决定了get函数需要和C++API中的主动类函数所需要的参数一一对应（本质是python调用C++)
	3. get类函数是在vnctpmd.cpp和vnctptd.cpp中使用主要使用的方法：
		1. 在vnctpmd.cpp或vnctpmd.cpp中调用C++API的主动函数需要传入的参数，通过对python字典数据解包获取
		   具体来说：
		   CThostFtdcReqUserLoginField 结构体是主动登录函数需要的参数之一，而该结构体包含的字段是在python中的字典数据中储存的
		   因此，我们需要编写C++函数来实现对python字典数据进行解析
		   解包逻辑：
		   1. 查看字典中是否包括对应Key
		   2. 如果对应key，则取出键值，用Pyobject缓存
		   3. 从PyObject取出对应键值，进行强制转化成结构体对应参数的类型
		   4. 最后将对应键值赋值到结构体中
		   5. 将结构体作为参数，调用对应主动函数




*/
void getInt(const dict &d, const char *key, double *value) //该函数是空函数no return
{
    if (d.conatins(key))                                   // 判断字典是否存在特定的键值
    {
        object o = d[key];                                // 对PyObject进行解包
        *value = o.cast<double>();                        // 指针赋值
    }
};

void getDouble(const dict &d, const char *key, double *value)
{
    if (d.contains(key))
    {
        object o = d[key];
        *value = o.cast<double>();
    }
};

void getChar(const dict &d, const char *key, char *value){
    if (d.contains(key))
    {
      object o = d[key]
      *value = o.cast<char>();   
    }
};

template <size_t size>
using string_literal = char[size]

template <size_t size>
void getString(const pybind11::dict &d, const char *key, string_literals<size> &value)
{
    if (d.contains(key))
    {

       object o = d[key]
       string s = o.cast<string>();
       const char *buf = s.c_str();
       strcpy(value, buf);


    }
};

