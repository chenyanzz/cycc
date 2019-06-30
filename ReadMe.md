#  一个C语言简易解释器cycc


## 简介
又一个准高一小孩子没事闲的挖的新坑：从零开始写个C解释器 emm...
路漫漫其修远兮，吾将上下而求索
好了现在高二了。鸽了一年又是暑假。

## 架构介绍

### 库
主要用的是libc和stl里的标准库函数，效率什么的...

### 特殊类

YCode  基类
YEnv   执行环境控制及变量生命周期管理

### 解析类

YType
    YNumType
    YPtrType
    YStructType
    YEnumType
YVar
YExpression
YStatement
YSCope
YFunction

### 开发思路

#### 解析

将代码逐层分解为不同YCode子类，以

```
void test(int i) {
    func(i+1);
}
```

为例

首先将整段代码解析为一个YFunction对象。将i解析为YVar：int的变量并存入YEnv

然后把{}中内容解析为一个YScope


YScope再把每一条语句识别为一个YStatement对象

YStatement解析func()，将func识别为函数调用，在YEnv函数表中寻找func函数并获取其YFunction对象

func对象的参数i+1解析为YExpression

若要执行test()，则递归调用各对象exec方法执行相应操作

YExpression首先执行，解析运算法则并读取YEnv进行相应计算，返回YVal：int

YFunction得到返回值，将其放入Yenv参数表中，之后调用执行func

func返回后test也紧接返回 完成此次test()调用。