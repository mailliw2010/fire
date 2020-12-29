#include "gsvr_thread_report.h"
#include "gsvr_info_break.h"
#include "gsvr_app_server.h"
#include "gsvr_app_log.h"
#include <chrono>
#include<iomanip>
#if defined(_WIN32)
#else
#include <sys/time.h>
#endif
#include <iostream>
#include "globalFun.h"
#include <sstream>
#include "gsvr_app_shmutex.h"

extern std::mutex cmd_and_info_mutex;


ThreadMqttReport::ThreadMqttReport(MainServer* mwin, int index):m(mwin),_index(index)
{

   //  PicHex = new QByteArray;
     m_serial = m->serial;
     m_upfile = m->UpFileInfo;
     m_para = m->para;
     client = m->local.get();
//    connect(this, &ThreadMqttReport::updateToJsSignals, this, &ThreadMqttReport::updateDataByJS);


}


ThreadMqttReport::~ThreadMqttReport()
{
    LOG(INFO) << "deleting ThreadMqttReport";
//    delete PicHex;


}






void ThreadMqttReport::ExecUpPicFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload)
{

//    /***ReMessage******/
//    tReFrame reFrame;
//    QString isOK = "";

//    QString filePathName = "pic-";
//    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
//    filePathName += ".png";
//    QFile file(filePathName);

//  //  ConfigWin::StringToHex(Payload[2], *PicHex);
//    file.open(QIODevice::ReadWrite);
//  //  file.write(*PicHex, PicHex->size());
//  //  qDebug() << "the size of MessageHex.size() = " << PicHex->size();
//    file.close();


}


std::string ThreadMqttReport::DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd)
{
    uint16_t *regAddr = nullptr;
    float * floataddr = nullptr;
    int regNum = 0;
    int InfoNum = 0;
    int baseAddr = 0;
    std::string rePayload = "";

    auto it = m->KeyValFind(devid);
    if(m->IsKeyValExist(it, devid) < 0)
    {
        return "";   //return "" if devid does not exists!
    }

    /**Need to set regNum/baseAddr**/
   if(cmd == Info)  //default setting
   {
       regNum = 34;   // read 34 total registers
       InfoNum = 38;  // read 38 InfoNum;
       baseAddr = 0x0010;


       /*** Get pointer ***/
       regAddr = &it->second->currentData->openState;

       /**************** Clear Info History Data ******************/
       uint8_t *ptr = reinterpret_cast<uint8_t *>(regAddr);
       memset(ptr, 0, static_cast<uint64_t>(regNum*2));
       *regAddr = 1;   //initial with 1;


//       std::unique_lock<std::mutex> locker(cmd_and_info_mutex);
        /**use process mutex, and pthread is effect!**/
       LOG(INFO) << "try to Lock ...";
       pthread_mutex_lock(&mm->mutex);
       LOG(INFO) << "Lock success! ...";


       if(m_serial->ModbusReadMultiRegister(devid, baseAddr, regNum, regAddr) < 0 )  // but 1 times seems ok;
       {
           it->second->IsOk = 1;
       }
       else {
           it->second->IsOk = 0;
       }
//       locker.unlock();
       pthread_mutex_unlock(&mm->mutex);

       std::this_thread::sleep_for(std::chrono::milliseconds(100));

       it->second->DataInfoHandle();

       /**Put data to repayload**/
       floataddr = &it->second->InfoData.IA;

       char time_buf[32];
       auto t = time(0);
       strftime(time_buf, 32, "%Y-%m-%d %H:%M:%S", localtime(&t)); //format date and time.


       rePayload += time_buf;
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(devid);                      // add id + time
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->InfoData.IsOpenState);   // add Open State
       rePayload += Fr_DDIV;
       rePayload += std::to_string(it->second->InfoData.IsWarState);   // add IsWarState
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->InfoData.IsFaultState);   // add IsFaultState
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->InfoData.isRunState);   // add isRunState


       for(int i = 0; i < InfoNum - 2 - 4; i++)   //updata InfoNum, exclude 2(time/id) + 4 state;
       {
           rePayload += Fr_DDIV;
           rePayload += to_string_n(*(floataddr + i));   // append double data by pointer

       }



       return rePayload;

   }


   if(cmd == War)  //setting War
   {
       regNum = 5;   // read 5 total registers
       InfoNum = 7;  // read 7 InfoNum;
       baseAddr = 0x0100;


       regAddr = &it->second->currentData->ErrNum;


       /**************** Clear War History Data ******************/
       uint8_t *ptr = (uint8_t *)regAddr;
       memset(ptr, 0, (size_t)regNum*2);


     //  std::unique_lock<std::mutex> locker(cmd_and_info_mutex);
       /**use process mutex, and pthread is effect!**/
       LOG(INFO) << "try to Lock ...";
       pthread_mutex_lock(&mm->mutex);
       LOG(INFO) << "Lock success! ...";
       if(m_serial->ModbusReadMultiRegister(devid, baseAddr, regNum, regAddr) < 0 )
       {
           it->second->IsOk = 1;
       }
       else {
           it->second->IsOk = 0;
       }
    //   locker.unlock();
       pthread_mutex_unlock(&mm->mutex);

       std::this_thread::sleep_for(std::chrono::milliseconds(100));



       /**Put data to repayload**/

       char time_buf[32];
       auto t = time(0);
       strftime(time_buf, 32, "%Y-%m-%d %H:%M:%S", localtime(&t)); //format date and time.


       rePayload += time_buf;
       rePayload += Fr_DDIV;
       rePayload += std::to_string(devid);                // add id + time

       rePayload += Fr_DDIV;
       rePayload += std::to_string(it->second->currentData->ErrNum);   // add ErrNum
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->currentData->ErrVal);   // add ErrVal
       rePayload += Fr_DDIV;
       rePayload += std::to_string(it->second->currentData->ErrType);   // add ErrType
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->currentData->ErrPhase);   // add ErrPhase
       rePayload +=  Fr_DDIV;
       rePayload += std::to_string(it->second->currentData->ErrProtect);   // add ErrProtecte

       return rePayload;

   }


}

