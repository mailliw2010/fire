#include "gsvr_app_server.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "ui_mainwindow.h"
#include "gsvr_app_config.h"
#include "gsvr_app_mqtt.h"
#include "ui_serialwindow.h"
#include "globalFun.h"
#include "dlt645/dlt645.h"
#include "dlt645/dlt645-private.h"
#include <QtCore/QDateTime>
#include <QtMqtt/QMqttClient>
#include <QtWidgets/QMessageBox>
#include <QDebug>

#include<QDateTimeEdit>


#include <QtNetwork/QAbstractSocket>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), // base class constructor, use it to initial base member.
    ui(new Ui::MainWindow)
{
    /****************Initial Config **************************/

    clientLocal = new gmqtt("Local Client");
    clientRemote = new gmqtt("Remote Client");
    setting = new SerialWindow(this, this);  // new serialwindow after gmqtt! config use mqtt to config mqtt
    pQTimerAutoSubscribe = new QTimer(this);
    /****************Initial Config **************************/


    serial = new gdlt645(DeviceName, BaurdRate, DataBit, StopBit);
    devMccb = new QMap<uint64_t, infoMCCB*>;  //initial a dictionary pointer

    UpFileInfo = new tUpFileInfo; // store in heap
    pQTimerUpdateDevStatus = new QTimer(this);

    qDebug() << "Creating ThreadMqttListen thread...";
    threadMqttListen = new ThreadMqttListen(this, 1);
    qDebug() << "Creating ThreadMqttListen thread...";
    threadMqttReport = new ThreadMqttReport(this, 2);


    initialize();

}


void MainWindow::initialize()
{

    ShowWindow();    // initial window first!


    if(this->serial->initialize() == -1)   // initial serial
    {

      //  QTimer::singleShot(0, qApp, SLOT(quit()));
        return;
    }
    setting->getserialObj(serial->ctx);    //


    /*************Initial member variable*******************/

    _pubvar::Topic_Head = ProductID+"/" + ProductAREA+"/" + ProductNUM + "/";

    /**Initialing Device to contain...we must get device id from database later!**/
    for(uint64_t i =DeviceIDStart; i <= DeviceIDEnd; i++)   // 21 devid, 0x01-0x0a
    {
        infoMCCB *devinfo = new infoMCCB;
        devMccb->insert(i, devinfo);

    }

    this->serial->setDevkeyVal(devMccb);   //serial get dev key-value;


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

    connect(pQTimerUpdateDevStatus, &QTimer::timeout, threadMqttReport, &ThreadMqttReport::ExecMCCBInfo, Qt::QueuedConnection);
    pQTimerUpdateDevStatus->start(FrameInfo.AutoUploadTime*1000);   // [ms]update every 8s
    QThreadMqttReport.start(QThread::LowPriority);


    /******Btw threadListen and threadReport*********/
    connect(threadMqttListen, &ThreadMqttListen::askGetFileSignal, threadMqttReport, &ThreadMqttReport::ExecGetFileFunc);

#endif



    /****************Initial Mqtt Remote Client**************************/

    clientRemote->initialize(CremoteInfo.port,CremoteInfo.host, CremoteInfo.user, CremoteInfo.pass);
           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!
    connect(clientRemote, &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
    connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientRemote, &gmqtt::publishMesg, Qt::QueuedConnection);
    connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientRemote, &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link

   /****************End of Initial Mqtt Remote Client**************************/



   /****************Initial Mqtt local Client**************************/
    clientLocal->initialize(ClocalInfo.port,ClocalInfo.host, ClocalInfo.user, ClocalInfo.pass);

           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!
    connect(clientLocal, &gmqtt::messageReceived, threadMqttListen, &ThreadMqttListen::RecvHandle,Qt::QueuedConnection);
    connect(threadMqttListen, &ThreadMqttListen::mesgResponseSignal, clientLocal, &gmqtt::publishMesg, Qt::QueuedConnection);
    connect(threadMqttReport, &ThreadMqttReport::mesgResponseInfoSignal, clientLocal, &gmqtt::publishMesg, Qt::QueuedConnection);  // noblocking link
  /****************End of Initial Mqtt local Client**************************/


    /****start to connect and subscribe to m_client to remote server********/

    Delay_MSec(1000);    // wait for 1s to subscribe
    this->clientLocal->subscribmqtt(_pubvar::Topic_Head + "#");
    this->clientRemote->subscribmqtt(_pubvar::Topic_Head + "#");



}

