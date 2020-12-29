#include "gsvr_thread_listen.h"
#include "gsvr_app_server.h"
#include "gsvr_app_config.h"
#include "gsvr_app_log.h"
#include "gsvr_app_public.h"
#include "gsvr_app_shmutex.h"
#include "globalFun.h"
#include<fstream>
#include<sstream>

using namespace _mqtt_topic;
extern std::mutex cmd_and_info_mutex;
extern std::mutex info_and_stat_mutex;
ThreadMqttListen::ThreadMqttListen(MainServer* mwin,  int index):_index(index),m(mwin)
{
    m_serial = m->serial;
    m_para = m->para;
    m_upfile = m->UpFileInfo;
    client = m->local.get();


    LOG(INFO) << "init thread_listen";
//    m_topic = m_conf->Para_Get_ProductInfo();


}


ThreadMqttListen::~ThreadMqttListen()
{
    LOG(INFO) << "deleting ThreadMqttListen";



}






// ClientID + DataType + DataCount + PayLoad








void ThreadMqttListen::GetParaFunc(const std::string& topicEnd, std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload)
{
    /***ReMessage******/
    tReFrame reFrame;
    std::string isOK = "";


    if(Payload.size() == 1)
    {
        if(Payload[0] == "ASK")
        {
            std::ostringstream of;
            of << m_para->FrameInfo.FrameEachSize << Fr_DDIV << m_para->FrameInfo.OverTime << Fr_DDIV << \
                  m_para->FrameInfo.ReSendCount << Fr_DDIV << m_para->FrameInfo.AutoUploadTime;

        }


        reFrame.reTopic = _mqtt_topic::Topic_Head + "Re" + topicEnd;
        this->m->seq_Num++;
     //   emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote/local server


    }




}

std::string ThreadMqttListen::PayloadReadB(int devid, const ENUM_TopicType& cmd)
{

    std::string rePayload = "";
    int baseAddr = 0;
    auto it = m->KeyValFind(devid);
    if (m->IsKeyValExist(it, devid) < 0) {
        return "";   //return  if devid does not exists!
    }
    switch (cmd) {
        case ReadBVal:
        {
            LOG(INFO) <<"ReadBVal:";
            int ReadNum = 12;
            baseAddr = 0X0040;
            /**************** Clear SetEn History Data ******************/
            uint8_t *ptr = reinterpret_cast<uint8_t *>(&it->second->currentData->OverProtectI);
            memset(ptr, 0, static_cast<uint64_t>(ReadNum*2));  // clear
            m_serial->ModbusReadMultiRegister(devid, baseAddr, ReadNum, &it->second->currentData->OverProtectI);

            it->second->DataSetEnHandle();
            /*** Make Payload ***/
            rePayload = std::to_string(devid);
            float* floataddr = &it->second->SetBData.OverProtectI;
            for(int i = 0; i < ReadNum; i++)
            {
                rePayload += Fr_DDIV;
                rePayload += to_string_n(*(floataddr + i));   // append double data by pointer
            }

        }
            break;
        case ReadBEn:
        {
            LOG(INFO) << "ReadBEn";
            int ReadNum = 9;
            baseAddr = 0X0050;
            /**************** Clear SetEn History Data ******************/
            uint8_t *ptr = reinterpret_cast<uint8_t *>(&it->second->currentData->OverLongDelayProEn);
            memset(ptr, 0, static_cast<uint64_t>(ReadNum*2));  // clear
            m_serial->ModbusReadMultiRegister(devid, baseAddr, ReadNum, &it->second->currentData->OverLongDelayProEn);

            it->second->DataSetEnHandle();
            /*** Make Payload ***/
            rePayload = std::to_string(devid);
            uint32_t * intaddr = &it->second->SetBData.OverLongDelayProEn;
            for(int i = 0; i < ReadNum; i++) {
                rePayload += Fr_DDIV;
                rePayload += std::to_string(*(intaddr + i));   // append double data by pointer
            }

        }
            break;
        case ReadBCor:
        {
            LOG(INFO) << "ReadBCor";
            int ReadNum = 11;
            baseAddr = 0X006A;
            /**************** Clear SetEn History Data ******************/
            uint8_t *ptr = reinterpret_cast<uint8_t *>(&it->second->currentData->correctI_A);
            memset(ptr, 0, static_cast<uint64_t>(ReadNum*2));  // clear
            m_serial->ModbusReadMultiRegister(devid, baseAddr, ReadNum, &it->second->currentData->correctI_A);

            /*** Make Payload ***/
            rePayload = std::to_string(devid);
            float * floataddr = &it->second->SetBData.correctI_A;
            for(int i = 0; i < ReadNum; i++)
            {
                rePayload += Fr_DDIV;
                rePayload += to_string_n(*(floataddr + i));   // append double data by pointer
            }
        }
            break;

    }
    return rePayload;

}

