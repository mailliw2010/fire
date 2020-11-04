#include "gsvr_app_mqtt.h"
#include "gsvr_app_public.h"
#include<QUuid>
#include<QDebug>
//#include<QTimer>


gmqtt::gmqtt(QString _name):name(_name)
{



}

gmqtt::gmqtt(const gmqtt& rhs)
{

}


gmqtt::~gmqtt()
{

}

// after new mqtt
void gmqtt::initialize(quint16 port, const QString& host, const QString& _user, const QString& _pass)
{

    setAutoClientID();          // set client ID;
    this->setHostname(host);   //set client host name; QString
    this->setPort(port);       //set port; int
    connectmqtt(_user, _pass);




}






void gmqtt::setAutoClientID()
{
    QString tmpid = QUuid::createUuid().toString();
    tmpid.chop(1);
    id = tmpid.right(12);
    this->setClientId(id);  // 12 Number

}



gmqtt* gmqtt::getclientObj()
{
    return this;

}

QString gmqtt::getclientID()
{
    return id;
}
void gmqtt::connectmqtt(const QString& user, const QString& pass)
{
    /********Set mqtt***************/

    if(this->state() == QMqttClient::disconnect())
    {
        qDebug() << "Connecting to client:" << this->name;
        this->setUsername(user);
        this->setPassword(pass);

        this->connectToHost();  //connect to host
        qDebug() << "Connect to client:" << this->name << " successful!" << "client ID:[" << this->clientId() << "]";


    }




}
int gmqtt::subscribmqtt(const QString& _topic)
{
    qDebug() << this->name << ":Subscribing for topic:" << _topic;
    auto subscription = this->subscribe(_topic);    //subscribe one topic.
    if (!subscription) {   // if subscription is nullptr, means subscribe error.
        qWarning() << "Could not subscribe " << this->name << ". Is there a valid connection?";
        return -1;

    }
    qDebug() << this->name << ":Subscribe for topic:" << _topic << "successful!";
    return 0;

}





void gmqtt::publishMesg(uint32_t seq_Num, const QString &topic, const QString &message)
{
    QString FrameMesg;
    QString DataType("TXT");

    /**Add Topic here***/
    QString TotalTopic = _mqtt_topic::Topic_Head + topic;

    /**Add Header here!**/
    /***RECMD: seq_No[0x1F]TXT[0x1F]@DateTime[0x1F]devid[0x1F]<recmd>[0x1F]ON/OFF[0x1F]OK/ERR*************/
    int DataCount = message.trimmed().split(Fr_DDIV).count();
    QString SseqNum = QString::number(seq_Num, 10);
    FrameMesg += Fr_Head;     //header
    FrameMesg += SseqNum + Fr_DDIV + this->clientId() + Fr_DDIV + DataType + Fr_DDIV + QString::number(DataCount,10);  // header
    FrameMesg += Fr_RDIV;  // Recort Div
    FrameMesg += message;  //payload
    FrameMesg += Fr_End;  // end

    if(this->state() == QMqttClient::Connected)   // publish only connect!
    {

        qInfo() << "Publishing -Topic:" << TotalTopic << "-Mesg:" << FrameMesg;
        if (this->publish(TotalTopic, FrameMesg.toUtf8()) == -1)   //why 1st parameter is ok? different type
        {
            qWarning() << "Coule Not Publish Message!";


        }



    }








}
