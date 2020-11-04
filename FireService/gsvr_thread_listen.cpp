#include "gsvr_thread_listen.h"
#include <QString>
#include <QtMqtt/QMqttClient>
#include "ui_config_win.h"
#include "gsvr_app_server.h"
#include "gsvr_app_config.h"
#include "globalFun.h"
#include<QtConcurrent/QtConcurrent>

using namespace _mqtt_topic;
extern QMutex uart_mutex;
ThreadMqttListen::ThreadMqttListen(MainServer* mwin,  int index):_index(index),m(mwin)
{
    m_serial = m->serial;
    m_para = m->para;
    m_upfile = m->UpFileInfo;
    m_conf = m->view->getSetobj();


    m_topic = m_conf->Para_Get_ProductInfo();


}


ThreadMqttListen::~ThreadMqttListen()
{
    qDebug() << "deleting ThreadMqttListen";



}




void ThreadMqttListen::mesgResponseSlot()
{




}

// ClientID + DataType + DataCount + PayLoad








void ThreadMqttListen::GetParaFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload)
{
    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";


    if(Payload.count() == 1)
    {
        if(Payload[0] == "ASK")
        {
            reFrame.rePayload = QString::number(m_para->FrameInfo.FrameEachSize,10) + Fr_DDIV + QString::number(m_para->FrameInfo.OverTime,10) + Fr_DDIV + \
                        QString::number(m_para->FrameInfo.ReSendCount,10) + Fr_DDIV + QString::number(m_para->FrameInfo.AutoUploadTime,10);


        }


        reFrame.reTopic = _mqtt_topic::Topic_Head + "Re" + topicEnd;
        this->m->seq_Num++;
        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote/local server


    }




}


QString ThreadMqttListen::PayloadDataAnalysis(const QStringList& paylist, const ENUM_TopicType& cmd)
{
    QList<uint16_t>datalist;
    int regNum = 0;
    int baseAddr = 0;
    uint8_t devid = (uint8_t)paylist.at(0).toInt();
    const int RegCount  = paylist.count() - 1;    // exclude devid
    int flag = 0;
    QString rePayload = "";
    QString isOK = "";

    if(cmd == SetBVal && RegCount == 13)
    {
        baseAddr = 0X0040;
        datalist.append((uint16_t)paylist.at(1).toDouble()*10);
        datalist.append((uint16_t)paylist.at(2).toDouble());
        datalist.append((uint16_t)paylist.at(3).toDouble()*10);
        datalist.append((uint16_t)paylist.at(4).toDouble());
        datalist.append((uint16_t)paylist.at(5).toDouble()*10);
        datalist.append((uint16_t)paylist.at(6).toDouble());
        datalist.append((uint16_t)paylist.at(7).toDouble()*10);
        datalist.append((uint16_t)paylist.at(8).toDouble());
        datalist.append((uint16_t)paylist.at(9).toDouble());
        datalist.append((uint16_t)paylist.at(10).toDouble());
        datalist.append((uint16_t)paylist.at(11).toDouble()*10);
        datalist.append((uint16_t)paylist.at(12).toDouble()*10);
        datalist.append((uint16_t)paylist.at(13).toDouble());
    }

    if(cmd == SetBEn && RegCount == 5)
    {
        baseAddr = 0X0050;
        datalist.append((uint16_t)paylist.at(1).toInt());
        datalist.append((uint16_t)paylist.at(2).toInt());
        datalist.append((uint16_t)paylist.at(3).toInt());
        datalist.append((uint16_t)paylist.at(4).toInt());
        datalist.append((uint16_t)paylist.at(5).toInt());
    }

    if(cmd == SetBCor && RegCount == 11)
    {
        baseAddr = 0X006A;
        datalist.append((uint16_t)paylist.at(1).toDouble()*10);
        datalist.append((uint16_t)paylist.at(2).toDouble()*10);
        datalist.append((uint16_t)paylist.at(3).toDouble()*10);
        datalist.append((uint16_t)paylist.at(4).toDouble()*10);
        datalist.append((uint16_t)paylist.at(5).toDouble()*10);
        datalist.append((uint16_t)paylist.at(6).toDouble()*10);
        datalist.append((uint16_t)paylist.at(7).toDouble()*10);
        datalist.append((uint16_t)paylist.at(8).toDouble());
        datalist.append((uint16_t)paylist.at(9).toDouble());
        datalist.append((uint16_t)paylist.at(10).toDouble());
        datalist.append((uint16_t)paylist.at(11).toDouble());
    }

    /*** send value to device***/
    for(int i = 0; i < RegCount; i++)  //
    {
        m_serial->ModbusWriteOneRegister(devid, baseAddr+i, datalist.at(i));
    }

    QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
    /******update status********/

    auto it = m->KeyValFind(devid);
    if(m->IsKeyValExist(it) < 0)
    {

        return "";   //return  if devid does not exists!
    }


    m_serial->ModbusReadMultiRegister(devid, baseAddr, RegCount, &it.value()->currentData->OverProtectI);  // except id
    uint16_t * pdata = &it.value()->currentData->OverProtectI;

    for(int i = 0; i < RegCount; i++)  //
    {
        if(*(pdata++) != datalist.at(i))
        {
            flag |= 1 << i;
        }

    }

    if(flag == 0)  // judge whether setting is ok
    {
        isOK = "OK";
        //[DevID][0x1F][OK/ERR][0x1F][ErrorInfo]
        rePayload = paylist.at(0) + Fr_DDIV + isOK  + Fr_DDIV;

    }
    else   // Err
    {
        isOK = "ERR";
        rePayload = paylist.at(0) + Fr_DDIV + isOK  + Fr_DDIV;
        rePayload += QString::number(flag,2);   //turn error code to 2

    }

    return rePayload;

}