std::string ThreadMqttListen::PayloadDataAnalysis(const std::vector<std::string>& paylist, const ENUM_TopicType& cmd)
{


    int baseAddr = 0;
    std::string rePayload = "";
    std::vector <uint16_t> datalist;
    uint8_t devid = std::stoi(paylist.at(0));
    const int RegCount = paylist.size() - 1;    // exclude devid
    int flag = 0;
    std::string isOK = "";
    auto it = m->KeyValFind(devid);
    if (m->IsKeyValExist(it, devid) < 0) {
        return "";   //return  if devid does not exists!
    }

    switch (cmd) {
        case SetBVal:
            if (RegCount == 12) {
                baseAddr = 0X0040;
                datalist.push_back(std::stof(paylist.at(1)) * 10);
                datalist.push_back(std::stof(paylist.at(2)));
                datalist.push_back(std::stof(paylist.at(3)) * 10);
                datalist.push_back(std::stof(paylist.at(4)) * 1);
                datalist.push_back(std::stof(paylist.at(5)) * 10);
                datalist.push_back(std::stof(paylist.at(6)) * 1);
                datalist.push_back(std::stof(paylist.at(7)) * 10);
                datalist.push_back(std::stof(paylist.at(8)) * 1);
                datalist.push_back(std::stof(paylist.at(9)) * 1);
                datalist.push_back(std::stof(paylist.at(10)) * 1);
//                datalist.push_back(std::stof(paylist.at(11))*10);
//                datalist.push_back(std::stof(paylist.at(12))*10);
//                datalist.push_back(std::stof(paylist.at(13))*1);
                datalist.push_back(std::stof(paylist.at(11)) * 1);
                datalist.push_back(std::stof(paylist.at(12)) * 1);
                LOG(INFO) << "the array of datalist = :";
                uint16_t *dataarray = datalist.data();
                for (int i = 0; i < datalist.size(); i++) {
                    printf("%d  ", *(dataarray++));
                }
                printf("\n");

                if (m_serial->ModbusWriteMultiRegister(devid, baseAddr, RegCount, datalist.data()) < 0) {
                    LOG(WARNING) << "Err to write!";
                    flag = 1;
                }

                /*** Make Payload ***/
                if (flag == 0) {
                    isOK = "OK";

                } else {
                    isOK = "ERR";
                }
                rePayload = paylist.at(0);
                rePayload += Fr_DDIV;
                rePayload += isOK;
                rePayload += Fr_DDIV;
                rePayload += std::to_string(flag);
            }
            break;
        case SetBEn:
            if (RegCount == 9) {
                baseAddr = 0X0050;
                datalist.push_back(std::stof(paylist.at(1)));
                datalist.push_back(std::stof(paylist.at(2)));
                datalist.push_back(std::stof(paylist.at(3)));
                datalist.push_back(std::stof(paylist.at(4)));
                datalist.push_back(std::stof(paylist.at(5)));
                datalist.push_back(std::stof(paylist.at(6)));
                datalist.push_back(std::stof(paylist.at(7)));
                datalist.push_back(std::stof(paylist.at(8)));
                datalist.push_back(std::stof(paylist.at(9)));
                LOG(INFO) << "the array of datalist = :";
                uint16_t *dataarray = datalist.data();
                for (int i = 0; i < datalist.size(); i++) {
                    printf("%d  ", *(dataarray++));
                }
                printf("\n");
                if (m_serial->ModbusWriteMultiRegister(devid, baseAddr, RegCount, datalist.data()) < 0) {
                    LOG(WARNING) << "Err to write!";
                    flag = 1;
                }
            }
            break;

        case SetBCor:
            if (RegCount == 11) {
                baseAddr = 0X006A;
                datalist.push_back(std::stod(paylist.at(1)) * 10);
                datalist.push_back(std::stod(paylist.at(2)) * 10);
                datalist.push_back(std::stod(paylist.at(3)) * 10);
                datalist.push_back(std::stod(paylist.at(4)) * 10);
                datalist.push_back(std::stod(paylist.at(5)) * 10);
                datalist.push_back(std::stod(paylist.at(6)) * 10);
                datalist.push_back(std::stod(paylist.at(7)) * 10);
                datalist.push_back(std::stod(paylist.at(8)) * 1);
                datalist.push_back(std::stod(paylist.at(9)) * 1);
                datalist.push_back(std::stod(paylist.at(10)) * 1);
                datalist.push_back(std::stod(paylist.at(11)) * 1);

                LOG(INFO) << "the array of datalist = :";
                uint16_t *dataarray = datalist.data();
                for (int i = 0; i < datalist.size(); i++) {
                    printf("%d  ", *(dataarray++));
                }
                printf("\n");
                if (m_serial->ModbusWriteMultiRegister(devid, baseAddr, RegCount, datalist.data()) < 0) {
                    LOG(WARNING) << "Err to write!";
                    flag = 1;
                }


            }
            break;
    }

    return rePayload;

}



