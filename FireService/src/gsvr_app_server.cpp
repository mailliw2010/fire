#include "gsvr_app_server.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "gsvr_app_config.h"
#include "gsvr_app_mqtt.h"
#include "gsvr_app_log.h"
#include "globalFun.h"
#include "modbus/modbus.h"
#include "modbus/modbus-private.h"
#include "gsvr_app_shmutex.h"
#include <signal.h>




MainServer::MainServer()

{
    /****************Initial Config **************************/
    LOG(WARNING) << "Constructing MainServer...";

    quit_flag = 0;

    // get para first to initialize;
    conf = std::make_shared<ConfigWin>("./conf/config.conf");
    para = conf.get()->getpara();
    ResponseTimeOut = para->Serial_Wait_Time;
//    load_config_byhand();



    /**new gmqtt***/

    LOG(INFO) << "set clientid by hand";
    std::string lclientid = setAutoClientID(12);
    LOG(WARNING) << "set auto id...";

 //   std::string rclientid = setAutoClientID();
    std::string l_host_port = para->ClocalInfo.host+":"+std::to_string(para->ClocalInfo.port);
 //   std::string r_host_port = para->CremoteInfo.host+":"+std::to_string(para->CremoteInfo.port);
    local = std::make_shared<gmqtt>("Local Client", lclientid, l_host_port,  maxBuffer, persistDir);

 //   remote = std::make_shared<gmqtt>("Remote Client", rclientid, r_host_port, maxBuffer, persistDir);

    /*new threadpool**/
    pool = new ThreadPool(4);   //for threads!

  //  pQTimerAutoSubscribe = new QTimer(/*this*/);
    /****************Initial Config **************************/


    serial = new gmodbus(para->DeviceName, para->BaurdRate, para->DataBit, para->StopBit);
    devBreak = new std::map<uint8_t, infoModel*>;  //initial a dictionary pointer


    UpFileInfo = new tUpFileInfo; // store in heap
  //  pQTimerUpdateDevStatus = new QTimer(/*this*/);

    LOG(INFO) << "Creating ThreadMqttListen thread...";
    threadMqttListen = new ThreadMqttListen(this, 1);
    LOG(INFO) << "Creating ThreadMqttReport thread...";
    threadMqttReport = new ThreadMqttReport(this, 2);






}


void MainServer::signal_handle_fun(int signum)
{
    printf("exit normally: signal = %d\n",signum);
    close_mutex_map();
    exit(0);

}

void MainServer::signal_init() {

    LOG(INFO) << "init signal";
    signal(SIGTERM, MainServer::signal_handle_fun);  // kill 15
    signal(SIGINT, MainServer::signal_handle_fun);	 // ctrl + c


}


void MainServer::initialize()
{


    if(create_mutex_map() < 0){
        LOG(INFO) << "create_process_mutex_error!";
        return;
    }
    memset(mm, 0x00, sizeof(tmutex));   //only initial pshare in this process;
    mutex_cond_init();  // initial mutex


    printf("the size of mutexattr = %04x, value = %04x\n",sizeof(mm->mutexattr),mm->mutexattr);
    if(mm->mutexattr.__align == 0)
    {
        LOG(INFO) << "You may need Master Mutex to initial mutex!";
        exit(FALSE);

    }

    signal_init();






//    auto result = pool->enqueue([](int answer){return answer;}, 43);



    /**initial async mqtt client**/

    // callback set first! then connect!
    cb =  std::make_shared<callback>(*dynamic_cast<mqtt::async_client*>(local.get()),
                                     local.get()->conn_opt, para->topic, local.get()->id, *this);
    local.get()->set_callback(*cb.get());

    LOG(INFO) << "para->ClocalInfo.user" << para->ClocalInfo.user;
    local->initialize(para->ClocalInfo.user, para->ClocalInfo.pass,
                      para->ClocalInfo.keepalivetime, para->topic);



//    remote->initialize(para->CremoteInfo.user.toStdString(), para->CremoteInfo.pass.toStdString(),
//                       para->CremoteInfo.keepalivetime, para->topic_ext.toStdString());

    if(this->serial->initialize() == -1)   // initial serial
    {

        return;
    }

    /*************Initial member variable*******************/


    /**Initialing Device to contain...we must get device id from database later!**/
    for(uint32_t i =para->DeviceIDStart; i <= para->DeviceIDEnd; i++)   // 21 devid, 0x01-0x0a
    {
        infoModel *devinfo = new infoModel;
        devBreak->insert(std::make_pair(i, devinfo));



    }

    for(auto it = devBreak->begin(); it != devBreak->end(); ++it)  //delete infomodel
        {
           LOG(INFO) << "Register for break:" << std::to_string(it->first);
        }

    this->serial->setDevkeyVal(devBreak);   //serial get dev key-value;


    this->seq_Num = 0;  // reTopic seq_Num;
//    std::vector<uint8_t> ss;



//    *UpFileInfo = {"Fire_20200101.bin","1024","","", nullptr, "", 0, 0, 0, 0,0};



    /**************This is thread for ThreadMqttListen!***********************/




#if 1
    /**************This is thread for ThreadMqttReport!***********************/

//    int i;
//    pool->enqueue([i]()->int {
//        std::cout << "hello " << i << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        std::cout << "world " << i << std::endl;
//        return i*i;
//    });
    //int x, y ;
//    pool->enqueue(hello_world, 3, "this is a ");
//    pool->enqueue([](int x, int y){ std::cout << "nice"  << std::endl;}, 3, 6);

 //ThreadMqttReport::ExecUpDevInfo
    // use bind , no need to declare static function;
    pool->enqueue(std::bind(&ThreadMqttReport::ExecUpDevInfo, threadMqttReport));



    /******Btw threadListen and threadReport*********/
  //  connect(threadMqttListen, &ThreadMqttListen::askGetFileSignal, threadMqttReport, &ThreadMqttReport::ExecGetFileFunc);

#endif


    /****************Initial Mqtt Remote Client**************************/

 //   connect(clientRemote.get(), &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
 //   connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientRemote.get(), &gmqtt::publishMesg, Qt::QueuedConnection);
 //   connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientRemote.get(), &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link

   /****************End of Initial Mqtt Remote Client**************************/



   /****************Initial Mqtt local Client**************************/
//    clientLocal->initialize(para->ClocalInfo.port,para->ClocalInfo.host, para->ClocalInfo.user, para->ClocalInfo.pass);

//           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!

//    // user function of get() to get original pointor
//    connect(clientLocal.get(), &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
//    connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientLocal.get(), &gmqtt::publishMesg, Qt::QueuedConnection);
//    connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientLocal.get(), &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link
  /****************End of Initial Mqtt local Client**************************/


    /****start to connect and subscribe to m_client to remote server********/

//    Delay_MSec(1000);    // wait for 1s to subscribe
//    this->clientLocal->subscribmqtt(_mqtt_topic::Topic_Head + "#");
//    this->clientRemote->subscribmqtt(_mqtt_topic::Topic_Head + "#");



}


