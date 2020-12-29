#ifndef GSVR_APP_MQTT_H
#define GSVR_APP_MQTT_H
#include<async_client.h>
#include<string>
#include<memory>
#include "gsvr_app_threadpool.h"



typedef struct{
    std::string ID;
    std::string AREA;
    std::string NUM;
}tTopicHead;


#define N_RETRY_ATTEMPTS    5

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
    std::string name_;

    void on_failure(const mqtt::token& tok) override;

    void on_success(const mqtt::token& tok) override;
public:
    action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */

class MainServer;
class callback : public virtual mqtt::callback,
                    public virtual mqtt::iaction_listener

{
    // Counter for the number of connection retries
    int nretry_;
    // The MQTT client
    mqtt::async_client& cli_;    //this reference can make callback can not be copy or assign--xcd
    // Options to use if we need to reconnect
    mqtt::connect_options& connOpts_;
    // An action listener to display the result of actions.
    action_listener subListener_;

    // by xcd
    std::string &topic_;
    std::string &cli_id_;
    MainServer& m;

    int QOS = 1;



    // This deomonstrates manually reconnecting to the broker by calling
    // connect() again. This is a possibility for an application that keeps
    // a copy of it's original connect_options, or if the app wants to
    // reconnect with different options.
    // Another way this can be done manually, if using the same options, is
    // to just call the async_client::reconnect() method.
    void reconnect();

    // Re-connection failure
    void on_failure(const mqtt::token& tok) override;

    // (Re)connection success
    // Either this or connected() can be used for callbacks.
    void on_success(const mqtt::token& tok) override {}

    // (Re)connection success
    void connected(const std::string& cause) override ;
    // Callback for when the connection is lost.
    // This will initiate the attempt to manually reconnect.
    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;

        std::cout << "Reconnecting..." << std::endl;
        nretry_ = 0;
        reconnect();
    }

    // Callback for when a message arrives.
    void message_arrived(mqtt::const_message_ptr msg) override;

    void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
    callback(mqtt::async_client& cli, mqtt::connect_options& connOpts,
             std::string &topic, std::string &cli_id, MainServer& _m)
                : nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription"),cli_id_(cli_id),
                  topic_(topic), m(_m){}
};



class MainServer;
class callback;
class gmqtt:public mqtt::async_client   // inherit QMqttClient
{
 //   Q_OBJECT    // signal and slot must have this

public:

    friend MainServer;
    explicit gmqtt(std::string _name, std::string _clientID, const std::string& svrname_port,
                   int maxBuf, const std::string& persistDir);
//    gmqtt(const gmqtt& ths);
    virtual ~gmqtt();
    int initialize(const std::string& _user, const std::string& _pass, int keepalive_time, std::string& _topic);
    // note: the message is payload at all;
    void publishMesg(uint32_t seq_Num, const std::string &topic, const std::string &message);

    gmqtt * getclientObj();
    std::string getclientID();
    void printdebugmsg();




private:

    std::string name;
    std::string id;
    std::string topic;
    mqtt::connect_options conn_opt;    // _pass, _user


};


#endif // GSVR_APP_MQTT_H