void ThreadMqttReport::ExecGetFileFunc()
{
    /***ReMessage******/
    tReFrame reFrame;
    std::string topicEnd = "GetFile";

    std::ostringstream of;
     of << m_upfile->FileName << Fr_DDIV \
                // set filesize to 0 to satisity condition
                <<"0" << Fr_DDIV \
                << m_upfile->CreateTime << Fr_DDIV \
                << m_upfile->ModifyTime << Fr_DDIV \
                << std::to_string(m_para->FrameInfo.FrameEachSize);   // add Framesize;
     reFrame.rePayload = of.str();

    /**Public**/
    reFrame.reTopic = topicEnd;
    this->m->AddSeqNum();
    //emit mesgResponseInfoSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote and local



}




// Analysis topic of Info/War/EqState
void ThreadMqttReport::ExecUpDevInfo()
{
    std::map<uint8_t, infoModel*>::iterator it;
    tReFrame reInfo;
    tReFrame reEqState;
    tReFrame reWar;
    std::string isDevOKList;
    uint8_t devid = 0;
    for(;;){
        std::this_thread::sleep_for(std::chrono::seconds(8));
        LOG(INFO) <<  "debuging for thread...";
        /***Get DevInfo from Device****/
        // relcear reEqState;
        reEqState.rePayload.clear();   // must clean this ;
        isDevOKList.clear();


        for(it = this->m->KeyValBegin(); it != m->KeyValEnd(); ++it)
        {

            /*Judge the whether */
//            if(m->get_quit_flag()){
//                LOG(INFO) << "ex 1";
//                break;
//            }
             devid = it->first;
             /************ Analysis Data **************************/




             // for Info
             reInfo.rePayload = DevDataToPayload(devid, Info);//(QString Topic, QString ClientID, QString DataType, QString PayLoad)
             reInfo.reTopic = "Info";
             this->m->AddSeqNum();
            client->publishMesg(m->seq_Num, reInfo.reTopic, reInfo.rePayload);



             // for Warn
             reWar.rePayload = DevDataToPayload(devid, War);
             reWar.reTopic = "War";
             this->m->AddSeqNum();
            client->publishMesg(m->seq_Num, reWar.reTopic, reWar.rePayload);



             // for EqState
             reEqState.rePayload += std::to_string(devid) + "|";
             isDevOKList += std::to_string(it->second->IsOk) + "|";




        }



        reEqState.reTopic = "EqState";
        reEqState.rePayload.pop_back();   // remove the last char of "|"
        isDevOKList.pop_back();           // remove the last char of "|"
        reEqState.rePayload += Fr_DDIV;
        reEqState.rePayload += isDevOKList;

        this->m->AddSeqNum();


        // publish mesag
        client->publishMesg(m->seq_Num, reEqState.reTopic, reEqState.rePayload);


    }




}

