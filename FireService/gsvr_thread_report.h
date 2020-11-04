#ifndef THREADMQTTREPORT_H
#define THREADMQTTREPORT_H
#include "gsvr_app_modbus.h"
#include "gsvr_app_config.h"
#include <QThread>
#include <QDebug>

#include <QMap>

#include <QtWebEngineWidgets>
#include "gsvr_info_break.h"
#include <QtMqtt/QMqttClient>
#include "gsvr_app_public.h"


class ConfigWin;
class MainServer;
class ThreadMqttReport: public QObject, public MqttPublic
{
    Q_OBJECT
public:


    ThreadMqttReport(MainServer* mwin, int index);
    ~ThreadMqttReport();
    void ExecUpDevInfo();
    void ExecUpMCCBInfo();
    void UpWarnInfo();
    void ExecGetFileFunc();
    QString DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd);

    void ExecUpPicFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload);



signals:
    void mesgResponseInfoSignal(uint32_t seq_Num, const QString &topic, const QString &message);



private:
    MainServer *m;   //create a pointer to mainwindow.
    ParaItems *m_para;
    tMqttClient m_Client;
    gmodbus *m_serial;
    int _index;
    QByteArray *PicHex;
    tUpFileInfo *m_upfile;
    tTopicHead m_topic;





};





#endif // THREADMQTTREPORT_H
