#ifndef THREADMQTTREPORT_H
#define THREADMQTTREPORT_H
#include "gsvr_app_modbus.h"
#include "gsvr_app_config.h"

//#include <QtWebEngineWidgets>
#include "gsvr_info_break.h"
#include "gsvr_app_public.h"
#include<vector>
#include<string>

class ConfigWin;
class MainServer;
class ThreadMqttReport:public MqttPublic
{

public:


    ThreadMqttReport(MainServer* mwin, int index);
    ~ThreadMqttReport();
    void ExecUpDevInfo();

    void UpWarnInfo();
    void ExecGetFileFunc();
    std::string DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd);

    void ExecUpPicFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload);



//signals:
  //  void mesgResponseInfoSignal(uint32_t seq_Num, const std::string &topic, const std::string &message);



private:
    MainServer *m;   //create a pointer to mainwindow.
    ParaItems *m_para;
    gmodbus *m_serial;
    int _index;

    tUpFileInfo *m_upfile;
    tTopicHead m_topic;
    gmqtt* client;





};





#endif // THREADMQTTREPORT_H