/**SetVal, SetEn, SetCorrrect Function**/
void ThreadMqttListen::ExecSetBFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead,
                                    const std::vector<std::string>& Payload, const ENUM_TopicType& cmd)
{
    const int DataCount  = std::stoi(MesgHead.at(3));

    /***ReMessage******/
    tReFrame reFrame;
    reFrame.reTopic = "Re" + topicEnd;
    if(Payload.size() == DataCount)
    {
        /**Analisis Data to datalist, put to rePayload**/
        LOG(INFO) << "setB try to Lock ...";
        pthread_mutex_lock(&mm->mutex);
        LOG(INFO) << "setB Lock success! ...";
        reFrame.rePayload = PayloadDataAnalysis(Payload, cmd);
        pthread_mutex_unlock(&mm->mutex);

        /*** send to mqtt***/
        if(!reFrame.rePayload.empty()){
            this->m->seq_Num++;
            client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);
        }



    }



}


void ThreadMqttListen::ExecUpFileFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
//    local variable will not be shared, because update is step by step;

//
//



    const int DataCount  = std::stoi(MesgHead.at(3));

    LOG(INFO) << "Entering UpInfo handle...";
    LOG(INFO) <<"upfile->FileSize = " << m_upfile->FileSize;
    LOG(INFO) << "Payload[1] = " << Payload[1];
    LOG(INFO) << "m_upfile->Createtime = " << m_upfile->CreateTime;
    LOG(INFO) << "Payload[2] = " << Payload[2];
    LOG(INFO) << "m_upfile->ModifyTime = " << m_upfile->ModifyTime;
    LOG(INFO) << "Payload[3] = " << Payload[3];
    if(cmd == UpInfo)
    {
        tReFrame reFrame;


        if(DataCount == Payload.size() && DataCount == 4)
        {

            LOG(INFO) << "prepare UpInfo";
            // if One of them id unequel, then update file
            if(m_upfile->FileSize != Payload[1] || m_upfile->CreateTime != Payload.at(2) || m_upfile->ModifyTime != Payload.at(3))
            {
                m_upfile->FileName = Payload.at(0);   //file name
                m_upfile->FileSize = Payload.at(1);    //file size
//                m_upfile->CreateTime = Payload.at(2);    //file create time
//                m_upfile->ModifyTime = Payload.at(3);    //file modify time
                m_upfile->CreateTime = "2020-09-20 17:14:39";    //file create time
                m_upfile->ModifyTime = "2020-09-20 17:14:39";    //file modify time


                //emit askGetFileSignal();    // send signal to thread Report
                m->threadMqttReport->ExecGetFileFunc();
            }



        }

    }

    if(cmd == ReGetFile)
    {
        tReFrame reFrame;
        std::string isOK = "";
        if(DataCount == Payload.size() && DataCount == 5)
        {
            LOG(INFO) << "prepare ReGetFile";
            if(Payload.at(1) != "0")
            {
                m_upfile->FileName = Payload.at(0);   //file name
                m_upfile->FrameEachSize = std::stoi(Payload.at(1));
                m_upfile->FrameEndSize = std::stoi(Payload.at(2));
                m_upfile->FrameAllSize = std::stoi(Payload.at(3));
                m_upfile->FrameCount = std::stoi(Payload.at(4));
                m_upfile->FrameNo = 0;

                isOK = "Ok";

                reFrame.rePayload += m_upfile->FileName;
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += isOK;
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += " ";

            }
            else
            {
                isOK = "ERR";

                reFrame.rePayload += m_upfile->FileName;
                reFrame.rePayload +=Fr_DDIV;
                reFrame.rePayload += isOK;
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += Payload.at(0) + Payload.at(1) + Payload.at(2) + Payload.at(3) + Payload.at(4) + Payload.at(5);

            }

            /**Send to mqtt**/
            std::string topicEnd = "UpOk";   // change to this topic!

            reFrame.reTopic = "Re" + topicEnd;
            this->m->seq_Num++;

            client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);


        }

    }

    if(cmd == FileData)
    {
        tReFrame reFrame;
        std::string isOK = "";
        std::string ErrInfo = "";
        if( Payload.at(0) == m_upfile->FileName)
        {
            LOG(INFO) << "prepare FileData";
            LOG(INFO) << "The FrameNo = " << m_upfile->FrameNo;
            LOG(INFO) << "GetNo = " << std::stoi(Payload.at(1));
            LOG(INFO) << "FrameCount = " << m_upfile->FrameCount;

            if(m_upfile->FrameNo == std::stoi(Payload.at(1)))
            {


                // append data will save in heap;
                // deconstructor function will free automaticlly!
                std::vector<uint8_t> pdata;
                /**when in last frame, FrameNo begins from 0,so +1**/
                if((m_upfile->FrameNo + 1) == m_upfile->FrameCount /*&& Payload.at(2).size() == m_upfile->FrameEndSize*/)
                {
                    LOG(INFO) << "this is the last frame";
                    LOG(INFO) << "FrameNo = " << m_upfile->FrameNo << ", LastFrameSize = " << m_upfile->FrameEndSize;

                    /*check sum**/

                    pdata.reserve((int)m_para->FrameInfo.FrameEachSize);
                    if(1)
                    {
                        ConfigWin::StringToHex(Payload[2], pdata);
                        //   m_upfile->FileBuffer.append(pdata);
                        m_upfile->FileBuffer.insert(m_upfile->FileBuffer.end(), pdata.begin(), pdata.end());

                        LOG(INFO) << "Receiving [Last] Update Frame..., No = " << m_upfile->FrameNo;
                        m_upfile->FrameNo++;

                        /*Judge whether Filesize is right**/
//                        qDebug() << "FileBuffer.size() = " << m_upfile->FileBuffer.size();
//                        qDebug() << "m_upfile->FileSize = " << m_upfile->FileSize;

                        if(m_upfile->FileBuffer.size() == std::stoi(m_upfile->FileSize))
                        {
                            /**write to file!**/
                            std::ofstream file;     // open file with binary
                            file.open(m_upfile->FileName,std::ios::out| std::ios::app | std::ios::binary);

                            file.write((char* )m_upfile->FileBuffer.data(), m_upfile->FileBuffer.size());

                            LOG(INFO) << "Update File Success!!";
                            isOK = "UPOK";
                            ErrInfo = " ";


                        }
                        else
                        {
                            isOK = "ERR";
                            ErrInfo = "FileSize ERR";

                        }

                        /**Send Mqtt back**/


                        reFrame.rePayload += m_upfile->FileName;
                        reFrame.rePayload += Fr_DDIV;
                        reFrame.rePayload += isOK;
                        reFrame.rePayload += Fr_DDIV;
                        reFrame.rePayload += ErrInfo;

                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        //emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                        client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);
                    }
                    else  // crc err
                    {
                        isOK = "ERR";
                        ErrInfo = "CRC ERR";

                        std::ostringstream of;
                        reFrame.rePayload += m_upfile->FileName;
                        reFrame.rePayload += Fr_DDIV;
                        reFrame.rePayload += std::to_string(m_upfile->FrameNo);
                        reFrame.rePayload += Fr_DDIV;
                        reFrame.rePayload += isOK;
                        reFrame.rePayload += Fr_DDIV;
                        reFrame.rePayload += ErrInfo;

                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        //emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                        client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);
                    }


//                    pdata.clear();
//                    pdata.squeeze();  // no need to clear buffer, it will clear by itself!


                }
                else /**when not in last frame**/
                {
                    /*check sum**/
                    std::vector<uint8_t> pdata;
                    pdata.reserve((int)m_para->FrameInfo.FrameEachSize);   //resize to FrameEachSize

                    //   Payload.at(4).toUShort() = checksum_16(pdata.data(),m_upfile->FrameEachSize);

                    if(1)
                    {
                        ConfigWin::StringToHex(Payload[2], pdata);


                        m_upfile->FileBuffer.insert(m_upfile->FileBuffer.end(), pdata.begin(), pdata.end());
//                        qDebug() << "the Filebuffer:****" << m_upfile->FileBuffer.size();
                        LOG(INFO) << "Receiving Update Frame..., No = " << m_upfile->FrameNo;
                        m_upfile->FrameNo++;
                        isOK = "Ok";   // little
                        ErrInfo = " ";


                    }
                    else  // crc err
                    {

                        isOK = "ERR";
                        ErrInfo = "Real FrameNo = " + std::to_string(m_upfile->FrameNo);


                    }

//                    pdata.clear();
//                    pdata.squeeze();  // no need to clear buffer, it will clear by itself!



                    reFrame.rePayload += m_upfile->FileName;
                    reFrame.rePayload += Fr_DDIV;
                    reFrame.rePayload += Payload.at(1);
                    reFrame.rePayload += Fr_DDIV;
                    reFrame.rePayload += isOK;
                    reFrame.rePayload += Fr_DDIV;
                    reFrame.rePayload += ErrInfo;

                    reFrame.reTopic = "Re" + topicEnd;
                    this->m->seq_Num++;
                    //emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                    client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);

                }
            }
            else   // if the first frame is not sync
            {
                LOG(INFO) << "this is the middle frame";
                isOK = "ERR";
                ErrInfo = "Real FrameNo = " + std::to_string(m_upfile->FrameNo);
                this->m->seq_Num++;

                reFrame.rePayload += m_upfile->FileName;
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += Payload.at(1);
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += isOK;
                reFrame.rePayload += Fr_DDIV;
                reFrame.rePayload += ErrInfo;


                reFrame.reTopic = "Re" + topicEnd;

                client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);

            }

        }
    }





}



