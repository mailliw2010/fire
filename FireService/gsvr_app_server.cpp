#include "gsvr_app_server.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "ui_main_win.h"
#include "gsvr_app_config.h"
#include "gsvr_app_mqtt.h"
#include "gsvr_app_view.h"
#include "ui_config_win.h"
#include "globalFun.h"
#include "modbus/modbus.h"
#include "modbus/modbus-private.h"
//#include "modbus/modbus-rtu-private.h"
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>
#include <QDebug>
#include<QSqlDatabase>
#include<QDateTimeEdit>
#include <QtNetwork/QAbstractSocket>



MainServer::MainServer(QWidget *parent) :
    QMainWindow(parent)// base class constructor, use it to initial base member.

{
    /****************Initial Config **************************/
    qDebug() << "Constructing MainServer...";
    clientLocal = std::make_shared<gmqtt>("Local Client");
    clientRemote = std::make_shared<gmqtt>("Remote Client");
    view = new View();

    // get para first to initialize;
    para = getpara();

    pQTimerAutoSubscribe = new QTimer(this);
    /****************Initial Config **************************/

    serial = new gmodbus(para->DeviceName, para->BaurdRate, para->DataBit, para->StopBit);
    devBreak = new QMap<uint8_t, infoModel*>;  //initial a dictionary pointer


    UpFileInfo = new tUpFileInfo; // store in heap
    pQTimerUpdateDevStatus = new QTimer(this);

    qDebug() << "Creating ThreadMqttListen thread...";
    threadMqttListen = new ThreadMqttListen(this, 1);
    qDebug() << "Creating ThreadMqttListen thread...";
    threadMqttReport = new ThreadMqttReport(this, 2);






}


void MainServer::initialize()
{

    view->initialize();


    if(this->serial->initialize() == -1)   // initial serial
    {

      //  QTimer::singleShot(0, qApp, SLOT(quit()));
        return;
    }

    /*************Initial member variable*******************/

    _mqtt_topic::Topic_Head = para->ProductID+"/" + para->ProductAREA+"/" + para->ProductNUM + "/";

    /**Initialing Device to contain...we must get device id from database later!**/
    for(uint8_t i =para->DeviceIDStart; i <= para->DeviceIDEnd; i++)   // 21 devid, 0x01-0x0a
    {
        infoModel *devinfo = new infoModel;
        devBreak->insert(i, devinfo);

    }

    this->serial->setDevkeyVal(devBreak);   //serial get dev key-value;


    this->seq_Num = 0;  // reTopic seq_Num;


    *UpFileInfo = {"Fire_20200101.bin","1024","","", nullptr, "", 0, 0, 0, 0,0};


    /**************This is thread for ThreadMqttListen!***********************/

    threadMqttListen->moveToThread(&QthreadMqttListen);
    connect(&QthreadMqttListen, &QThread::finished, threadMqttListen, &QObject::deleteLater);  // must have this to prevent bug
    QthreadMqttListen.start(QThread::HighestPriority);   // here to set priority

#if 1
    /**************This is thread for ThreadMqttReport!***********************/

    threadMqttReport->moveToThread(&QThreadMqttReport);
    connect(&QThreadMqttReport, &QThread::finished, threadMqttReport, &QObject::deleteLater);

    connect(pQTimerUpdateDevStatus, &QTimer::timeout, threadMqttReport, &ThreadMqttReport::ExecUpDevInfo, Qt::QueuedConnection);
    pQTimerUpdateDevStatus->start(para->FrameInfo.AutoUploadTime*1000);   // [ms]update every 8s
    QThreadMqttReport.start(QThread::LowPriority);


    /******Btw threadListen and threadReport*********/
    connect(threadMqttListen, &ThreadMqttListen::askGetFileSignal, threadMqttReport, &ThreadMqttReport::ExecGetFileFunc);

#endif



    /****************Initial Mqtt Remote Client**************************/

    clientRemote->initialize(para->CremoteInfo.port,para->CremoteInfo.host, para->CremoteInfo.user, para->CremoteInfo.pass);
           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!
    connect(clientRemote.get(), &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
    connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientRemote.get(), &gmqtt::publishMesg, Qt::QueuedConnection);
    connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientRemote.get(), &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link

   /****************End of Initial Mqtt Remote Client**************************/



   /****************Initial Mqtt local Client**************************/
    clientLocal->initialize(para->ClocalInfo.port,para->ClocalInfo.host, para->ClocalInfo.user, para->ClocalInfo.pass);

           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!

    // user function of get() to get original pointor
    connect(clientLocal.get(), &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
    connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientLocal.get(), &gmqtt::publishMesg, Qt::QueuedConnection);
    connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientLocal.get(), &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link
  /****************End of Initial Mqtt local Client**************************/


    /****start to connect and subscribe to m_client to remote server********/

    Delay_MSec(1000);    // wait for 1s to subscribe
    this->clientLocal->subscribmqtt(_mqtt_topic::Topic_Head + "#");
    this->clientRemote->subscribmqtt(_mqtt_topic::Topic_Head + "#");



}