/**SetVal, SetEn, SetCorrrect Function**/
void ThreadMqttListen::ExecSetBFunc(const QString& topicEnd, const QStringList& MesgHead,
                                    const QStringList& Payload, const ENUM_TopicType& cmd)
{
    const int DataCount  = MesgHead.at(3).toInt();

    /***ReMessage******/
    tReFrame reFrame;

    qDebug() << "ThreadListen wait for unmutex...";
    QMutexLocker lock(&uart_mutex);  // use mutex

    if(Payload.count() == DataCount)
    {
        /**Analisis Data to datalist, put to rePayload**/
        reFrame.rePayload = PayloadDataAnalysis(Payload, cmd);


        /*** send rcmd to mqtt***/



        /***RECMD: seq_No|TXT|@DateTime|devid|<recmd>|ON/OFF|OK/ERR*************/
        reFrame.reTopic = _mqtt_topic::Topic_Head + "Re" + topicEnd;
        this->m->seq_Num++;
        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote server
           // send RECMD to local server

    }



}

QString ThreadMqttListen::DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd)
{
    uint16_t * addr = nullptr;
    double * doubleAddr = nullptr;
    int regNum = 0;
    int InfoNum = 0;
    int baseAddr = 0;
    QString rePayload = "";
    /**Need to set regNum/baseAddr**/
   if(cmd == SetBVal)  //default setting
   {
       regNum = 13;
       InfoNum = 33;
       baseAddr = 0x0040;

   }

   if(cmd == SetBEn)
   {
       regNum = 9;
       InfoNum = 33;
       baseAddr = 0x0050;

   }

   if(cmd == SetBCor)
   {
       regNum = 11;
       InfoNum = 33;
       baseAddr = 0x006A;

   }

   auto it = m->KeyValFind(devid);
   if(m->IsKeyValExist(it) < 0)
   {

       return "";   //return  if devid does not exists!
   }


   addr = &it.value()->currentData->OverProtectI;

   qDebug() << "Concurrent wait for unmutex...";
   QMutexLocker lock(&uart_mutex);  // use mutex
   m_serial->ModbusReadMultiRegister(devid, baseAddr, regNum, addr);
   lock.unlock();
   it.value()->DataSettingHandle();

   /**Put data to repayload**/
   doubleAddr = &it.value()->SetBData.OverProtectI;
   rePayload += QString::number(devid, 10);   //dev id
   for(int i = 0; i < InfoNum; i++)  //updata InfoNum;
   {
       rePayload += Fr_DDIV + QString::number(*(doubleAddr + i));   // append double data by pointer

   }

   return rePayload;


}