void ThreadMqttListen::ExecReadBFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead,
                                     const std::vector<std::string>& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
    tReFrame reFrame;
    reFrame.reTopic = "Re" + topicEnd;

    std::string isOK = "";
    const int DataCount  = std::stoi(MesgHead.at(3));

    if(DataCount == Payload.size())
    {
        LOG(INFO) << "ReadB try to Lock ...";
        pthread_mutex_lock(&mm->mutex);
        LOG(INFO) << "ReadB Lock success! ...";
        for(auto it = Payload.begin(); it != Payload.end(); ++it)
        {
            /**Read From Breaker to rePayload**/
            reFrame.rePayload = PayloadReadB(std::stoi(*it), cmd);
            /**Send to mqtt**/
            if(!reFrame.rePayload.empty()){
                this->m->seq_Num++;
                client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);
            }

        }
        pthread_mutex_unlock(&mm->mutex);
    }

}




void ThreadMqttListen::ExecHandFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
    tReFrame reFrame;
    std::string isOK = "";
    std::string ErrInfo = "";
    const int DataCount  = std::stoi(MesgHead.at(3));

    if(DataCount == Payload.size())
    {

        /**Send to mqtt**/
        isOK = "OK";
        std::ostringstream of;
        of << isOK << Fr_DDIV << ErrInfo;
        reFrame.rePayload = of.str();

        reFrame.reTopic = "Re" + topicEnd;
        m->AddSeqNum();
        //emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD
    }
}


