20201102
原则构造函数尽量用于构造对象；其中Config.ini参数例外，放在ConfigWin构造函数中，
因为某些构造对象初始化依赖于此
para存放： conf
完成了调试；

20201030
分离界面view;新建gsvr_app_view文件存放view类


20201028
1、mqttclient/mqttremote 尝试使用std::shared_ptr<gmqtt> 管理资源；
2、注意：QString/QByteArray是指针类型，append的内容在堆区。（实验证明）
3、除了内置类型、STL迭代器、STL函数对象外，建议其他类型使用pass-by-reference-to-const 替换pass-by-value
以避免切割问题！现在就改完！


20201027
1、修改名称：SerialWindow改为 ConfigItem类； MainWindow改为MainServer;
2、构造函数不要泄露this指针原则： 
把MainServer的Initialize从构造函数移动到main()函数。所谓二段式构造。
3、使用构造函数最好使用成员初值列表初始化，而不是用赋值操作。
4、编译器可以暗自为class创建default构造函数/copy构造函数/copy assignment函数/
析构函数。
5、如果类不使用copy构造和copy assignment.那么最好在private中声明两者，
并不要实现！防止被调用拷贝和赋值；

20201022
slaveid还是用uint64_t ，不用数组；
addrid还是用 uint32_t表示；
这样设计方便计算！


keyvalue改为<uint64_t, infoMCCB>

20201021
分离MCCB 程序；
编写dlt645协议代码；
完成： 
写数据；
读数据；

正在编写：
设备控制代码；

20201017
添加breaker上传故障告警信息完成; 
添加breaker上传EqState信息，完成；

修改文件名 infomodel为gsvr_info_break;

新增塑壳断路器MCCB info 文件；
gsvr_info_mccb;



20201016

1、每次UpInfo消息之后，要清空历史数据；避免因为modbus通信异常而循环发送历史数据；
2、添加UseDefaultConfig()方法; 当配置文件不存在时，配置默认参数；
3、把窗口初始化放在所有初始化前；这样可以在参数配置错误时，依然能打开窗口，重新配置参数；
3.2、上传故障告警信息；未完成；只写了接口函数UpWarnInfo();
4、添加塑壳断路器；未完成；



20201015
添加gsvr_app_mqtt;
添加gsvt_app_modbus;

配置文件完成；
中间出现无法subscribe情况，原因是 connect之后需要等待一定时间才能订阅。添加 Delay_MSec后解决

void MainWindow::Delay_MSec(unsigned int msec)    //delay but not block current thread;
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

20201013
1、添加 namespace _pubvar
声明公共变量， Topic_Head, ExtTopic_Head;
2、FireConfig界面修改，添加配置变量；
3、FireConfig添加方法：保存配置变量和加载配置文件；


20201012
配置数据入口:FireConfigure写入配置数据，然后配置数据写入INI配置文件；
重启后 load INI 配置文件给全局变量实现配置

目前状况：
1、配置界面已写；
2、写入配置文件代码已写；
3、根据配置文件初始化未完成；
4、设计思路需要进一步整理；


配置文件格式：

[Mode]
DebugMode=0   

[Uart]
DevName=/dev/tty/USB0
Baurd=9600
DataBit=8
StopBit=1

[Modbus]
ResponseTimeOut_US=500000

[MqttPublic]
ProductID=FIRE
ProductAREA=CQ-BS
ProductNUM=2
DeviceIDStart=1
DeviceIDEnd=21

FrameEach=1024
TimeOut=5
ResendNum=3
UpInfoPeriod_S=10



[Mqtt-Local]
Host=LocalHost
Port=1883
User=
PassWord=
ReConnectTimsp_S=

[Mqtt-Remote]
Host=183.66.138.122
Port=1883
User=admin
PassWord=qvcmjd
ReConnectTimsp_S=


20201004

1.代码修改：成员变量改为private；
2.添加系统时间设置功能(但是注意使用，系统时间错误会导致某些文件找不到，编译和上网问题)
如果出现某些文件错误，只需要remove掉file，重新添加，编译就会通过


20200930
代码修改，
1新建tMqttClient类，装两个client；
2修改MyTopic名字为MqttPublic;
3public成员修改为private


20200929

