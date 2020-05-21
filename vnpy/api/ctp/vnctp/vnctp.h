// vnctp.h��vnctp�е�ͨ�����������������ṹ�壬�̰߳�ȫ���У��ֵ�ȡֵ���ַ�����ת���ĸ�����


// include<string.h>��ʾ������Ǿɰ��stringͷ�ļ����°�c++����Ҫ����.h
#include <string> //����string��׼��ͷ�ļ�
#include <queue> // ����queue��׼��ͷ�ļ�
#include <thread> // �����߳�thread��׼��ͷ�ļ�
#include <mutex>  //���뻥������׼��ͷ�ļ���thread���
#include <iostream> //���������������׼��ͷ�ļ�
#include <codecvt> //�����ַ�����ת��ͷ�ļ�
#include <condition_variable> //���봦��������ı�׼����̱߳�׼�����
#include <locale> //����ϵͳ�������ñ�׼��

#include "pybind11/pybind11.h"

//using namespace һ����˵���������ã������������ٴ��ַ��������Ŀ�����ٴ��ַ�
// ��ͬ��python from xx import *
using namespace std;
using namespace pybind11;

// ��������ṹ�壻������ṹ��������ǿ��̴߳�������
/*
   ����ṹ����ֶ�����������Ӧҵ���߼���
	1. CTP�еĻص�����������������һһ��Ӧ�����������task_name�ֶ�
	2. Task����ṹ����Ƶ�����Ŀ������֧��CTP�е����лص�������
		OnRsp��ص������� ��ʹ��ǰ��Ҫ���ù���Ӧ��ǰ�ú�������˸ú�����Ҫ��һ�������ķ���ֵ���磺

			CThostFtdcRsqUserLoginField(��¼�����ɹ���¼�ķ���ֵ��
			CThostFtdcRsqUserLoginField(��¼�����ı�����Ϣ��
	        nRequestID������ţ���ΪCTP���첽���������Ҫ����һ������ţ�
			bIsLast��������Щ�����ķ���ֵ������һ��ȫ�����صģ������ѯ��Լ�۸񣬿�����һ����Լһ����Լ�ķ��أ���ˣ������صĺ�Լ��Ϣ�Բ������һ��ʱ���ú����ķ���ֵ��bIsLast��False)
		
		������ĸ������ͺ�Task�ṹ����ֶγ�һһ��Ӧ
		�ܵ���˵��Task�ṹ����ֶ����ڷ�����CTP�����лص�������ȡ����С��������ʵ�ֶ����лص�������ͳһ֧��



*/
struct Task
{   int task_name;                                 // �ص������������̴߳���������ĳ�������Ͽ������Ϊ�ڶ����в��ϵĵ��ûص������������Ҫ�ص������ĺ�����
    void *task_data;                               // ������ָ;���̴߳�������Ҫ�õ�Ҫ�������ݵ�ָ��
    void *task_error;                              // ����ָ��;C++������Ҫ���쳣���д�����Ȼ��������
    int task_id;                                   // ���������
    bool task_last;                                // ����ֵ����ֵ; ����ȷ���Ƿ��Ƕ��������һ������
};

// �����쳣�׳��࣬�̳�C++��׼�쳣��Ļ���
class TerminatedError: std::exception
{};

class TaskQueue
/* ����class�����ã�
   ��class�ǳ���̶ȱȽϸߵģ������������ڸ���ӿڡ�
	 1. �����̰߳�ȫ�ĽǶȷֱ�������private��public��Ա���ԣ�private����ֻ����class�ڵĴ���ſ��Է���ʹ���»���_������
	 2. public�������������ֱ���push();pop();terminate()
	    push()������
			������ �ṹ��Task��ָ��
			�����߼���
			        1. ���������û���õ�����һֱ�ȴ�
					2. �õ���֮��Task�������
					3. ����
					4. ֪ͨ�����̣߳���ȡ�̣߳�
	    pop()������
			������ �޲���
			�����߼���
					1. ���������û���õ�����һֱ�ȴ�
					2. �õ���֮��ʼ�ȴ�		    
					3. ������в�Ϊ�գ����ȡ���������һ�����񣨰����Ƚ��ȳ���ԭ��
					4. �ڶ�����ɾ����Ӧ����
					5. ���ظ�����
	
	    terminate()������
			������  �޲���
			�����߼���
				    1.ʹ���߳��ܹ���ȫ�˳�


*/