void ThreadMqttListen::ExecUpFileFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";
    QString ErrInfo = "";

    const int DataCount  = MesgHead.at(3).toInt();

    if(cmd == UpInfo)
    {
        if(DataCount == Payload.count() && DataCount == 4)
        {
            // if One of them id unequel, then update file
            if(m_upfile->FileSize != Payload.at(1) || m_upfile->CreateTime != Payload.at(2) || m_upfile->ModifyTime != Payload.at(3))
            {
                m_upfile->FileName = Payload.at(0);   //file name
                m_upfile->FileSize = Payload.at(1);    //file size
//                m_upfile->CreateTime = Payload.at(2);    //file create time
//                m_upfile->ModifyTime = Payload.at(3);    //file modify time
                m_upfile->CreateTime = "2020-09-20 17:14:39";    //file create time
                m_upfile->ModifyTime = "2020-09-20 17:14:39";    //file modify time


                emit askGetFileSignal();    // send signal to thread Report
            }



        }

    }

    if(cmd == ReGetFile)
    {
        if(DataCount == Payload.count() && DataCount == 5)
        {
            // if != 0, update
            if(Payload.at(1) != "0")
            {
                m_upfile->FileName = Payload.at(0);   //file name
                m_upfile->FrameEachSize = Payload.at(1).toInt();
                m_upfile->FrameEndSize = Payload.at(2).toInt();
                m_upfile->FrameAllSize = Payload.at(3).toInt();
                m_upfile->FrameCount = Payload.at(4).toInt();
                m_upfile->FrameNo = 0;

                isOK = "OK";

                reFrame.rePayload = m_upfile->FileName + Fr_DDIV + isOK + Fr_DDIV;

             }
            else
            {
                isOK = "ERR";
                reFrame.rePayload = m_upfile->FileName + Fr_DDIV + isOK + Fr_DDIV + \
                Payload.at(0) + Payload.at(1) + Payload.at(2) + Payload.at(3) + \
                Payload.at(4) + Payload.at(5);


            }

            /**Send to mqtt**/
            QString topicEnd = "UpOk";   // change to this topic!
            reFrame.reTopic = "Re" + topicEnd;
            this->m->seq_Num++;
            emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD

        }

    }

    if(cmd == FileData)
    {

        if( Payload.at(0) == m_upfile->FileName)
        {
            if(m_upfile->FrameNo == Payload.at(1).toInt())
            {
                /**when in last frame, FrameNo begins from 0,so +1**/
                  QByteArray pdata;   // append data will save in heap; deconstructor function will free automaticlly!
//                qDebug() << "The FrameNo = " << m_upfile->FrameNo << ", GetNo = " << Payload.at(1).toInt() << "FrameCount = " << m_upfile->FrameCount;
                if((m_upfile->FrameNo + 1) == m_upfile->FrameCount /*&& Payload.at(2).size() == m_upfile->FrameEndSize*/)
                {
//                    qDebug() << "FrameNo = " << m_upfile->FrameNo << ", LastFrameSize = " << m_upfile->FrameEndSize;

                    /*check sum**/

                    pdata.reserve((int)m_para->FrameInfo.FrameEachSize);
                    if(1)
                    {
                        ConfigWin::StringToHex(Payload[2], pdata);
                        m_upfile->FileBuffer.append(pdata);
                        qInfo() << "Receiving [Last] Update Frame..., No = " << m_upfile->FrameNo;
                        m_upfile->FrameNo++;

                        /*Judge whether Filesize is right**/
//                        qDebug() << "FileBuffer.size() = " << m_upfile->FileBuffer.size();
//                        qDebug() << "m_upfile->FileSize = " << m_upfile->FileSize;

                        if(m_upfile->FileBuffer.size() == m_upfile->FileSize.toInt())
                        {
                            /**write to file!**/
                            QFile file(m_upfile->FileName);
                            file.open(QIODevice::ReadWrite);
                            file.write(m_upfile->FileBuffer, m_upfile->FileBuffer.size());
                            qInfo() << "Update File Success!!";
                            isOK = "UPOK";
                            ErrInfo = "";


                        }
                        else
                        {
                            isOK = "ERR";
                            ErrInfo = "FileSize ERR";

                        }

                        /**Send Mqtt back**/
                        reFrame.rePayload = m_upfile->FileName + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;

                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                    }
                    else  // crc err
                    {
                        isOK = "ERR";
                        ErrInfo = "CRC ERR";

                        reFrame.rePayload = m_upfile->FileName + Fr_DDIV + QString::number(m_upfile->FrameNo,10) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                    }


//                    pdata.clear();
//                    pdata.squeeze();  // no need to clear buffer, it will clear by itself!


                }
                else /**when not in last frame**/
                {
                    /*check sum**/
                    QByteArray pdata;
                    pdata.reserve((int)m_para->FrameInfo.FrameEachSize);   //resize to FrameEachSize

                 //   Payload.at(4).toUShort() = checksum_16(pdata.data(),m_upfile->FrameEachSize);

                    if(1)
                    {
                        ConfigWin::StringToHex(Payload[2], pdata);
                        m_upfile->FileBuffer.append(pdata);
//                        qDebug() << "the Filebuffer:****" << m_upfile->FileBuffer.size();
                        qInfo() << "Receiving Update Frame..., No = " << m_upfile->FrameNo;
                        m_upfile->FrameNo++;
                        isOK = "OK";
                        ErrInfo = "";


                    }
                    else  // crc err
                    {

                        isOK = "ERR";
                        ErrInfo = "Real FrameNo = " + QString::number(m_upfile->FrameNo,10);


                    }

//                    pdata.clear();
//                    pdata.squeeze();  // no need to clear buffer, it will clear by itself!

                    reFrame.rePayload = m_upfile->FileName + Fr_DDIV + Payload.at(1) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                    reFrame.reTopic = "Re" + topicEnd;
                    this->m->seq_Num++;
                    emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD

                }
            }
            else   // if the first frame is not sync
            {
                isOK = "ERR";
                ErrInfo = "Real FrameNo = " + QString::number(m_upfile->FrameNo,10);
                this->m->seq_Num++;
                reFrame.rePayload = m_upfile->FileName + Fr_DDIV + Payload.at(1) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                reFrame.reTopic = "Re" + topicEnd;
                emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD

            }

        }
    }








}