void ThreadMqttListen::ExecParaFunc(const std::string& topicEnd, std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload)
{

#if 1
    /***ReMessage******/
    tReFrame reFrame;
    std::string isOK = "";
    int flag = 0;

    if(Payload.size() == 4)
    {
        flag = m_conf->Para_Set_FrameAll(std::stoi(Payload.at(0)), std::stoi(Payload.at(1)), std::stoi(Payload.at(2)), std::stoi(Payload.at(3)));

        if(flag == 0)
        {
            isOK = "OK";
            reFrame.rePayload += isOK;
            reFrame.rePayload.push_back(Fr_DDIV);
        }
        else   // Err
        {
            isOK = "ERR";
            std::ostringstream of;
            of << isOK  << Fr_DDIV << std::to_string(flag);
            reFrame.rePayload = of.str();

        }
    }
    else
    {
        isOK = "ERR";
        std::ostringstream of;
        of << isOK  << Fr_DDIV << "DataCount InCorrect!";
        reFrame.rePayload = of.str();

    }


    reFrame.reTopic = "Re" + topicEnd;
    m->AddSeqNum();
    //emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD

#else
   while(true)
   {
        QThread::sleep(1);
        qDebug()  << "the Returnnnnnnnnnnnnnnnnnnnn" ;

   }

#endif

}



