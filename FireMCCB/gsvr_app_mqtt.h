#ifndef GSVR_APP_MQTT_H
#define GSVR_APP_MQTT_H
#include<QtMqtt/QMqttClient>
#include<QTimer>


class gmqtt:public QMqttClient   // inherit QMqttClient
{
    Q_OBJECT    // signal and slot must have this

public:

    explicit gmqtt(QString _name);
    virtual ~gmqtt();
    void initialize(quint16 port, QString host, QString _user, QString _pass);
    void setAutoClientID();
    void connectmqtt(QString user, QString pass);
    void publishMesg(uint32_t seq_Num, const QString &topic, const QString &message);
    int subscribmqtt(QString _topic);   // main thread
    gmqtt * getclientObj();
    QString getclientID();


private:


    QString name;
    QString id;
    QString user;
    QString pass;



};


#endif // GSVR_APP_MQTT_H