void ThreadMqttListen::ExecReadBFunc(const QString& topicEnd, const QStringList& MesgHead,
                                     const QStringList& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
    tReFrame reFrame;
    reFrame.reTopic = "Re" + topicEnd;

    QString isOK = "";
    const int DataCount  = MesgHead.at(3).toInt();

    if(DataCount == Payload.count())
    {
        for(int i = 0; i < DataCount; ++i)
        {
            /**Read From Breaker to rePayload**/
            reFrame.rePayload = DevDataToPayload((uint8_t)Payload.at(i).toInt(), cmd);
            /**Send to mqtt**/
            this->m->seq_Num++;
            emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
        }
    }

}




void ThreadMqttListen::ExecHandFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload, const ENUM_TopicType& cmd)
{
    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";
    QString ErrInfo = "";
    const int DataCount  = MesgHead.at(3).toInt();

    if(DataCount == Payload.count())
    {

        /**Send to mqtt**/
        isOK = "OK";
        reFrame.rePayload = isOK + Fr_DDIV + ErrInfo;

        reFrame.reTopic = "Re" + topicEnd;
        m->AddSeqNum();
        emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD
    }
}


void ThreadMqttListen::ExecParaFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload)
{

#if 1
    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";
    int flag = 0;

    if(Payload.count() == 4)
    {
        flag = m_conf->Para_Set_FrameAll(Payload.at(0).toUInt(), Payload.at(1).toUInt(), Payload.at(2).toUInt(), Payload.at(3).toUInt());

        if(flag == 0)
        {
            isOK = "OK";
            reFrame.rePayload = isOK  + Fr_DDIV;
        }
        else   // Err
        {
            isOK = "ERR";
            reFrame.rePayload = isOK  + Fr_DDIV;
            reFrame.rePayload += QString::number(flag,10);

        }
    }
    else
    {
        isOK = "ERR";
        reFrame.rePayload = isOK  + Fr_DDIV;
        reFrame.rePayload += "DataCount InCorrect!";

    }


    reFrame.reTopic = "Re" + topicEnd;
    m->AddSeqNum();
    emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD

#else
   while(true)
   {
        QThread::sleep(1);
        qDebug()  << "the Returnnnnnnnnnnnnnnnnnnnn" ;

   }

#endif

}