1、修改代码， tFrame_local 去掉；应答topic和payload统一用 tFrame表示 ;
2、ThreadListen中监听到的某些耗时非重要任务，使用concurrent并发功能新建线程后台执行；保证ThreadListen线程主要处理cmd/clear等重要耗时任务；
3、ThreadReport主要监听耗时非重要任务：UpInfo;定时上报；

添加功能：日志写入和终端打印同时执行。
使用 if(gDefaultHandler)
       {
           gDefaultHandler(type,context,msg);
       }


4、添加EqState, 如果Info没有返回，标识异常；


20200925

1、修改状态显示，1-表示闭合；0-表示断开



20200924

已经完成兼容windows和linux版程序


1、程序退出时，有错误！
deleting the serialwindow ui
原因： m_client和m_client_local需要在serialwindow之前delete!!!!

2、QT的
QApplication::quit()和QApplication::exit()都能结束进程，并且调用析构函数；
问号： exit(0)是什么回事？它不会调用析构函数
构造函数中退出程序。并析构的方法：
QTimer::singleShot(0, qApp, SLOT(quit()));   // use this to exit

3、完成固件升级程序；


20200923

modbus.obj:-1: error: LNK2019: 无法解析的外部符号 tmp，该符号在函数 send_msg 中被引用
这样的问题，是因为 c++定义的变量在c文件中使用不能识别！！！

支持中文。跨平台方法
https://blog.csdn.net/libaineu2004/article/details/19245205

编译器自动识别：
如果是windows visual c++ 编译器： 会自动 #define _WIN32

#if defined(_WIN32)
   xxx
#else
   xxx
#endif


查看编译器宏定义
gcc -E -dM - </dev/null


Qss file在button上没有效果！一次在Qbutton的
autodefult, default, flat选择打钩，再取消，竟然可以了；





20200922

协程相关知识
https://www.cnblogs.com/NerdWill/p/6166220.html
https://segmentfault.com/a/1190000017963725
https://blog.csdn.net/u010981745/article/details/50496963


ST相比于MP, MT的性能优势！！特别是在高并发上！
https://blog.csdn.net/xiaojun111111/article/details/52759428?utm_medium=distribute.wap_relevant.none-task-blog-title-1



#程序移植到windows工作

1、关于winsock.h版本的问题，在工程文件.pro中添加以下代码

DEFINES += WIN32_LEAN_AND_MEAN #文件包含顺序错误，这两个头文件是：#include <winsock2.h>和#include<windows.h>

2、
在“modbus.obj:-1: error: LNK2019: 无法解析的外部符号 UartMesg，该符号在函数 _modbus_receive_msg 中被引用”
对应的文件的头文件中，添加如下代码；
#pragma comment(lib,"ws2_32.lib")

3、modbus.obj:-1: error: LNK2019: 无法解析的外部符号 tmp，该符号在函数 send_msg 中被引用
尚未解决！





20200921

# 添加Git
# 开关统计都是0,功能尚未未完成；
# 现在数据分3次读取；13 =+ 13 + 8 读取；但是发现直接读取34个寄存器结果是一样的。根据参考；电能低寄存器的值是存在的，为30；
# 电能修改：使用uint64_t 类型，防止数据溢出；




#20200918


已经恢复功能；


#20200914

#调试FireService 版，发现控制空开不能用。排查发现 在modbus-rtu.c 的write()函数中，重写了发送数据！修改后恢复；

#发现现在控制空开很快！为什么？
暂不清楚

# 互斥锁修复: 之前有一把锁在主线程中，并不安全; 现改为把两把全局锁放在MqttListen和MqttReport中。
当进入ExecCmdFunc()时，上锁;
当进入DevUpdateFun()时，上锁;

# 不能强制把 long int 型指针转为 double型指针。因为double 存储的格式和int型完全不同!


#20200917

1. 修改FireClient:
	1.界面，增大长度以显示；
	2.增加clear；
	3.颜色lighlight消息；暂未做