{
 private:
	queue<Task> queue_;                              //��׼����У��ö����з��õ���ǰ�涨�������ṹ��
    mutex mutex_;                                    //��������Ŀ���Ǳ�֤�̰߳�ȫ
    condition_variable cond_;                        //condition_variable������������ʵ��������-������ģ��
	bool _terminate = false;                         //������е�״̬���Ƿ�ȡ�������е���������
	
public:
    // �����µ����� push()
    void push(const Task &task)                      //c++���庯����Ҫ���������ķ������ͣ����������������ݵ�ʱ��ʹ��void
    {
        unique_lock<mutex> mlock(mutex_);           //���� mlockӦ���Ǳ�׼��mutex�еĺ���
        queue_.push(task);                          //������������,pushӦ����class queue�ķ���
        mlock.unlock();                             //����
        cond_.notify_one();                         //֪ͨ���������ȴ����̣߳�ͨ��condition_variableʵ��

    }

    // ȡ���ϵ����� pop()
    Task pop()                                      //����ȡ������ĺ������ú������ص��Ƕ����е�task
    {
        unique_lock<mutex> mlock(mutex_);           //����
        cond_.wait(mlock,[&](){                     //�ú����������ǵȴ���������֪ͨ
            return !queue_.empty() || _terminate;

        });
        if (_terminate)
            throw TerminatedError();           
        Task task = queue_.front();                //������в�Ϊ�գ�ȡ�������е����һ������
        queue_.pop();                              //������Ӷ�����ȡ��
        return task;
    }
    void terminate()
    {
        _terminate = true;
        cond_.notify_all();
    }
};

//���ֵ��л�ȡĳ����ֵ��Ӧ�ĸ�����������ֵ������ṹ�����ֵ��
// ���ֵ��ȡPyobject�����Pyobject�õ��ײ����ͨ��ָ����и�ֵ



/*
 get�ຯ��������˼·��
	1. get�ຯ����Ŀ���ǽ�python�е�dict�������ʹ��ݵ�C++�� (��python�д��뵽C++��ʹ�õ����ֵ��������ͣ�
	2. get�ຯ����Ŀ�ľ�����get������Ҫ��C++API�е������ຯ������Ҫ�Ĳ���һһ��Ӧ��������python����C++)
	3. get�ຯ������vnctpmd.cpp��vnctptd.cpp��ʹ����Ҫʹ�õķ�����
		1. ��vnctpmd.cpp��vnctpmd.cpp�е���C++API������������Ҫ����Ĳ�����ͨ����python�ֵ����ݽ����ȡ
		   ������˵��
		   CThostFtdcReqUserLoginField �ṹ����������¼������Ҫ�Ĳ���֮һ�����ýṹ��������ֶ�����python�е��ֵ������д����
		   ��ˣ�������Ҫ��дC++������ʵ�ֶ�python�ֵ����ݽ��н���
		   ����߼���
		   1. �鿴�ֵ����Ƿ������ӦKey
		   2. �����Ӧkey����ȡ����ֵ����Pyobject����
		   3. ��PyObjectȡ����Ӧ��ֵ������ǿ��ת���ɽṹ���Ӧ����������
		   4. ��󽫶�Ӧ��ֵ��ֵ���ṹ����
		   5. ���ṹ����Ϊ���������ö�Ӧ��������




*/
void getInt(const dict &d, const char *key, double *value) //�ú����ǿպ���no return
{
    if (d.conatins(key))                                   // �ж��ֵ��Ƿ�����ض��ļ�ֵ
    {
        object o = d[key];                                // ��PyObject���н��
        *value = o.cast<double>();                        // ָ�븳ֵ
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