void ThreadMqttListen::ExecClearFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    short cmdCode = 0x0080;
    int index = MessageCmd.indexOf(Payload[0]);
    QString isOK = "";
    uint16_t ReadReg = 0;

    qDebug() << QThread::currentThreadId() << " wait for unmutex...";
    QMutexLocker lock(&uart_mutex);  // use mutex

    switch(index)
    {
        case 2:    // "ENE"
        case 3:    // "FAUL"    clear fault number
        case 4:     // "WAR"
        case 5:     // "COUNT"
        {
            qDebug() << "Get cmd :" << "ENE";
            if(Payload.count() - 1 > MAXDEVNUM)
            {
                qDebug() << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.count();
                return;
            }
            /******control switch********/

            for(int i = 1; i < Payload.count(); i++)  // devID starts from 1
            {
                /*** send cmd to device***/
                m_serial->ModbusWriteOneRegister(Payload.at(i).toInt(), cmdCode + index, 0x00);  //clear power/faul/war/close number
            }

            QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
            /******update status********/
            for(int i = 1; i < Payload.count(); i++)
            {
                /*** update device data***/
                auto it = m->KeyValFind((uint8_t)Payload.at(i).toInt());
                if(m->IsKeyValExist(it) < 0)
                {

                    return;   //return  if devid does not exists!
                }

                if(index == 2)
                {

                    m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), 0x002F, 2, &it.value()->currentData->elecPw_HBit);// read hight and low bit;
                    ReadReg = it.value()->currentData->elecPw_HBit;

                }
                else if(index == 3)
                {
                    m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), 0x0100, 1, &it.value()->currentData->ErrNum);// read hight and low bit;
                    ReadReg = it.value()->currentData->ErrNum;

                }
                else if(index == 4)
                {
                    m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), 0x0062, 1, &it.value()->currentData->lossElecWARVal);// read hight and low bit;
                    ReadReg = it.value()->currentData->lossElecWARVal;

                }
                else
                {
                    m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), 0x0031, 1, &it.value()->currentData->numOfSwitch);// read hight and low bit;
                    ReadReg = it.value()->currentData->numOfSwitch;

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
                QByteArray tmp_recvUartData;
                tmp_recvUartData.append((char *)UartMesg.recvUartData, UartMesg.recvUartLen);  // golbal data from modbus

                QString sndData = tmp_recvUartData.toHex(' ').toUpper();    // turn to hex, upper
                reFrame.rePayload = Payload[0] + Fr_DDIV + Payload[i] + Fr_DDIV + isOK + Fr_DDIV + sndData;

                reFrame.reTopic = "Re" + topicEnd;
                m->AddSeqNum();
                emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD
            }

        }

        break;

        default:
            qDebug() << "Get MessageCmd Error:" << Payload[0];

        break;



//    QByteArray tmp_recvUartData;
//    tmp_recvUartData.append((char *)UartMesg.recvUartData, UartMesg.recvUartLen);  // golbal data from modbus
//    if(tmp_recvUartData.at(1) == 0x86 || tmp_recvUartData.at(1) == 0x83)

}

}


