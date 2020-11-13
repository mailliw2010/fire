#include "gsvr_thread_report.h"
#include <QString>
#include <QtMqtt/QMqttClient>
#include "gsvr_info_break.h"
//#include "FireConfigure.h"
#include "gsvr_app_server.h"
#if defined(_WIN32)
#else
#include <sys/time.h>
#endif
#include <iostream>
#include "globalFun.h"

extern QMutex uart_mutex;


ThreadMqttReport::ThreadMqttReport(MainServer* mwin, int index):m(mwin),_index(index)
{

     PicHex = new QByteArray;
     m_serial = m->serial;
     m_upfile = m->UpFileInfo;
     m_para = m->para;
     m_Client.local = m->getMqttClientLocal();
//    connect(this, &ThreadMqttReport::updateToJsSignals, this, &ThreadMqttReport::updateDataByJS);


}


ThreadMqttReport::~ThreadMqttReport()
{
    qDebug() << "deleting ThreadMqttReport";
    delete PicHex;


}






void ThreadMqttReport::ExecUpPicFunc(const QString& topicEnd, const QStringList& MesgHead, const QStringList& Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";

    QString filePathName = "pic-";
    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
    filePathName += ".png";
    QFile file(filePathName);

    ConfigWin::StringToHex(Payload[2], *PicHex);
    file.open(QIODevice::ReadWrite);
    file.write(*PicHex, PicHex->size());
    qDebug() << "the size of MessageHex.size() = " << PicHex->size();
    file.close();


}


QString ThreadMqttReport::DevDataToPayload(uint8_t devid, const ENUM_TopicType& cmd)
{
    uint16_t *regAddr = nullptr;
    double * doubleAddr = nullptr;
    int regNum = 0;
    int InfoNum = 0;
    int baseAddr = 0;
    QString rePayload = "";

    auto it = m->KeyValFind(devid);
    if(m->IsKeyValExist(it) < 0)
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
       regAddr = &it.value()->currentData->openState;

       /**************** Clear Info History Data ******************/
       uint8_t *ptr = reinterpret_cast<uint8_t *>(regAddr);
       memset(ptr, 0, static_cast<uint64_t>(regNum*2));


        /**Tradeoff way, must divide to 3 times**/
    //   this->m->setting->ModbusReadMultiRegister(devid, baseAddr, 13, regAddr);
    //   this->m->setting->ModbusReadMultiRegister(devid, baseAddr + 13, 13, regAddr + 13);
    //   this->m->setting->ModbusReadMultiRegister(devid, baseAddr + 13 + 13, 8, regAddr + 13 + 13);

    //   qDebug() << "ThreadReport: wait for unmutex...";
       QMutexLocker locker(&uart_mutex);   //user Mutex to protect data
       if(m_serial->ModbusReadMultiRegister(devid, baseAddr, regNum, regAddr) < 0 )  // but 1 times seems ok;
       {
           it.value()->IsOk = 1;
       }
       else {
           it.value()->IsOk = 0;
       }
       locker.unlock();
       QThread::msleep(300);  // sleep 100ms, to let other thread comes in!

       it.value()->DataInfoHandle();

       /**Put data to repayload**/
       doubleAddr = &it.value()->InfoData.IA;
       QDateTime currTime = QDateTime::currentDateTime();
       QString strDate = currTime.toString("yyyy-MM-dd hh:mm:ss");
       rePayload =  strDate + Fr_DDIV + QString::number(devid, 10);                // add id + time

       rePayload +=  Fr_DDIV + QString::number(it.value()->InfoData.IsOpenState);   // add Open State
       rePayload +=  Fr_DDIV + QString::number(it.value()->InfoData.IsWarState);   // add IsWarState
       rePayload +=  Fr_DDIV + QString::number(it.value()->InfoData.IsFaultState);   // add IsFaultState
       rePayload +=  Fr_DDIV + QString::number(it.value()->InfoData.isRunState);   // add isRunState


       for(int i = 0; i < InfoNum - 2 - 4; i++)   //updata InfoNum, exclude 2(time/id) + 4 state;
       {
            rePayload += Fr_DDIV + QString::number(*(doubleAddr + i));   // append double data by pointer

       }

       return rePayload;

   }


   if(cmd == War)  //setting War
   {
       regNum = 5;   // read 5 total registers
       InfoNum = 7;  // read 7 InfoNum;
       baseAddr = 0x0100;


       regAddr = &it.value()->currentData->ErrNum;


       /**************** Clear War History Data ******************/
       uint8_t *ptr = (uint8_t *)regAddr;
       memset(ptr, 0, (size_t)regNum*2);

    //   qDebug() << "ThreadReport: wait for unmutex...";
       QMutexLocker locker(&uart_mutex);   //user Mutex to protect data
       if(m_serial->ModbusReadMultiRegister(devid, baseAddr, regNum, regAddr) < 0 )
       {
           it.value()->IsOk = 1;
       }
       else {
           it.value()->IsOk = 0;
       }
       locker.unlock();
       QThread::msleep(300);  // sleep 100ms, to let other thread comes in!


       /**Put data to repayload**/
       QDateTime currTime = QDateTime::currentDateTime();
       QString strDate = currTime.toString("yyyy-MM-dd hh:mm:ss");
       rePayload =  strDate + Fr_DDIV + QString::number(devid, 10);                // add id + time

       rePayload +=  Fr_DDIV + QString::number(it.value()->currentData->ErrNum);   // add ErrNum
       rePayload +=  Fr_DDIV + QString::number(it.value()->currentData->ErrVal);   // add ErrVal
       rePayload +=  Fr_DDIV + QString::number(it.value()->currentData->ErrType);   // add ErrType
       rePayload +=  Fr_DDIV + QString::number(it.value()->currentData->ErrPhase);   // add ErrPhase
       rePayload +=  Fr_DDIV + QString::number(it.value()->currentData->ErrProtect);   // add ErrProtecte

       return rePayload;

   }


}

