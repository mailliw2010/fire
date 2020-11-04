#20200910

添加完mqtt服务功能

#20200907
1、运行参数
2、状态上报
3、告警记录



#20200904
根据协议修改功能：
1、Topic主题

#Server Listen：
Cmd、Clear、
FrameInfo、GetPara、
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