void ThreadMqttListen::ExecCmdFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    short cmdCode = 0x0010;
    int index = MessageCmd.indexOf(Payload[0]);
    int SwitchCount = Payload.count() - 1;  //  miners "ON"
    QString isOK = "";
    uint16_t ReadReg = 0;
    qDebug() << QThread::currentThreadId() << " wait for unmutex...";
    QMutexLocker lock(&uart_mutex);  // use mutex
    switch(index)
    {
        case 0:    // "ON"
        if(SwitchCount > MAXDEVNUM)
        {
            qDebug() << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.count();
            return;
        }
        /******control switch********/
        for(int i = 1; i <= SwitchCount; i++)  // devID starts from 1
        {
            /*** send cmd to device***/
             qDebug() << "Controling Device" << Payload.at(i);
            m_serial->ModbusCtrlSwitch(Payload.at(i).toInt(), 0xff);
             QThread::msleep(50);
        }

        QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
        /******update status********/

        for(int i = 1; i <= SwitchCount; i++)
        {

            /*** update device data***/
            auto it = m->KeyValFind((uint8_t)Payload.at(i).toInt());
            if(m->IsKeyValExist(it) < 0)
            {

                return ;   //return  if devid does not exists!
            }

            m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), cmdCode, 1, &it.value()->currentData->openState);
            if(it.value()->currentData->openState == 0)   // open
            {
                isOK = "OK";

            }
            else {
                isOK = "ERR";
            }
            /*** send rcmd to mqtt***/
            /***RECMD: seq_No|TXT|@DateTime|devid|<recmd>|ON/OFF|OK/ERR*************/
            QByteArray tmp_sndUartData;
            tmp_sndUartData.append((char *)UartMesg.sndUartData, UartMesg.sndUartLen);  // golbal data from modbus
            QString sndData = tmp_sndUartData.toHex(' ').toUpper();    // turn to hex, upper
            reFrame.rePayload = Payload[0] + Fr_DDIV + Payload[i] + Fr_DDIV + isOK + Fr_DDIV + sndData;
            reFrame.reTopic = "Re" + topicEnd;
            m->AddSeqNum();
            emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD

        }

        break;
        case 1:    // "OFF"

        if(SwitchCount > MAXDEVNUM)
        {
            qDebug() << "Error! device Num is >" << MAXDEVNUM <<", DeviceID.count = " << Payload.count();
            return;
        }
        /******control switch********/
        for(int i = 1; i <= SwitchCount; i++)  // devID starts from 1
        {
            /*** send cmd to device***/
            qDebug() << "Controling Device" << Payload.at(i);
            m_serial->ModbusCtrlSwitch(Payload.at(i).toInt(), 0);
            QThread::msleep(50);

        }

        QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
        /******update status********/
        for(int i = 1; i <= SwitchCount; i++)
        {
            /*** update device data***/

            auto it = m->KeyValFind((uint8_t)Payload.at(i).toInt());
            if(m->IsKeyValExist(it) < 0)
            {

                return ;   //return  if devid does not exists!
            }
            m_serial->ModbusReadMultiRegister(Payload.at(i).toInt(), cmdCode, 1, &it.value()->currentData->openState);
            if(it.value()->currentData->openState == 1)   // close
            {
                isOK = "OK";

            }
            else {
                isOK = "ERR";
            }
            /*** send rcmd to mqtt***/
            /***RECMD: seq_No|TXT|@DateTime|devid|<recmd>|ON/OFF|OK/ERR*************/
            QByteArray tmp_sndUartData;
            tmp_sndUartData.append((char *)UartMesg.sndUartData, UartMesg.sndUartLen);  // golbal data from modbus
            QString sndData = tmp_sndUartData.toHex(' ').toUpper();    // turn to hex, upper
            reFrame.rePayload = Payload[0] + Fr_DDIV + Payload[i] + Fr_DDIV + isOK + Fr_DDIV + sndData;

            reFrame.reTopic = "Re" + topicEnd;
            m->AddSeqNum();
            emit mesgResponseSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD

        }


        break;

        default:
            qDebug() << "Get MessageCmd Error:" << Payload[0];
        break;

    }

}




