#include "gsvr_app_mqtt.h"
#include "gsvr_app_public.h"
#include "gsvr_app_log.h"
#include "gsvr_app_threadpool.h"
#include "gsvr_app_public.h"
#include "gsvr_app_server.h"
#include "globalFun.h"




void action_listener::on_failure(const mqtt::token& tok)
{
       std::cout << name_ << " failure";
       if (tok.get_message_id() != 0)
           std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
       std::cout << std::endl;
}


void action_listener::on_success(const mqtt::token &tok)
{
       std::cout << name_ << " success";
       if (tok.get_message_id() != 0)
           std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
       auto top = tok.get_topics();
       if (top && !top->empty())
           std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
       std::cout << std::endl;
 }


void callback::reconnect()
{
       std::this_thread::sleep_for(std::chrono::milliseconds(2500));
       try {
           cli_.connect(connOpts_, nullptr, *this);
       }
       catch (const mqtt::exception& exc) {
           std::cerr << "Error: " << exc.what() << std::endl;
           exit(1);
       }
}

void callback::on_failure(const mqtt::token &tok)
{
       std::cout << "Connection attempt failed" << std::endl;
       if (++nretry_ > N_RETRY_ATTEMPTS)
           exit(1);
       reconnect();
}


void callback::connected(const std::string &cause)
{
        LOG(INFO) << "\nConnection success" ;
        LOG(INFO) << "\nSubscribing to topic '" << this->topic_ << "'\n"
            << "\tfor client " << this->connOpts_.get_user_name()
            << " using QoS" << QOS << "\n"
            << "\nPress Q<Enter> to quit\n";
        cli_.subscribe(topic_, QOS, nullptr, subListener_);
}




void callback::message_arrived(mqtt::const_message_ptr msg)
{
//        std::cout << "Message arrived" << std::endl;
//        std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
//        std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;



        /***Topic******/

        std::string rev_top = msg->get_topic();

        std::vector<std::string> TopicFrame = split_by_char(rev_top, '/');
    //    std::for_each(TopicFrame.begin(), TopicFrame.end(),[=](std::string str_){LOG(INFO) << "__" << str_;});


//        if(_mqtt_topic::Topic_Head != TopicFrame[0] || m_topic.AREA != TopicFrame[1] || m_topic.NUM != TopicFrame[2])
//        {
//            LOG(INFO) << "Get Incorrect Topic:" << rev_top;
//            return;
//        }

        /***Message******/
        std::string msg_tmp = msg->to_string();

        msg_tmp.erase(0, 1);    // remove the first
        msg_tmp.pop_back();  // remove the last ;

        std::vector<std::string> rev_mesg = split_by_char(msg_tmp, Fr_RDIV);
      //  std::for_each(rev_mesg.begin(), rev_mesg.end(),[=](std::string str_){LOG(INFO) << "__" << str_;});


        if(rev_mesg.size() <= 1)
        {
            LOG(INFO) << "MessageData Error:" << msg_tmp[0];
            return;
        }



        std::vector<std::string> MessageHead = split_by_char(rev_mesg[0], Fr_DDIV);

        std::vector<std::string> Payload = split_by_char(rev_mesg[1], Fr_DDIV);


        switch(hash_(TopicFrame[3].data()))
        {
            case "FrameInfo"_hash:
            {
                LOG(INFO) << "Get Topic:" << "FrameInfo";
                // ExecParaFunc(TopicFrame[3], MessageHead, Payload);
                // put the function to exec in one new thread!
            //    QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecParaFunc, TopicFrame[3], MessageHead, Payload);

            }
            break;

            case "GetPara"_hash:
            {
                LOG(INFO) << "Get Topic:" << "GetPara";
                //GetParaFunc(TopicFrame[3], MessageHead, Payload);
                // put the function to exec in one new thread!
           //     QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::GetParaFunc, TopicFrame[3], MessageHead, Payload);


            }

            break;

            case "Info"_hash:  //Info
            break;

            case "Cmd"_hash:   //CMD
                LOG(INFO) << "Get Topic:" << "Cmd";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecCmdFunc, m.threadMqttListen,
                                          TopicFrame[3], MessageHead, Payload),
                                TopicFrame[3], MessageHead, Payload);

            break;

            case "ReCmd"_hash:    // RECMD
            break;

            case "Clear"_hash:   //"CLEAR"      "ENE" << "FAULT" << "WAR" << "COUNT"
                LOG(INFO) << "Get Topic:" << "Clear";
           //     ExecClearFunc(TopicFrame[3], MessageHead, Payload);
            break;

            case "ReClear"_hash:  //RECLEAR
            break;

            case "Fire_TimeSyn"_hash:  //Fire_TimeSyn