2. 修改FireService
	1.日志统一：分为 debug/info/warn/critical/fatal; 添加 > 100M事，日志清空；√
	2.Report的createFrame去掉，统一在Publish方法中组帧；√
	3.mutex锁需要分别放在MqttListen线程和MqttReport线程；其中 MqttListen需要上锁的方法有ExecCmdFun/ExecSetBFunc/ExecReadBFunc;  √
	4.modelInfo DataHandle中去掉long int指针强制转换为double指针, 数据依然分为double和int型；double型由int*0.1得方法转换       √
	5.时间调试宏定义 √
	6.every it = keyValuePairs.find(devid); after that, must judge it == keyValuePairs.end(); √
	  如果没有设备；在cmd控制时，直接返回，不执行；在info主题时；payload 填写空"";  √
	7.关于预命令发送0x0080时返回值的判断：0x0080返回值有误，不确定，但确定的是，没有设备时，返回0；所以判断改为： returnVal == 0 --> return ；
	8.上电初始化时以广播的方式获取设备ID：目前广播功能不能使用，暂且手动添加设备信息到字典中；√
	
	



#20200911
#解析时去掉头和尾， Fr_Head, Fr_END
#控制开关的逻辑是，这个逻辑符合的前提是 状态必须是100%正确；
当开关本身处于合闸的状态，不执行合闸操作；
当开关本身处于开闸的状态，不执行开闸操作；



#20200910

添加完mqtt服务功能

修改代码名字和框架：


-->FireElecMonitor
				-->FireService  //尚未精简
				-->FireClient   //已经精简
				-->FireLog







#20200907
1、运行参数
2、状态上报
3、告警记录



#20200904
根据协议修改功能：
1、Topic主题

#Server Listen：
Cmd、Clear、
SetPara、GetPara、
SetBVal、ReadBVal、
SetBEn、ReadBEn、
SetBCor、ReadBCor、
GwState、定时；


#Server Report:
Info、Warn
EqState




#20200829

485串口等待超时设置为200ms；如果设置为100ms，读取状态会失败(暂不清楚原因，韩工用100ms也可以)；
Report的参数由原来的107个修改为50个，因为不需要；一个断路器的读取时间缩短为4-5s一次轮训；
添加逻辑判断：执行闭闸操作时，如果断路器状态本身为闭闸，则不执行指令；执行开闸操作时，如果断路器本身为开闸，则不执行指令；这样可以提高效率；



#20200827

添加图传测试程序；
保存版本：simpleclient-20200828




#20200824
排除bug: seq_Number 在local server 和 remote server上 重复计算
排除bug: 上传Info上不需要上传串口命令码；取消；
排除bug: 不需要再开发板上做http web服务器，因为远程的183.66.138.122上已经有网站。只需要网关与远程mqtt服务器通信即可！
排除bug: 当控制一个断路器的时候也必须预留足够的时间等待断路器反应，所以需要在一个、或多个断路器执行预命令、动命令之后添加等待, 目前设置为500ms，这个值可以保证读取的状态是正确的。

下一步计划：
1、mqtt图传截屏给服务器；
2、修复bug，打开单一设备无法显示网络的问题；
3、学习rstp和rtmp等视频协议
4、网页界面在板子上无法全屏的问题；



#20200820

更新！
成功连接到websocket!在开发板上工作正常。并添加了m_client_local；
开发板运行local_server, 同时监听mqtt和websocket协议
注：运行在local上的网页，mqtt 的server name 要改为 localhost.否则无法识别（目前代码使用的是主机名）  

运行：
1. 后台运行mqtt local server.  mosquitto -c /etc/mosquitto/mosquitto.conf -d
2. 执行fireserver程序 (./keypad)


#20200812
固化程序，界面基本做出来。下一步把service抽离；与网页挂接。


20200807
1子窗口改为dialog,可以关闭；
2设置主机模式为TRUE；即一直程序一直是client。发送数据超时等待返回。
保存的版本为最新。先打算精简modbus流程。


#20200727

1、移植libmodbus库源码的unit-test-client，使用该源码可使用(libmodbus-3.1.6)，读取空开状态，但目前还不能用0x6命令码控制空开，发送预命令后，读取0x80一直是0(不是库的问题，是空开的问题)，不清楚原因；
2、QT程序 mqtt+485串口收发可以使用，子窗口的settings用于调试，保存在simlpeclient-0727中

3、下一步工作是创建线程：
线程1：ThreadMqttListen 类 轮训监听mqtt client发过来的命令，对固定topic下的命令进行解析，进行应答和转发给485串口；
线程2：主线程，轮训监听界面事件，比如有按键事件发生，执行485相应功能；
线程3：ThreadMqttReport 类，上报状态：mqtt


4、不同线程之间使用槽函数，成功。记录版本：simlpeclient-0727_2