void MainWindow::ShowWindow()
{
    ui->setupUi(this);  //Initial, ui
    /****Initial Window Profile******/
    qDebug() << "Initial MainWindow Profile...";
    this->grabKeyboard();     // make key always focus in mainwindow! this will diable setting.
    this->setContentsMargins(0,0,0,0);
    this->setWindowTitle("电气火灾监控系统");




if(debugmode)
{

}
else
{
    this->setWindowFlags(Qt::FramelessWindowHint);    //hide window, max/min button  seems usless
}



    this->showMaximized();    // control this to show full screen!
    /********webengine***************/
    qDebug() << "Initial Web Engine...";
    QWidget *widget = new QWidget(this);
    ui->m_webView->setParent(widget);
    setCentralWidget(widget);
    QHBoxLayout *windowLayout = new QHBoxLayout(this);
    windowLayout->setContentsMargins(0,0,0,0);
    windowLayout->addWidget(ui->m_webView);
    widget->setLayout(windowLayout);
if(debugmode)
    this->menuBar()->show();
else
    this->menuBar()->hide();


    QUrl url = QUrl::fromLocalFile(UrlName);
    ui->m_webView->load(QUrl(url));

    /**Initial event**/
    connect(ui->actionSettings,&QAction::triggered, this, &MainWindow::actionsettings_clicked);     //settings assigned parameters.
    connect(ui->actionCapture, &QAction::triggered, this, &MainWindow::actioncapture_clicked);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::actionQuit_clicked);
    connect(ui->actionTestButton, &QAction::triggered, this, &MainWindow::actionTestButton_clicked);



}


void MainWindow::Delay_MSec(unsigned int msec)    //delay but not block current thread;
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWindow::AddSeqNum()
{
    seq_Num++;
}

uint32_t MainWindow::getSeqNum()
{
    return seq_Num;
}


gmqtt* MainWindow::getMqttClientLocal()
{
    return this->clientLocal;


}

gmqtt* MainWindow::getMqttClientRemote()
{
    return this->clientRemote;
}

void MainWindow::actionTestButton_clicked()
{


}

/**useless without grabKeyboard*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // 捕捉 Ctrl + Alt + A 组合键;
       if (event->key() == Qt::Key_C  &&  event->modifiers() == Qt::ControlModifier)    // quit Application
       {

           qInfo() << "Quitting FireService Gracefully!\n";
           QApplication::quit();
       }




}


void MainWindow::actioncapture_clicked()
{

//    QScreen *screen = QGuiApplication::primaryScreen();

////    QString filePathName = "capture-";
////    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
////    filePathName += ".png";
//    *capturePixMap = screen->grabWindow(QApplication::desktop()->winId());

//    qDebug() << "QApplication::desktop()->winId()" << QApplication::desktop()->winId();

//    capturePixMap->save("file:///mnt/abc123", "jpg");
//    if(!capturePixMap->save("abc123.jpg", "jpg"))
//    {
//        qDebug() << "capture png failed!!";


//    }


    /*****Transfer Picture********/

    QFile file("FireMonitor.jpg");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    QByteArray dataArray;
    QString reMessage;
    dataArray = file.readAll().toHex();   // read pic to ascii hex
    QDateTime currTime = QDateTime::currentDateTime();
    QString strDate = currTime.toString("yyyy-MM-dd hh:mm:ss");
    QString SseqNum = QString::number(++seq_Num, 10);
    reMessage = SseqNum + "|" + clientLocal->getclientID() + "|TXT|" + 5 + "|@";
    reMessage += dataArray;

    qDebug() << "the dataArray size is " << dataArray.size();


  //  qDebug() << "the QDataStream is " << dataArray;
  //  QString Topic = ProductID+"/"+AreaID+"/"+AreaID_Num + "/UpLoad";
//    ExecPublish(m_client_local, Topic, reMessage.toUtf8());




}


 //when close MainWindow, delete all new object!!
 //if member has father, dont have to delete.
 //but threadMqttReport and threadMqttListen will be delete by signal. so dont have to delete