ParaItems* MainServer::getpara()
{

   return view->setting->para;

}



void MainServer::Delay_MSec(unsigned int msec)    //delay but not block current thread;
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
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
    return this->clientLocal;


}

std::shared_ptr<gmqtt> MainServer::getMqttClientRemote()
{
    return this->clientRemote;
}





 //when close MainServer, delete all new object!!
 //if member has father, dont have to delete.
 //but threadMqttReport and threadMqttListen will be delete by signal. so dont have to delete
MainServer::~MainServer()
{

    printf("~MainServer is delete!\n");
    qDebug() << "Deleting Pointer to Release Memory...";
    qDebug() << ">>Deleting QthreadMqttListen...";
    QthreadMqttListen.quit();
    QthreadMqttListen.wait();
    qDebug() << ">>Deleting QThreadMqttReport...";
    QThreadMqttReport.quit();
    QThreadMqttReport.wait();


    qDebug() << ">>Deleting UpFileInfo...";
    delete UpFileInfo;
    UpFileInfo = nullptr;

    qDebug() << ">>Deleting UI...";


    /**Must free m_client before ConfigWin class!!
    m_client is set to null, then delete m_client_local will crash, not know yet
    ***/
    qDebug() << "shared_ptr, no need to detete m_client";
  //  delete clientRemote;
  //  clientRemote = nullptr;
    qDebug() << "shared_ptr, no need to detete m_client_local";
 //   delete clientLocal;
 //   clientLocal = nullptr;

//    try {
//        delete clientLocal;

//    } catch (bool e) {
//        qDebug() << "eeeeeeeeeeeeeeeeeeeee" << e;
//    }



    QMap<uint8_t, infoModel*>::iterator it;  // iterator
    for(it = devBreak->begin(); it != devBreak->end();)  //delete infomodel
    {
        auto second = it.value();
        delete second;   // delete value;
        second = nullptr;
        devBreak->erase(it++);    // it++   remove member from map
    }

    delete devBreak;
    devBreak = nullptr;
    qDebug() << "deleting devBreak";



    qDebug() << "finish ~MainServer";

}




 int MainServer::IsKeyValExist(QMap<uint8_t, infoModel*>::iterator it)
 {

     if(it == KeyValEnd())   // if no keys in contain, return;
     {
         qWarning() << "Not found key in Map:" << it.key();
         return -1;   //return -1 if devid does not exists!
     }
     return 0;

 }

 QMap<uint8_t, infoModel*>::iterator MainServer::KeyValFind(uint8_t devid)
 {
     return devBreak->find(devid);

 }

 QMap<uint8_t, infoModel*>::iterator MainServer::KeyValBegin()
 {
     return devBreak->begin();
 }

 QMap<uint8_t, infoModel*>::iterator MainServer::KeyValEnd()
 {
     return devBreak->end();
 }




void MainServer::SetSysTime(QString SdateTime)
{

    QDateTimeEdit timeEditor;
    SdateTime = SdateTime.trimmed();
    QDateTime dt = QDateTime::fromString(SdateTime, "yyyy:MM:dd:HH:mm:ss");
    time_t tt = (time_t)dt.toTime_t();

  //  stime(&tt); reset system time   be careful to use this in build development!
    qDebug() << "tt = " << tt;
    qDebug() << "time = " << timeEditor.time();




}
