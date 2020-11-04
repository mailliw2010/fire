#ifndef THREADMQTTLISTEN_H
#define THREADMQTTLISTEN_H
#include <QThread>
#include <QDebug>
#include <QtMqtt/QMqttClient>
#include <QPushButton>
#include "gsvr_app_public.h"
#include "gsvr_info_mccb.h"
#include "gsvr_app_config.h"
#include "gsvr_app_dlt645.h"

class MainWindow;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class SerialWindow;
class ThreadMqttListen: public QObject, public MqttPublic
{
    Q_OBJECT
public:

    ThreadMqttListen(MainWindow* mwin, int index);
    ~ThreadMqttListen();
    void RecvHandle(const QByteArray &message, const QMqttTopicName &topic);
    void mesgResponseSlot();
    void ExecCmdFunc(QString topicEnd, QStringList MesgHead, QStringList Payload);  // topicend and Payload
    void ExecUpFileFunc(QString topicEnd, QStringList MesgHead, QStringList Payload, ENUM_TopicType cmd);
    void ExecHandFunc(QString topicEnd, QStringList MesgHead, QStringList Payload, ENUM_TopicType cmd);



//    void ExecParaFunc(QString topicEnd, QStringList MesgHead, QStringList Payload);
//    void GetParaFunc(QString topicEnd, QStringList MesgHead, QStringList Payload);


signals:
    void mesgResponseSignal(uint32_t seq_Num, const QString &topic, const QString &message);
    void askGetFileSignal();




public slots:
    void Myslot();

private:
    int _index;
    MainWindow *m;   //create a pointer to mainwindow.
    tMqttClient *m_Client;
    gdlt645 *m_serial;




};




#endif // THREADMQTT_H