/***handle in sync way, when recv mesg, handle to control switch***/
void ThreadMqttListen::RecvHandle(const QByteArray &message, const QMqttTopicName &topic)
{

// this is for window display
#if 0


    const QString content = "TOPIC:" + topic.name() + QDateTime::currentDateTime().toString()
    + QLatin1String(" Received Topic: ")
    + topic.name()
    + QLatin1String(" Message: ")
    + message
    + QLatin1Char('\n');    //content. time + topic + Message
   // MainServer::ui->->editLog->insertPlainText(content);   //put content to editlog

    static int AllSeqNum = 0;  // calculate all data
    if(AllSeqNum % 30 == 0)
        this->m->setting->ui->editLog->insertPlainText(content);   //put content to editlog

    AllSeqNum++;



#endif

    /****
     * Frame = Topic , MessageConTent
   @@Topic:
   [产品代号]/[片区号]/[ID]/[TopicDataType]
    FIRE/CQ-BS/1/Cmd
    FIRE/CQ-BS/1/ReCmd

    FIRE/CQ-BS/1/Clear
    FIRE/CQ-BS/1/ReClear

    FIRE/CQ-BS/1/Info
    FIRE/CQ-BS/1/War

    use FIRE/CQ-BS/1/#   to subscribe all related topic

    @@Message:
    [MessageCmd][MessageDeviceID]   

    ex:
     For Topic CMD:
     [SeqNo][0x1F][Clientid][0x1F][DataType][0x1F][DataCount]ON@1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18
     OFF@1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18

     For Topic CLEAR:
     ENE@1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18
     FAULT@1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18
     WAR@1|2|3|4|5|6|7|8|10|11|12|13|14|15|16|17|18
     COUNT@1|2|3|4|5|6|7|8|10|11|12|13|14|15[0x1F]16|17|18



    May Use function:
    str.trimed();
    QStringList list = date.split("/");//QString字符串分割函数
    ***/

    /*    TopicDataType << "CMD" << "RECMD" << "CLEAR" << "RECLEAR" << "INFO";
          MessageCmd << "ON" << "OFF" << "ENE" << "FAULT" << "WAR" << "COUNT";

    */




    /***Topic******/
    QStringList TopicFrame = topic.name().trimmed().split('/');

    if(m_topic.ID != TopicFrame[0] || m_topic.AREA != TopicFrame[1] || m_topic.NUM != TopicFrame[2])
    {
        qDebug() << "Get Incorrect Topic:" << topic.name();
        return;

    }

    /***Message******/

    QString Message(message.chopped(1).remove(0,1));    //remove Fr_Head and Fr_End, byte array to string
    QStringList MessageContent = Message.trimmed().split(Fr_RDIV);


    if(MessageContent.count() == 1)
    {
        qDebug() << "MessageData Error:" << Message;
        return;
    }



    QStringList MessageHead = MessageContent[0].trimmed().split(Fr_DDIV);;
    QStringList Payload = MessageContent[1].trimmed().split(Fr_DDIV);


//    QMap<uint8_t, infoModel*>::iterator it;
    /**if we get the message, the topic format is right!***/


    switch(TopicType.indexOf(TopicFrame[3]))
    {
        case FrameInfo:
        {
            qDebug() << "Get Topic:" << "FrameInfo";
            // ExecParaFunc(TopicFrame[3], MessageHead, Payload);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecParaFunc, TopicFrame[3], MessageHead, Payload);

        }
        break;

        case GetPara:
        {
            qDebug() << "Get Topic:" << "GetPara";
            //GetParaFunc(TopicFrame[3], MessageHead, Payload);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::GetParaFunc, TopicFrame[3], MessageHead, Payload);


        }

        break;

        case Info:  //Info
        break;

        case Cmd:   //CMD
            qDebug() << "Get Topic:" << "Cmd";
            ExecCmdFunc(TopicFrame[3], MessageHead, Payload);

        break;

        case ReCmd:    // RECMD
        break;

        case Clear:   //"CLEAR"      "ENE" << "FAULT" << "WAR" << "COUNT"
            qDebug() << "Get Topic:" << "Clear";
            ExecClearFunc(TopicFrame[3], MessageHead, Payload);
        break;

        case ReClear:  //RECLEAR
        break;

        case Fire_TimeSyn:  //Fire_TimeSyn
            qDebug() << "Get Topic:" << "TimeSyn";
            if(Payload.count() == 1)
            {
                m->SetSysTime(Payload[0]);

            }
        break;

        case SetBVal:  //RECLEAR
        {
            qDebug() << "Get Topic:" << "SetBVal";
           // ExecSetBFunc(TopicFrame[3], MessageHead, Payload, SetBVal);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecSetBFunc, TopicFrame[3], MessageHead, Payload, SetBVal);
        }

        break;

        case ReadBVal:
        {
            qDebug() << "Get Topic:" << "ReadBVal";
           // ExecReadBFunc(TopicFrame[3], MessageHead, Payload, ReadBVal);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecReadBFunc, TopicFrame[3], MessageHead, Payload, ReadBVal);
        }

        break;

        case SetBEn:
        {
            qDebug() << "Get Topic:" << "SetBEn";
           // ExecSetBFunc(TopicFrame[3], MessageHead, Payload, SetBEn);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecSetBFunc, TopicFrame[3], MessageHead, Payload, SetBEn);
        }

        break;

        case ReadBEn:
        {
            qDebug() << "Get Topic:" << "ReadBEn";
           // ExecReadBFunc(TopicFrame[3], MessageHead, Payload, ReadBEn);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecReadBFunc, TopicFrame[3], MessageHead, Payload, ReadBEn);
        }

        break;

        case SetBCor:
        {
            qDebug() << "Get Topic:" << "SetBCor";
           // ExecSetBFunc(TopicFrame[3], MessageHead, Payload, SetBCor);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecSetBFunc, TopicFrame[3], MessageHead, Payload, SetBCor);
        }

        break;

        case ReadBCor:
        {
            qDebug() << "Get Topic:" << "ReadBCor";
           // ExecReadBFunc(TopicFrame[3], MessageHead, Payload, ReadBCor);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecReadBFunc, TopicFrame[3], MessageHead, Payload, ReadBCor);
        }


        break;

        case GwState:

        break;

        case UpInfo:
            qDebug() << "Get Topic:" << "UpInfo";
            ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, UpInfo);

        break;

        case ReGetFile:
            qDebug() << "Get Topic:" << "ReGetFile";
            ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, ReGetFile);

        break;

        case FileData:
        {
            qDebug() << "Get Topic:" << "FileData";
           // ExecUpFileFunc(TopicFrame[3], MessageHead, Payload, FileData);
           // put the expensive event to one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecUpFileFunc, TopicFrame[3], MessageHead, Payload, FileData);
        }


    break;

        case Hand:
        {
            qDebug() << "Get Topic:" << "Hand";
            qDebug() << "Topic:" << TopicFrame[3];
            qDebug() << "****************"<< Payload;
           // ExecHandFunc(TopicFrame[3], MessageHead, Payload, Hand);
            // put the function to exec in one new thread!
            QFuture<void> f1 = QtConcurrent::run(this, &ThreadMqttListen::ExecHandFunc, TopicFrame[3], MessageHead, Payload, Hand);
        }


        break;




/**transfer picture by mqtt protocol-- just for test
 * StringToHex() is a static function, which has no object.
 * PicHex has better to be storaged to heap(new). cause it may be big. the size can be less than memory in theory
 *

***/
    case UpPic:  //UpLoad
        qDebug() << "Get DataType of" << "UpPic";


        break;

    default:

        break;


    }







}


void ThreadMqttListen::Myslot()
{
    qDebug()<<"from ThreadMqttListen slot:" <<QThread::currentThreadId();



}
