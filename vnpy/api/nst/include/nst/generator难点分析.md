## Generator中的难点

1. 注释间的分隔符有时候不是均等的，因此需要生成出的代码要自己修改
2. 常量定义和typedef中使用了很多新的数据类型，建立字典映射的时候需要自己给字典加入新的键值
3. struct定义中出现了嵌套的数据类型，目前的想法是在typedef中添加嵌套的struct映射为字典
4. 别名定义,针对出现别名的情况，生成一个只包含别名的文件合在一起，大部分的别名是有规律的tag可以用C替换，使用vscode的replace功能就可以。
