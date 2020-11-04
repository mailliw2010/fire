#ifndef THREADMQTTLISTEN_H
#define THREADMQTTLISTEN_H
#include <QThread>
#include <QDebug>
#include <QtMqtt/QMqttClient>
#include <QPushButton>
#include "gsvr_app_public.h"
#include "gsvr_info_break.h"
#include "gsvr_app_config.h"
#include "gsvr_app_modbus.h"

class MainServer;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class ConfigWin;
class ThreadMqttListen: public QObject, public MqttPublic
{
    Q_OBJECT
public:

    ThreadMqttListen(MainServer* mwin, int index);
    ~ThreadMqttListen();
    void RecvHandle(const QByteArray &message, const QMqttTopicName &topic);
    void mesgResponseSlot();
    void ExecCmdFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload);  // topicend and Payload
    void ExecClearFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload);
    void ExecSetBFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd);
    void ExecReadBFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd);
    QString PayloadDataAnalysis(const QStringList& paylist, const ENUM_TopicType& cmd);

    QString DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd);

    void ExecUpFileFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd);
    void ExecHandFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd);

    void ExecParaFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload);
    void GetParaFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload);




signals:
    void mesgResponseSignal(uint32_t seq_Num, const QString &topic, const QString &message);
    void askGetFileSignal();




public slots:
    void Myslot();

private:
    int _index;
    MainServer *m;   //create a pointer to mainwindow.
    ParaItems *m_para;
    tMqttClient *m_Client;
    gmodbus *m_serial;
    tUpFileInfo *m_upfile;
    tTopicHead m_topic;
    ConfigWin* m_conf;






};




#endif // THREADMQTT_H
