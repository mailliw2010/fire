#ifndef THREADMQTTREPORT_H
#define THREADMQTTREPORT_H
#include "gsvr_app_dlt645.h"

#include <QThread>
#include <QDebug>

#include <QMap>

#include <QtWebEngineWidgets>
#include "gsvr_info_mccb.h"
#include <QtMqtt/QMqttClient>
#include "gsvr_app_public.h"


class SerialWindow;
class MainWindow;
class ThreadMqttReport: public QObject, public MqttPublic
{
    Q_OBJECT
public:


    ThreadMqttReport(MainWindow* mwin, int index);
    ~ThreadMqttReport();
    void ExecMCCBInfo();
    void UpWarnInfo();
    void ExecGetFileFunc();
    QString DevDataToPayload(uint64_t devid, ENUM_TopicType cmd);

    void ExecUpPicFunc(QString topicEnd, QStringList MesgHead, QStringList Payload);



signals:
    void mesgResponseInfoSignal(uint32_t seq_Num, const QString &topic, const QString &message);



private:
    MainWindow *m;   //create a pointer to mainwindow.
    tMqttClient m_Client;
    gdlt645 *m_serial;
    int _index;
    QByteArray *PicHex;





};





#endif // THREADMQTTREPORT_H