void ThreadMqttListen::ExecClearFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead, const std::vector<std::string>& Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    short cmdCode = 0x0080;
   // int index = MessageCmd.indexOf(Payload[0]);
    std::string isOK = "";
    uint16_t ReadReg = 0;


    std::unique_lock<std::mutex> lock(cmd_and_info_mutex);
//    QMutexLocker lock(&cmd_and_info_mutex);  // use mutex


    switch(hash_(Payload[0].data()))
    {

        case "ENE"_hash:
        case "FAUL"_hash:    // "FAUL"    clear fault number
        case "WAR"_hash:     // "WAR"
        case "COUNT"_hash:     // "COUNT"
        {
            LOG(INFO) << "Get cmd :" << "ENE";
            if(Payload.size() - 1 > MAXDEVNUM)
            {
                LOG(WARNING) << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.size();
                return;
            }
            /******control switch********/
            for(int i = 1; i < Payload.size(); i++)  // devID starts from 1
            {
                /*** send cmd to device***/
                m_serial->ModbusWriteOneRegister(std::stoi(Payload.at(i)), cmdCode + 0, 0x00);  //0 need to update clear power/faul/war/close number
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
         //   QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
            /******update status********/
            for(int i = 1; i < Payload.size(); i++)
            {
                /*** update device data***/
                auto it = m->KeyValFind(std::stoi(Payload.at(i)));
                if(m->IsKeyValExist(it, i) < 0)
                {

                    return;   //return  if devid does not exists!
                }

                if(Payload[0] == "WAR")
                {

                    m_serial->ModbusReadMultiRegister(std::stoi(Payload.at(i)), 0x002F, 2, &it->second->currentData->elecPw_HBit);// read hight and low bit;
                    ReadReg = it->second->currentData->elecPw_HBit;

                }
                else if(Payload[0] == "COUNT")
                {
                    m_serial->ModbusReadMultiRegister(std::stoi(Payload.at(i)), 0x0100, 1, &it->second->currentData->ErrNum);// read hight and low bit;
                    ReadReg = it->second->currentData->ErrNum;

                }
                else if(1)
                {
                    m_serial->ModbusReadMultiRegister(std::stoi(Payload.at(i)), 0x0062, 1, &it->second->currentData->lossElecWARVal);// read hight and low bit;
                    ReadReg = it->second->currentData->lossElecWARVal;

                }
                else
                {
                    m_serial->ModbusReadMultiRegister(std::stoi(Payload.at(i)), 0x0031, 1, &it->second->currentData->numOfSwitch);// read hight and low bit;
                    ReadReg = it->second->currentData->numOfSwitch;

                }

                if(ReadReg == 0)
                {
                    isOK = "OK";

                }
                else {
                    isOK = "ERR";
                }
                /*** send rcmd to mqtt***/
                /***RECMD: seq_No|TXT|@DateTime|devid|<recmd>|ON/OFF|OK/ERR*************/
                std::stringstream ss;
                std::string sndData;
                ss << std::hex << UartMesg.recvUartData;
                ss >> sndData;
               // tmp_recvUartData.append((char *)UartMesg.recvUartData, UartMesg.recvUartLen);  // golbal data from modbus


                std::ostringstream of;
                of << Payload[0] << Fr_DDIV << Payload[i] << Fr_DDIV << isOK << Fr_DDIV << sndData;
                reFrame.rePayload = of.str();

                reFrame.reTopic = "Re" + topicEnd;
                m->AddSeqNum();
                //emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD
            }

        }

        break;

        default:
            LOG(INFO) << "Get MessageCmd Error:" << Payload[0];

        break;



//    QByteArray tmp_recvUartData;
//    tmp_recvUartData.append((char *)UartMesg.recvUartData, UartMesg.recvUartLen);  // golbal data from modbus
//    if(tmp_recvUartData.at(1) == 0x86 || tmp_recvUartData.at(1) == 0x83)

}

}



