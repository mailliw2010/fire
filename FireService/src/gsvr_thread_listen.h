#ifndef THREADMQTTLISTEN_H
#define THREADMQTTLISTEN_H
#include "gsvr_app_public.h"
#include "gsvr_info_break.h"
#include "gsvr_app_config.h"
#include "gsvr_app_modbus.h"

class MainServer;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class ConfigWin;
class ThreadMqttListen: public MqttPublic
{

public:

    ThreadMqttListen(MainServer* mwin, int index);
    ~ThreadMqttListen();

    void ExecCmdFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead,  std::vector<std::string>& Payload);  // topicend and Payload
    void UpdateSwitchState(const std::string& topicEnd, std::vector<std::string>& Payload, int dev_count);
    void ExecClearFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload);
    void ExecSetBFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload, const ENUM_TopicType& cmd);
    void ExecReadBFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>&Payload, const ENUM_TopicType& cmd);
    std::string PayloadDataAnalysis(const std::vector<std::string>& paylist, const ENUM_TopicType& cmd);
    std::string PayloadReadB(int devid, const ENUM_TopicType& cmd);





    void ExecUpFileFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload, const ENUM_TopicType& cmd);
    void ExecHandFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload, const ENUM_TopicType& cmd);

    void ExecParaFunc(const std::string& topicEnd, std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload);
    void GetParaFunc(const std::string& topicEnd, std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload);






private:
    int _index;
    MainServer *m;   //create a pointer to mainwindow.
    ParaItems *m_para;
    tMqttClient *m_Client;
    gmqtt* client;
    gmodbus *m_serial;
    tUpFileInfo *m_upfile;
    tTopicHead m_topic;
    ConfigWin* m_conf;






};




#endif // THREADMQTT_H