void ThreadMqttReport::ExecGetFileFunc()
{
    /***ReMessage******/
    tReFrame reFrame;
    QString topicEnd = "GetFile";


    reFrame.rePayload = m_upfile->FileName + Fr_DDIV \
                // set filesize to 0 to satisity condition
                +"0" + Fr_DDIV \
                +m_upfile->CreateTime + Fr_DDIV \
                +m_upfile->ModifyTime + Fr_DDIV \
                +QString::number(m_para->FrameInfo.FrameEachSize,10);   // add Framesize;

    /**Public**/
    reFrame.reTopic = topicEnd;
    this->m->AddSeqNum();
    emit mesgResponseInfoSignal(this->m->getSeqNum(), reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote and local



}

// Analysis topic of MCCB.../Info/War/EqState
void ThreadMqttReport::ExecUpMCCBInfo()
{
//    QMap<uint8_t, infoModel*>::iterator it;
//    tReFrame reInfo;
//    tReFrame reEqState;
////    tReFrame reWar;        no War topic in MCCB
//    QString isDevOKList;

//    /***Get DevInfo from Device****/
//    for(it = this->m->KeyValBegin(); it != m->KeyValEnd(); ++it)
//    {

//         /************ Analysis Data **************************/
//         // for Info
//         reInfo.rePayload = DevDataToPayload(it.key(), Info);//(QString Topic, QString ClientID, QString DataType, QString PayLoad)
//         reInfo.reTopic = "Info";
//         this->m->AddSeqNum();
//         emit mesgResponseInfoSignal(this->m->getSeqNum(), reInfo.reTopic, reInfo.rePayload);    // send RECMD to remote and local

////         // for Warn
////         reWar.rePayload = DevDataToPayload(it.key(), War);
////         reWar.reTopic = "War";
////         this->m->seq_Num++;
////         emit mesgResponseInfoSignal(this->m->seq_Num, reWar.reTopic, reWar.rePayload);

//         // for EqState
//         reEqState.rePayload += QString::number(it.key()) + "|";
//         isDevOKList += QString::number(it.value()->IsOk) + "|";

//    }

//    reEqState.reTopic = "EqState";
//    reEqState.rePayload.chop(1);   // remove the last char of "|"
//    isDevOKList.chop(1);           // remove the last char of "|"
//    reEqState.rePayload += Fr_DDIV;
//    reEqState.rePayload += isDevOKList;
//    this->m->seq_Num++;
//    emit mesgResponseInfoSignal(this->m->seq_Num, reEqState.reTopic, reEqState.rePayload);    // send RECMD to remote and local




}




// Analysis topic of Info/War/EqState
void ThreadMqttReport::ExecUpDevInfo()
{
    QMap<uint8_t, infoModel*>::iterator it;
    tReFrame reInfo;
    tReFrame reEqState;
    tReFrame reWar;
    QString isDevOKList;

    /***Get DevInfo from Device****/
    for(it = this->m->KeyValBegin(); it != m->KeyValEnd(); ++it)
    {

         /************ Analysis Data **************************/
        qDebug() << "*******Dev:" << it.key();
         // for Info
         reInfo.rePayload = DevDataToPayload(it.key(), Info);//(QString Topic, QString ClientID, QString DataType, QString PayLoad)
         reInfo.reTopic = "Info";
         this->m->AddSeqNum();
         emit mesgResponseInfoSignal(this->m->getSeqNum(), reInfo.reTopic, reInfo.rePayload);    // send RECMD to remote and local

         // for Warn
         reWar.rePayload = DevDataToPayload(it.key(), War);
         reWar.reTopic = "War";
         this->m->AddSeqNum();
         emit mesgResponseInfoSignal(this->m->getSeqNum(), reWar.reTopic, reWar.rePayload);

         // for EqState
         reEqState.rePayload += QString::number(it.key()) + "|";
         isDevOKList += QString::number(it.value()->IsOk) + "|";

    }

    reEqState.reTopic = "EqState";
    reEqState.rePayload.chop(1);   // remove the last char of "|"
    isDevOKList.chop(1);           // remove the last char of "|"
    reEqState.rePayload += Fr_DDIV;
    reEqState.rePayload += isDevOKList;
    this->m->AddSeqNum();
    emit mesgResponseInfoSignal(this->m->getSeqNum(), reEqState.reTopic, reEqState.rePayload);    // send RECMD to remote and local


}