void ThreadMqttListen::UpdateSwitchState(const std::string& topicEnd, std::vector<std::string>& Payload, int dev_count)
{

    short cmdCode = 0x0010;   // the addr
    uint8_t devid;

    std::string isOK = "";
    tReFrame reFrame;

    int cmdtype = 0;
    if(Payload.at(0) == "ON"){
        cmdtype = 0;
    }
    else{
        cmdtype = 1;
    }

    LOG(INFO) << "UpdateSwitchState Info";
    /******update status********/
 //   std::unique_lock<std::mutex> lock(info_and_stat_mutex);
//    QMutexLocker lock(&info_and_stat_mutex);  // use mutex
    for(int i = 1; i <= dev_count; i++)
    {

        /*** update device data***/
        devid = static_cast<uint8_t>(std::stoi(Payload.at(i)));
        auto it = m->KeyValFind(devid);
        if(m->IsKeyValExist(it, devid) < 0)
        {

            continue;   //return  if devid does not exists!   nono, change to continue!
        }

//        if(m->cmd_come_flag){
//            LOG(INFO) << "comes cmd, stop";
//            return;
//
//        }

        m_serial->ModbusReadMultiRegister(devid, cmdCode, 1, &it->second->currentData->openState);
        if(it->second->currentData->openState == cmdtype)   // ON:0--ok; OFF:1--ok
        {
            isOK = "OK";

        }
        else {
            isOK = "ERR";
        }
        /*** send rcmd to mqtt***/
        /***RECMD: seq_No|TXT|@DateTime|devid|<recmd>|ON/OFF|OK/ERR*************/
        std::stringstream ss;
        std::string sndData;
        ss << std::hex << UartMesg.recvUartData;
        ss >> sndData;


        reFrame.rePayload = Payload[0];  // cmdtype
        reFrame.rePayload += Fr_DDIV;
        reFrame.rePayload += Payload[i];   // devid
        reFrame.rePayload += Fr_DDIV;
        reFrame.rePayload += isOK;   // isOK
        reFrame.rePayload += Fr_DDIV;
        reFrame.rePayload += sndData;

        reFrame.reTopic = "Re" + topicEnd;
        m->AddSeqNum();
        client->publishMesg(m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);



    }


}




void ThreadMqttListen::ExecCmdFunc(const std::string& topicEnd, const std::vector<std::string>& MesgHead,  std::vector<std::string>& Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    short cmdCode = 0x0010;

    int SwitchCount = Payload.size() - 1;  //  miners "ON"
    std::string isOK = "";
    uint16_t ReadReg = 0;

    LOG(INFO) << "cmd try to Lock ...";
    pthread_mutex_lock(&mm->mutex);
    LOG(INFO) << "cmd Lock success! ...";

   // std::unique_lock<std::mutex> lock(cmd_and_info_mutex);  // it seems that process ,mutex are useful in pthread.


    switch(hash_(Payload[0].data()))
    {
        case "ON"_hash:    // "ON"
        {
            if(SwitchCount > MAXDEVNUM)
            {
                LOG(INFO) << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.size();
                return;
            }
            /******control switch********/
            for(int i = 1; i <= SwitchCount; i++)  // devID starts from 1
            {
                /*** send cmd to device***/
                 LOG(INFO) << "Controling Device" << Payload.at(i);
                m_serial->ModbusCtrlSwitch(std::stoi(Payload[i]), 0xff);
                 std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
            /******update status********/

         m->threadMqttListen->UpdateSwitchState(topicEnd, Payload, SwitchCount);
        }
        break;
        case "OFF"_hash:    // "OFF"
        {
            if(SwitchCount > MAXDEVNUM)
            {
                LOG(WARNING) << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.size();
                return;
            }
            /******control switch********/
            for(int i = 1; i <= SwitchCount; i++)  // devID starts from 1
            {
                /*** send cmd to device***/
                LOG(INFO) << "Controling Device" << Payload.at(i);
                m_serial->ModbusCtrlSwitch(std::stoi(Payload[i]), 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
            /******update status********/
            m->threadMqttListen->UpdateSwitchState(topicEnd, Payload, SwitchCount);
        }

        break;

        default:
            LOG(INFO) << "Get MessageCmd Error:" << Payload[0];
        break;

    }
    pthread_mutex_unlock(&mm->mutex);

}








