#20201229

std::string ThreadMqttListen::PayloadDataAnalysis
在Read/SetBVal, Read/SetBEn 上加上锁；
调整语法结构，改为switch语句； 


分离出PayloadReadB(); 读取断路器value 和 enable;
para添加Serial_Waite_Time 给modbus判断timeout；
isKeyExist判断添加设备id;


#20201228
发现读取设备一段时间后会出现乱串，如控制设备25，的时候出现读取设备26的warn；
原因：Warn 主题的 串口读没有加锁，现在加上。问题解决

问题：电量一直为0。
原因：进行小数运算时，需要把运算的数强制转为float。修改后，电量正常显示；
所有进行小数运算的uint16_t运算数都强制转换为float;

#20201225
添加方法int gmodbus::ModbusWriteMultiRegister(int devid, int addr, int n, uint16_t* data);

#20201224
添加进程锁。
gsvr_app_shmutex.c/h

网关可以完美运行塑壳和断路器程序；

#20201221
修复了若干bug:
1、para->FrameInfo->Timeout 的值是modbus时间时延，被二次覆盖了；修改；
gsvr_app_config.cpp line307 注释//   para->FrameInfo.OverTime = get_json_val("mqtt_public","timeout").GetUint();
2、控制开关后的UpdateSwitchState函数中，没有对ON和OFF是否执行成功做判断，已修改做判断；
gsvr_thread_listen.cpp line799      if(it->second->currentData->openState == cmdtype)   // ON:0--ok; OFF:1--ok
3、当设备ID不存在时，UpdateSwitchState直接返回，不对，应该是continue；已修改；
gsvr_thread_listen.cpp line789     continue;   //return  if devid does not exists!   nono, change to continue!

目前可以正常使用Info和Cmd！


另外 configure  和  fire_service/fire_daemon 有些问题，修改过来了；


1、split_by_ch() 函数修改，判断最后的字符是否为"|", 若为"|",则需要push_back一个空字符串；否则在payload最后的ERR_INFO为空
的情况下， string.size() 会少一个数，导致格式错误！
2、完成升级功能，ThreadMqttListen::ExecUpFileFunc() 进行若干修正，基本上也是因为以上一点的问题引发的格式错误！
3、发现UPOK 和UPOk都兼容。服务端也许修改了；


#20201218
之前开机自启动后由于执行路径在根目录，无法导入配置文件，也无法打开日志路径。
所以在fire_service脚本中的start()添加 cd ${FIREROOT}, 于是成功！

#20201216
这个版本可以完美运行在网关，添加了json配置，还做了一下优化
1、优化程序路径；
2、改在ar9331用户上编译和运行。不需要使用root;  文件路径在/home/ar9331中。 环境变量也是针对用户的，不在/etc/profile中设置，而是在~/.bash_profile中设置。
3、CMakeList.txt 使用相对路径。这样可以拷贝在任意路径下编译。
4、去掉了uuid，参考srs源码，结合srandom 和random函数，生成了随机数。 发现去掉uuid之后，fire_service 大小从7M降低至只有600K,
使用strip之后只剩下500K；


结构：
进程：
${FIREROOT}/fire_service	// 二进制文件
${FIREROOT}/fire_daemon.sh  // 脚本daemon 文件

${FIREROOT}/etc/init.d/fire_service	// 脚本服务
${FIREROOT}/etc/init.d/fire_daemon  // 脚本服务






#20201215
这个版本可以完美运行在网关，但是还没有添加json配置；



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
