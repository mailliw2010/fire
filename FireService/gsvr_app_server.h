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
#include "gsvr_app_modbus.h"
#include "gsvr_info_mccb.h"


class ThreadMqttListen;



namespace Ui {    //先声明一个在Ui命名空间的名为MainServer的未定义的的类，现在只是一个声明而已 Define is in ui_mainwindow.h
class MainServer;
}

class MainServer : public QMainWindow, public MqttPublic
{
    Q_OBJECT    // signal and slot must have this


public:

    friend class ThreadMqttListen;
    friend ThreadMqttReport::ThreadMqttReport(MainServer* mwin, int index);
    explicit MainServer(QWidget *parent = nullptr);   // construct function, default
    ~MainServer();
    void initialize();
    const std::shared_ptr<gmqtt>& getMqttClientLocal() const;

    std::shared_ptr<gmqtt> getMqttClientRemote();
    void SetSysTime(QString SdateTime);
    void Delay_MSec(unsigned int msec);

    void AddSeqNum();
    uint32_t getSeqNum();

    QMap<uint8_t, infoModel*>::iterator KeyValFind(uint8_t devid);
    QMap<uint8_t, infoModel*>::iterator KeyValBegin();
    QMap<uint8_t, infoModel*>::iterator KeyValEnd();
    int IsKeyValExist(QMap<uint8_t, infoModel*>::iterator it);

    ParaItems* getpara();

private:
    MainServer(const MainServer& rhs);    // copy, only declare
    MainServer& operator=(const MainServer& rhs);   //assignment, only declare
    void ShowWindow();
    ParaItems *para;  //all serial object
    View* view;
    QString SysDateTime;
    QTimer *pQTimerUpdateDevStatus;
    QTimer *pQTimerAutoSubscribe;

    gmodbus *serial;
    QMap<uint8_t, infoModel*> *devBreak;     // for Breaker, uint_8: devid; inifoModel: devInfo
    QMap<uint8_t, infomccb*> *devMccb;       // for MCCB, MCCB may not add in this app, need to fork one new app;

    QThread QthreadMqttListen;
    ThreadMqttListen *threadMqttListen;

    QThread QThreadMqttReport;
    ThreadMqttReport *threadMqttReport;


    std::shared_ptr<gmqtt> clientLocal;
    std::shared_ptr<gmqtt> clientRemote;
    uint32_t seq_Num;   // mqtt serial count number
    tUpFileInfo *UpFileInfo;


};





#endif // MAINWINDOW_H