MainWindow::~MainWindow()
{

    printf("~MainWindow is delete!\n");
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
    delete ui;


    /**Must free m_client before SerialWindow class!!
    m_client is set to null, then delete m_client_local will crash, not know yet
    ***/
    qDebug() << "detete m_client";
    delete clientRemote;
 //   m_client = nullptr;
    qDebug() << "detete m_client_local";
    delete clientLocal;
 //   m_client_local = nullptr;

    QMap<uint64_t, infoMCCB*>::iterator it;  // iterator
    for(it = devMccb->begin(); it != devMccb->end();)  //delete infomodel
    {
        auto second = it.value();
        delete second;   // delete value;
        second = nullptr;
        devMccb->erase(it++);    // it++   remove member from map
    }

    delete devMccb;
    devMccb = nullptr;
    qDebug() << "deleting devMccb";



    qDebug() << "finish ~MainWindow";

}


 void MainWindow::actionsettings_clicked()
 {

     qDebug() << "Opening Setting Window";
     this->releaseKeyboard();
     setting->setModal(true);
     setting->showNormal();


 }

 int MainWindow::IsKeyValExist(QMap<uint64_t, infoMCCB*>::iterator it)
 {

     if(it == KeyValEnd())   // if no keys in contain, return;
     {
         qWarning() << "Not found key in Map:" << it.key();
         return -1;   //return -1 if devid does not exists!
     }
     return 0;

 }

 QMap<uint64_t, infoMCCB*>::iterator MainWindow::KeyValFind(uint64_t devid)
 {
     return devMccb->find(devid);

 }

 QMap<uint64_t, infoMCCB*>::iterator MainWindow::KeyValBegin()
 {
     return devMccb->begin();
 }

 QMap<uint64_t, infoMCCB*>::iterator MainWindow::KeyValEnd()
 {
     return devMccb->end();
 }






void MainWindow::SetSysTime(QString SdateTime)
{

    QDateTimeEdit timeEditor;
    SdateTime = SdateTime.trimmed();
    QDateTime dt = QDateTime::fromString(SdateTime, "yyyy:MM:dd:HH:mm:ss");
    time_t tt = (time_t)dt.toTime_t();

  //  stime(&tt); reset system time   be careful to use this in build development!
    qDebug() << "tt = " << tt;
    qDebug() << "time = " << timeEditor.time();




}

int MainWindow::Para_Set_ProductInfo(QString ProductID, QString ProductAREA, QString ProductNUM)
{
    this->ProductID = ProductID;
    this->ProductAREA = ProductAREA;
    this->ProductNUM = ProductNUM;
    return 0;

}

int MainWindow::Para_Set_ExtProductInfo(QString ProductID, QString ProductAREA, QString ProductNUM)
{
    this->ExtProductID = ProductID;
    this->ExtProductAREA = ProductAREA;
    this->ExtProductNUM = ProductNUM;
    return 0;
}


//int MainWindow::Para_Set_DeviceID(uint32_t DevIDStart, uint32_t DevIDEnd)
//{


//}


int MainWindow::Para_Set_FrameAll(uint32_t frameEachSize, uint32_t overtime, uint32_t resendCount, uint32_t autoUploadTime)
{
    int flag  = 0;

    if(frameEachSize >= 1 && frameEachSize <= 204800000)
    {
        FrameInfo.FrameEachSize = frameEachSize;
    }
    else {
        flag |= 1<<0;
    }

    if(overtime >= 1 && overtime <= 7200)
    {
        FrameInfo.OverTime = overtime;
    }
    else {
          flag |= 1<<1;
    }

    if(resendCount >= 1 && resendCount <= 5)
    {
        FrameInfo.ReSendCount = resendCount;

    }
    else {

         flag |= 1<<2;
    }


    if(autoUploadTime >= 1 && autoUploadTime <= 86400)   // 1s - 1 day
    {
        FrameInfo.AutoUploadTime = autoUploadTime;

    }
    else {
          flag |= 1<<3;
    }

    if((flag & 0x01) != 0)
    {
        qDebug() << "Set para out of range:frameEachSize";

    }
    if((flag & 0x02) != 0)
    {
        qDebug() << "Set para out of range:overtime";
    }
    if((flag & 0x04) != 0)
    {
        qDebug() << "Set para out of range:ReSendCount";
    }
    if((flag & 0x08) != 0)
    {
        qDebug() << "Set para out of range:AutoUploadTime";

    }

    return flag;

}


