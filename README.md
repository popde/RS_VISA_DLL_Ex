# RS_VISA_DLL_Ex
利用vs2019对visa32.dll通信动态库进行二次封装, 使用起来更方便

研究visa发现, 其实很多时候并不需要那么多的函数来使用, 为了对aardio中使用更友好, 我在RS官方示例基础上, 对dll进行了二次封装, 只保留了5个函数.
VisaInit
VisaClose
VisaSetAttr
VisaWrite
VisaRead
增加了aardio中调用示例.
这里我用串口进行了visa的模拟,  

特别注意: 串口初始化完成, 默认波特率是9600 , 8,0,0,1 , 接收中断默认是接收到'\n'即"0x0A"字符,  如果你的设备不是默认配置, 那么就使用 VisaSetAttr() 对设备进行属性设置, 改变波特率和结束符符号.

至于属性是哪个, 可以在文档中找到.
首发地址: [NI-VISA通信的调用](https://htmlayout.cn/t/474)

![image](https://github.com/popde/RS_VISA_DLL_Ex/blob/main/GIF.gif)