//                qDebug() << "Get Topic:" << "TimeSyn";
//                if(Payload.size() == 1)
//                {
//                    m->SetSysTime(Payload[0]);

//                }
            break;

            case "SetBVal"_hash:  //RECLEAR
            {
                LOG(INFO) << "Get Topic:" << "SetBVal";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecSetBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::SetBVal),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::SetBVal);
               // ExecSetBFunc(TopicFrame[3], MessageHead, Payload, SetBVal);
                // put the function to exec in one new thread!
           //     QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecSetBFunc, TopicFrame[3], MessageHead, Payload, SetBVal);



            }

            break;

            case "ReadBVal"_hash:
            {
                LOG(INFO) << "Get Topic:" << "ReadBVal";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecReadBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBVal),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBVal);
               // ExecReadBFunc(TopicFrame[3], MessageHead, Payload, ReadBVal);
                // put the function to exec in one new thread!
            //    QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecReadBFunc, TopicFrame[3], MessageHead, Payload, ReadBVal);
            }

            break;

            case "SetBEn"_hash:
            {
                LOG(INFO) << "Get Topic:" << "SetBEn";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecSetBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::SetBEn),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::SetBEn);
               // ExecSetBFunc(TopicFrame[3], MessageHead, Payload, SetBEn);
                // put the function to exec in one new thread!
             //   QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecSetBFunc, TopicFrame[3], MessageHead, Payload, SetBEn);
            }

            break;

            case "ReadBEn"_hash:
            {
                LOG(INFO) << "Get Topic:" << "ReadBEn";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecReadBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBEn),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBEn);

            }

            break;

            case "SetBCor"_hash:
            {
                LOG(INFO) << "Get Topic:" << "SetBCor";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecSetBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::SetBCor),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::SetBCor);

            }

            break;

            case "ReadBCor"_hash:
            {
               LOG(INFO) << "Get Topic:" << "ReadBCor";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecReadBFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBCor),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::ReadBCor);
            }


            break;

            case "GwState"_hash:

            break;

            case "UpInfo"_hash:
                LOG(INFO) << "Get Topic:" << "UpInfo";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecUpFileFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::UpInfo),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::UpInfo);
            //    ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, UpInfo);

            break;

            case "ReGetFile"_hash:
                LOG(INFO) << "Get Topic:" << "ReGetFile";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecUpFileFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::ReGetFile),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::ReGetFile);
            //    ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, ReGetFile);

            break;

            case "FileData"_hash:
            {
                LOG(INFO) << "Get Topic:" << "FileData";
                m.pool->enqueue(std::bind(&ThreadMqttListen::ExecUpFileFunc, m.threadMqttListen, TopicFrame[3], MessageHead, Payload, MqttPublic::FileData),
                                TopicFrame[3], MessageHead, Payload, MqttPublic::FileData);
               // ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, FileData);
               // put the expensive event to one new thread!
             //   QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecUpFileFunc, TopicFrame[3], MessageHead, Payload, FileData);
            }


        break;

            case "Hand"_hash:
            {
                LOG(INFO) << "Get Topic:" << "Hand";
                LOG(INFO) << "Topic:" << TopicFrame[3];


            //    QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecHandFunc, TopicFrame[3], MessageHead, Payload, Hand);
            }


            break;




    /**transfer picture by mqtt protocol-- just for test
     * StringToHex() is a static function, which has no object.
     * PicHex has better to be storaged to heap(new). cause it may be big. the size can be less than memory in theory
     *

    ***/
        case "UpPic"_hash:  //UpLoad
            LOG(INFO) << "Get DataType of" << "UpPic";


            break;

        default:

            break;


        }














}