int MainWindow::Para_Set_FrameEachSize(uint32_t frameEachSize)
{
    int flag = 0;
    if(frameEachSize >= 1 && frameEachSize <= 204800000)
    {
        FrameInfo.FrameEachSize = frameEachSize;
    }
    else {
        flag &= 1<<0;
    }
    return flag;
}
int MainWindow::Para_Set_FrameOverTime(uint32_t overtime)
{
    int flag = 0;
    if(overtime >= 1 && overtime <= 7200)
    {
        FrameInfo.OverTime = overtime;
    }
    else {
          flag &= 1<<1;
    }
    return flag;
}
int MainWindow::Para_Set_FrameReSendCount(uint32_t resendCount)
{
    int flag = 0;
    if(resendCount >= 1 && resendCount <= 5)
    {
        FrameInfo.OverTime = resendCount;

    }
    else {
         flag &= 1<<2;
    }
    return flag;
}
int MainWindow::Para_Set_FrameAutoUploadTime(uint32_t autoUploadTime)
{
    int flag = 0;
    if(autoUploadTime >= 10 && autoUploadTime <= 86400)   // 10ms - 1 day
    {
        FrameInfo.AutoUploadTime = autoUploadTime;

    }
    else {
          flag &= 1<<3;
    }
    return flag;
}


int MainWindow::Para_Set_ClientLocalInfo(quint16 port, QString host, QString user, QString pass)
{
    this->ClocalInfo.port = port;
    this->ClocalInfo.host = host;
    this->ClocalInfo.user = user;
    this->ClocalInfo.pass = pass;

}
int MainWindow::Para_Set_ClientRemoteInfo(quint16 port, QString host, QString user, QString pass)
{
    this->CremoteInfo.port = port;
    this->CremoteInfo.host = host;
    this->CremoteInfo.user = user;
    this->CremoteInfo.pass = pass;
}



MainWindow::tTopicHead MainWindow::Para_Get_ProductInfo()
{
    tTopicHead topic;
    topic.ID = ProductID;
    topic.AREA = ProductAREA;
    topic.NUM = ProductNUM;
    return topic;

}

MainWindow::tTopicHead MainWindow::Para_Get_ExtProductInfo()
{
    tTopicHead topic;
    topic.ID = ExtProductID;
    topic.AREA = ExtProductAREA;
    topic.NUM = ExtProductNUM;
    return topic;



}


MainWindow::tDevSerialNum MainWindow::Para_Get_DeviceIDSerial()
{
    tDevSerialNum dev;
    dev.devStart = DeviceIDStart;
    dev.devEnd = DeviceIDEnd;
    return dev;

}

int MainWindow::Para_Set_DeviceIDSerial(uint32_t start, uint32_t end)
{
    DeviceIDStart = start;
    DeviceIDEnd = end;
    return 0;


}

uint32_t MainWindow::Para_Get_FrameEachSize()
{
    return FrameInfo.FrameEachSize;

}
uint32_t MainWindow::Para_Get_FrameOverTime()
{
    return FrameInfo.OverTime;
}
uint32_t MainWindow::Para_Get_FrameReSendCount()
{
    return FrameInfo.ReSendCount;
}
uint32_t MainWindow::Para_Get_FrameAutoUploadTime()
{
    return FrameInfo.AutoUploadTime;
}



MainWindow::tclientInfo MainWindow::Para_Get_ClientRemoteInfo()
{
    return CremoteInfo;

}
MainWindow::tclientInfo MainWindow::Para_Get_ClientLocalInfo()
{
    return ClocalInfo;

}

int MainWindow::Para_Set_Mode(int mode)
{
    debugmode = mode;
    return 0;




}

int MainWindow::Para_Set_SerialP(QString devname, int baurd, int databit, int stopbit)
{
   DeviceName = devname;
   BaurdRate = baurd;
   DataBit = databit;
   StopBit = stopbit;
   return 0;
}

int MainWindow::Para_Get_Mode()
{
    return debugmode;
}

MainWindow::tserialpara MainWindow::Para_Get_SerialP()
{
    tserialpara ctx;
    ctx.name = DeviceName;
    ctx.baurd = BaurdRate;
    ctx.databit = DataBit;
    ctx.stopbit = StopBit;
    return ctx;

}

void MainWindow::actionQuit_clicked()
{
    QApplication::quit();  // use this to replace exit
}

void MainWindow::updateLogStateChange()
{
    // set contant type
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(clientRemote->state())
                    + QLatin1Char('\n');
    setting->ui->editLog->insertPlainText(content);  //show contents
}

void MainWindow::brokerDisconnected()
{
    setting->ui->lineEditHost->setEnabled(true);
    setting->ui->spinBoxPort_Remote->setEnabled(true);
    setting->ui->buttonConnect->setText(tr("Connect"));
}