void MainServer::load_config_byhand()
{
    para = new ParaItems;
    para->BaurdRate = 9600;
    para->DataBit = 8;
    para->StopBit = 1;
    para->FrameInfo.OverTime = 800;




    para->ClocalInfo.host = "183.66.138.122";
    para->ClocalInfo.keepalivetime = 5;
    para->ClocalInfo.port = 1883;
    para->ClocalInfo.user = "admin";
    para->ClocalInfo.pass = "public";

  //  para->CremoteInfo.host = "183.66.138.1222";
    para->CremoteInfo.keepalivetime = 5;
    para->CremoteInfo.pass = "public";
    para->CremoteInfo.port = 1883;
    para->CremoteInfo.user = "admin";
    para->DeviceIDStart = 1;
    para->DeviceIDEnd = 21;
    para->DeviceName = "/dev/ttyUSB0";
    para->LOCAL_CLIENT_ID = "123456";
    para->ProductAREA = "CQ-BS";
    para->ProductID = "FIRE";
    para->ProductNUM = 1;
    para->topic = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/#";
    _mqtt_topic::Topic_Head = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/";

    ResponseTimeOut = para->FrameInfo.OverTime;
}


std::string MainServer::setAutoClientID(int len)
{


    static std::string random_table = "01234567890123456789012345678901234567890123456789abcdefghijklmnopqrstuvwxyz";

    timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t now_us = ((int64_t)tv.tv_sec) * 1000 * 1000 + (int64_t)tv.tv_usec;

    // set the seed of random
    srandom((unsigned long)(now_us | (::getpid()<<13)));


    std::string ret;
    ret.reserve(len);
    for (int i = 0; i < len; ++i) {

        ret.append(1, random_table[random() % random_table.size()]);
    }

    return ret;



}


ParaItems* MainServer::getpara()
{



}



void MainServer::AddSeqNum()
{
    seq_Num++;
}

uint32_t MainServer::getSeqNum()
{
    return seq_Num;
}


const std::shared_ptr<gmqtt>& MainServer::getMqttClientLocal() const
{
    return this->local;


}

std::shared_ptr<gmqtt> MainServer::getMqttClientRemote()
{
    return this->remote;
}





 //when close MainServer, delete all new object!!
 //if member has father, dont have to delete.
 //but threadMqttReport and threadMqttListen will be delete by signal. so dont have to delete
MainServer::~MainServer()
{

    printf("~MainServer is delete!\n");

    LOG(INFO) << ">>Deleting UpFileInfo...";
    delete UpFileInfo;
    UpFileInfo = nullptr;

    LOG(INFO) << ">>Deleting UI...";


    /**Must free m_client before ConfigWin class!!
    m_client is set to null, then delete m_client_local will crash, not know yet
    ***/
    LOG(INFO) << "shared_ptr, no need to detete m_client";

    LOG(INFO) << "shared_ptr, no need to detete m_client_local";




    std::map<uint8_t, infoModel*>::iterator it;  // iterator
    for(it = devBreak->begin(); it != devBreak->end();)  //delete infomodel
    {
        auto second = it->second;
        delete second;   // delete value;
        second = nullptr;
        devBreak->erase(it++);    // it++   remove member from map
    }

    delete devBreak;
    devBreak = nullptr;
    LOG(INFO) << "deleting devBreak";



    LOG(INFO) << "finish ~MainServer";

}




 int MainServer::IsKeyValExist(std::map<uint8_t, infoModel*>::iterator it, uint8_t key)
 {

     if(it == KeyValEnd())   // if no keys in contain, return;
     {
         LOG(INFO) << "Not found key in Map:" << std::to_string(key);
         return -1;   //return -1 if devid does not exists!
     }
     return 0;

 }

 std::map<uint8_t, infoModel*>::iterator MainServer::KeyValFind(uint8_t devid)
 {
     return devBreak->find(devid);

 }

 std::map<uint8_t, infoModel*>::iterator MainServer::KeyValBegin()
 {
     return devBreak->begin();
 }

 std::map<uint8_t, infoModel*>::iterator MainServer::KeyValEnd()
 {
     return devBreak->end();
 }










