#ifndef GSVR_APP_SERVER_H
#define GSVR_APP_SERVER_H

#include "gsvr_app_config.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "gsvr_app_mqtt.h"
#include "gsvr_app_public.h"
#include "gsvr_app_modbus.h"

#include "async_client.h"
#include "gsvr_app_threadpool.h"


class ThreadMqttListen;


class MainServer : public MqttPublic
{
//    Q_OBJECT    // signal and slot must have this


public:

    friend class ThreadMqttListen;
    friend class ThreadMqttReport;
    friend class gmqtt;
    friend class callback;
    friend ThreadMqttReport::ThreadMqttReport(MainServer* mwin, int index);
    explicit MainServer();   // construct function, default
    ~MainServer();
    void initialize();
    const std::shared_ptr<gmqtt>& getMqttClientLocal() const;

    std::shared_ptr<gmqtt> getMqttClientRemote();


    void load_config_byhand();

    void AddSeqNum();
    uint32_t getSeqNum();
    std::atomic_int& get_quit_flag(){
        return quit_flag;
    }
    void set_quit_flag(){
        quit_flag = 1;

    }


    std::atomic_int& get_cmd_come_flag(){
        return cmd_come_flag;

    }

    void set_cmd_come_flag(){
        cmd_come_flag = 1;

    }

    std::map<uint8_t, infoModel*>::iterator KeyValFind(uint8_t devid);
    std::map<uint8_t, infoModel*>::iterator KeyValBegin();
    std::map<uint8_t, infoModel*>::iterator KeyValEnd();
    int IsKeyValExist(std::map<uint8_t, infoModel*>::iterator it, uint8_t key);

    ParaItems* getpara();

private:
    MainServer(const MainServer& rhs) = delete;    // copy, only declare
    MainServer& operator=(const MainServer& rhs) = delete;   //assignment, only declare
    std::string setAutoClientID(int len);
    void ShowWindow();
    static void signal_handle_fun(int signum);
    void signal_init();
    ParaItems *para;  //all serial object

    gmodbus *serial;
    std::map<uint8_t, infoModel*> *devBreak;     // for Breaker, uint_8: devid; inifoModel: devInfo

    ThreadMqttListen *threadMqttListen;
    ThreadMqttReport *threadMqttReport;

    std::shared_ptr<gmqtt> local;
    std::shared_ptr<gmqtt> remote;
    std::atomic_int quit_flag;
    std::atomic_int cmd_come_flag;   // 0: cmd ctrl not come; 1:cmd ctrl comes; so multi cmd will recv only one recmd;

    std::shared_ptr<ConfigWin> conf;
    uint32_t seq_Num;   // mqtt serial count number
    tUpFileInfo *UpFileInfo;
    ThreadPool* pool;
    std::queue<tReFrame> frame_queue;
    std::shared_ptr<callback> cb;

};





#endif // MAINWINDOW_H
