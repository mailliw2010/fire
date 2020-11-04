#ifndef GSVR_APP_MQTT_H
#define GSVR_APP_MQTT_H
#include<QtMqtt/QMqttClient>
#include<QTimer>


class gmqtt:public QMqttClient   // inherit QMqttClient
{
    Q_OBJECT    // signal and slot must have this

public:

    explicit gmqtt(QString _name);
    gmqtt(const gmqtt& ths);
    virtual ~gmqtt();
    void initialize(quint16 port, const QString& host, const QString& _user, const QString& _pass);
    void connectmqtt(const QString& user, const QString& pass);
    void publishMesg(uint32_t seq_Num, const QString &topic, const QString &message);
    int subscribmqtt(const QString& _topic);   // main thread
    gmqtt * getclientObj();
    QString getclientID();


private:
    void setAutoClientID();

    QString name;
    QString id;
    QString user;
    QString pass;



};


#endif // GSVR_APP_MQTT_H