//async_client(const string& serverURI, const string& clientId,
//int maxBufferedMessages, const string& persistDir);

//explicit gmqtt(QString _name, const std::string& svrname_port, QString clientid,
//int maxBuf, const std::string& persistDir);
gmqtt::gmqtt(std::string _name, std::string client_ID, const std::string& svrname_port,
             int maxBuf, const std::string& persistDir):
    mqtt::async_client(svrname_port, client_ID, maxBuf, persistDir),    // father construct
    name(_name)

{
    this->id = client_ID;


}

//gmqtt::gmqtt(const gmqtt& rhs)
//{

//}


gmqtt::~gmqtt()
{

}

// after new mqtt
int gmqtt::initialize(const std::string& _user, const std::string& _pass, int keepalive_time, std::string& _topic)
{

    conn_opt =  mqtt::connect_options(_user, _pass);
    conn_opt.set_keep_alive_interval(keepalive_time);


    //use dynamic_cast, cast from son to father, it is save

//    cb =  std::make_shared<callback>(*dynamic_cast<mqtt::async_client*>(this), conn_opt, _topic, id/*, m*/);

//    this->set_callback(*cb.get());

    // Start the connection.
    // When connect completed, the callback will subscribe to topic.
    try {
            LOG(INFO) << this->name << " Connecting to the MQTT server...";
            connect(conn_opt);
        }
        catch (const mqtt::exception&) {
            LOG(ERROR) << "\nERROR: Unable to connect to MQTT server";
            return -1;
        }
    LOG(INFO) << "successful!" << "client ID:[" << this->id << "]" ;
  //  printdebugmsg();
    return 0;

}



void gmqtt::printdebugmsg()
{
    LOG(INFO) << "name:" << name ;
    LOG(INFO) << "the id:" << id ;
    LOG(INFO) << "the user:" << this->conn_opt.get_user_name() ;
    LOG(INFO) << "the server:" << this->conn_opt.get_servers() ;


}





gmqtt* gmqtt::getclientObj()
{
    return this;

}

std::string gmqtt::getclientID()
{
    return id;
}





void gmqtt::publishMesg(uint32_t seq_Num, const std::string &topic, const std::string &message)
{
    std::string FrameMesg;
    std::string DataType("TXT");

    /**Add Topic here***/
    std::string TotalTopic = _mqtt_topic::Topic_Head + topic;

    /**Add Header here!**/
    /***RECMD: seq_No[0x1F]TXT[0x1F]@DateTime[0x1F]devid[0x1F]<recmd>[0x1F]ON/OFF[0x1F]OK/ERR*************/
    std::string mesg(message);   // from const to unconst
    std::vector<std::string> tmps = split_by_char(mesg, Fr_DDIV);
    int DataCount = tmps.size();


    FrameMesg += Fr_Head;
    FrameMesg += std::to_string(seq_Num);
    FrameMesg += Fr_DDIV;
    FrameMesg += this->id;
    FrameMesg += Fr_DDIV;
    FrameMesg += DataType;
    FrameMesg += Fr_DDIV ;
    FrameMesg += std::to_string(DataCount);  // header
    FrameMesg += Fr_RDIV;  // Recort Div
    FrameMesg += message;  //payload
    FrameMesg += Fr_End;  // end

    mqtt::message_ptr pubmsg = mqtt::make_message(TotalTopic, FrameMesg);
    pubmsg->set_qos(1);

    const auto TIMEOUT = std::chrono::seconds(10);
    this->publish(pubmsg)->wait_for(TIMEOUT);
   // LOG(INFO) << "Publishing -Topic:" << TotalTopic << "-Mesg:" << FrameMesg;







}
