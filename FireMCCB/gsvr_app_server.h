#ifndef GSVR_APP_SERVER_H
#define GSVR_APP_SERVER_H

#include <QMainWindow>
#include <QtMqtt/QMqttClient>
#include <QMap>

#include "gsvr_app_config.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "gsvr_app_mqtt.h"
#include <QHBoxLayout>
#include "gsvr_app_public.h"
#include "gsvr_app_dlt645.h"
#include "gsvr_info_mccb.h"


class ThreadMqttListen;



namespace Ui {    //先声明一个在Ui命名空间的名为MainWindow的未定义的的类，现在只是一个声明而已 Define is in ui_mainwindow.h
class MainWindow;
}

class MainWindow : public QMainWindow, public MqttPublic
{
    Q_OBJECT    // signal and slot must have this


public:

    typedef struct{
        QString ID;
        QString AREA;
        QString NUM;
    }tTopicHead;

    typedef struct{
      uint32_t devStart;
      uint32_t devEnd;
    }tDevSerialNum;


    typedef struct{
        quint16 port;
        QString host;
        QString user;
        QString pass;

    }tclientInfo;

    typedef struct{
      QString name;
      int baurd;
      int databit;
      int stopbit;
    }tserialpara;



//    friend class ThreadMqttListen;
    friend ThreadMqttListen::ThreadMqttListen(MainWindow* mwin,  int index);
    friend ThreadMqttReport::ThreadMqttReport(MainWindow* mwin, int index);
    friend void ThreadMqttReport::ExecGetFileFunc();
    friend void ThreadMqttListen::ExecUpFileFunc(QString topicEnd, QStringList MesgHead, QStringList Payload, ENUM_TopicType cmd);

    explicit MainWindow(QWidget *parent = nullptr);   // construct function, default
    ~MainWindow();
    gmqtt* getMqttClientLocal();
    gmqtt* getMqttClientRemote();
    void SetSysTime(QString SdateTime);
    void Delay_MSec(unsigned int msec);

    void AddSeqNum();
    uint32_t getSeqNum();

    QMap<uint64_t, infoMCCB*>::iterator KeyValFind(uint64_t devid);
    QMap<uint64_t, infoMCCB*>::iterator KeyValBegin();
    QMap<uint64_t, infoMCCB*>::iterator KeyValEnd();
    int IsKeyValExist(QMap<uint64_t, infoMCCB*>::iterator it);

    int Para_Set_ProductInfo(QString ProductID, QString ProductAREA, QString ProductNUM);
    int Para_Set_ExtProductInfo(QString ProductID, QString ProductAREA, QString ProductNUM);
    int Para_Set_DeviceIDSerial(uint32_t start, uint32_t end);
    int Para_Set_FrameAll(uint32_t frameEachSize, uint32_t overtime, uint32_t resendCount, uint32_t autoUploadTime);
    int Para_Set_FrameEachSize(uint32_t frameEachSize);
    int Para_Set_FrameOverTime(uint32_t overtime);
    int Para_Set_FrameReSendCount(uint32_t resendCount);
    int Para_Set_FrameAutoUploadTime(uint32_t autoUploadTime);
    int Para_Set_ClientLocalInfo(quint16 port, QString host, QString user, QString pass);
    int Para_Set_ClientRemoteInfo(quint16 port, QString host, QString user, QString pass);
    int Para_Set_Mode(int mode);
    int Para_Set_SerialP(QString devname, int baurd, int databit, int stopbit);

    tTopicHead Para_Get_ProductInfo();
    tTopicHead Para_Get_ExtProductInfo();
    tDevSerialNum Para_Get_DeviceIDSerial();
    uint32_t Para_Get_FrameEachSize();
    uint32_t Para_Get_FrameOverTime();
    uint32_t Para_Get_FrameReSendCount();
    uint32_t Para_Get_FrameAutoUploadTime();
    tclientInfo Para_Get_ClientRemoteInfo();
    tclientInfo Para_Get_ClientLocalInfo();
    int Para_Get_Mode();
    tserialpara Para_Get_SerialP();





signals:

    void Ctrl_AllSwitch_Signal(const int cmd);
    void Ctrl_OneSwitch_Signal(const int devid, const int cmd);


private slots:
    void actionQuit_clicked();
    void updateLogStateChange();
    void brokerDisconnected();
    void actionsettings_clicked();

    void actioncapture_clicked();
    void actionTestButton_clicked();



protected:
    void keyPressEvent(QKeyEvent *event);

private:

    void initialize();
    void ShowWindow();
    SerialWindow *setting;  //all serial object
    QString SysDateTime;
    QTimer *pQTimerUpdateDevStatus;
    QTimer *pQTimerAutoSubscribe;

    gdlt645 *serial;
    QMap<uint64_t, infoMCCB*> *devMccb;     // for Breaker, uint_8: devid; inifoModel: devInfo

    QThread QthreadMqttListen;
    ThreadMqttListen *threadMqttListen;

    QThread QThreadMqttReport;
    ThreadMqttReport *threadMqttReport;

    gmqtt *clientLocal;
    gmqtt *clientRemote;
    uint32_t seq_Num;   // mqtt serial count number
    Ui::MainWindow *ui;
    tUpFileInfo *UpFileInfo;




    /************************Config Items*************************/
    /**mqtt***/

    tclientInfo ClocalInfo;
    tclientInfo CremoteInfo;

    QString ProductID;
    QString ProductAREA;
    QString ProductNUM;

    QString ExtProductID;
    QString ExtProductAREA;
    QString ExtProductNUM;

    uint32_t DeviceIDStart;
    uint32_t DeviceIDEnd;

    QString REMOTE_CLIENT_ID;
    QString LOCAL_CLIENT_ID;
    tFrameInfo FrameInfo;
    int debugmode;


    /**Serial Variable**/    // value is set in Config file!!
#if defined (_WIN32)   // windows
    QString DeviceName = "COM6";
#elif defined (__x86_64)  // x86 gcc
    QString DeviceName = "/dev/pts/1";
#else  // _ARM_LINUX_    // arm-linux-gcc
    QString DeviceName = "/dev/ttymxc0";
#endif
    int BaurdRate;
    int DataBit;
    int StopBit;


#if defined (_WIN32)   // windows
    QString UrlName = "C:/inetpub/wwwroot/views/switch.html";
#elif defined (__x86_64)  // x86 gcc
   // /home/forlinx/web/index.html   --old web
   // /home/forlinx/wwwroot/views/index.html  --new web
    QString UrlName = "/home/forlinx/wwwroot/views/switch.html";
#else    // default is  _ARM_LINUX_    // arm-linux-gcc
    QString UrlName = "/home/forlinx/web/index.html";
#endif


};





#endif // MAINWINDOW_H
