#include "gsvr_thread_report.h"
#include <QString>
#include <QtMqtt/QMqttClient>
#include "gsvr_info_mccb.h"
//#include "FireConfigure.h"
#include "gsvr_app_server.h"
#if defined(_WIN32)
#else
#include <sys/time.h>
#endif
#include <iostream>
#include "globalFun.h"

extern QMutex uart_mutex;


ThreadMqttReport::ThreadMqttReport(MainWindow* mwin, int index):m(mwin),_index(index)
{

     PicHex = new QByteArray;
     m_serial = m->serial;
     m_Client.local = m->getMqttClientLocal();
//    connect(this, &ThreadMqttReport::updateToJsSignals, this, &ThreadMqttReport::updateDataByJS);


}


ThreadMqttReport::~ThreadMqttReport()
{
    qDebug() << "deleting ThreadMqttReport";
    delete PicHex;


}






void ThreadMqttReport::ExecUpPicFunc(QString topicEnd, QStringList MesgHead, QStringList Payload)
{

    /***ReMessage******/
    tReFrame reFrame;
    QString isOK = "";

    QString filePathName = "pic-";
    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
    filePathName += ".png";
    QFile file(filePathName);

    SerialWindow::StringToHex(Payload[2], *PicHex);
    file.open(QIODevice::ReadWrite);
    file.write(*PicHex, PicHex->size());
    qDebug() << "the size of MessageHex.size() = " << PicHex->size();
    file.close();


}


QString ThreadMqttReport::DevDataToPayload(uint64_t devid, ENUM_TopicType cmd)
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
     //  regNum = 34;   // read 34 total registers
       InfoNum = 16;  // read 16 InfoNum;



       /**************** Clear Info History Data ******************/

       QMutexLocker locker(&uart_mutex);   //user Mutex to protect data
       if(m_serial->read_run_status(it.key(), &it.value()->currentData.runState) < 0 )  // but 1 times seems ok;
       {
           it.value()->IsOk = 1;
       }
       else {
           it.value()->IsOk = 0;
       }

       m_serial->read_abcphase_current(it.key(), &it.value()->currentData.currentI_E_A);
       uint32_t addr_lossI = 0x02910100;
       m_serial->read_one_registers(it.key(), addr_lossI, &it.value()->currentData.restIVal);
       m_serial->read_abcphase_voltage(it.key(), &it.value()->currentData.currentI_E_A);

       uint32_t addr_lossPhase = 0x02900000;
       m_serial->read_one_registers(it.key(), addr_lossPhase, &it.value()->currentData.lossPhaseType);

       uint32_t addr_lossIOver = 0x02910100;
       m_serial->read_one_registers(it.key(), addr_lossIOver, &it.value()->currentData.restIVal);

       uint32_t addr_timedrive = 0x02910200;
       m_serial->read_one_registers(it.key(), addr_timedrive, &it.value()->currentData.lossNoDriveTime);

       m_serial->read_swtcnt_num(it.key(), &it.value()->currentData.numOfSwitch);

       locker.unlock();
       QThread::msleep(100);  // sleep 100ms, to let other thread comes in!

       it.value()->DataInfoHandle();

       /**Put data to repayload**/

       QDateTime currTime = QDateTime::currentDateTime();
       QString strDate = currTime.toString("yyyy-MM-dd hh:mm:ss");
       rePayload =  strDate + Fr_DDIV + QString::number(devid, 10);                // add id + time

       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IsOpenState);   // add Open State
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IsWarState);   // add IsWarState
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IsFaultState);   // add IsFaultState
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.isRunState);   // add isRunState
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IA);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IB);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.IC);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.ILoss);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.UA);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.UB);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.UC);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.PhaseType);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.numOfSw);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.RestCurrentVal);
       rePayload += Fr_DDIV + QString::number(it.value()->InfoData.TimeOfLossUnDrive);

       return rePayload;

   }


   if(cmd == War)  //setting War
   {


   }


}

void ThreadMqttReport::ExecGetFileFunc()
{
    /***ReMessage******/
    tReFrame reFrame;
    QString topicEnd = "GetFile";


    reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV \
                // set filesize to 0 to satisity condition
                +"0" + Fr_DDIV \
                +m->UpFileInfo->CreateTime + Fr_DDIV \
                +m->UpFileInfo->ModifyTime + Fr_DDIV \
                +QString::number(m->FrameInfo.FrameEachSize,10);   // add Framesize;

    /**Public**/
    reFrame.reTopic = topicEnd;
    this->m->seq_Num++;
    emit mesgResponseInfoSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD to remote and local



}

// Analysis topic of MCCB.../Info/War/EqState
void ThreadMqttReport::ExecMCCBInfo()
{
    QMap<uint64_t, infoMCCB*>::iterator it;
    tReFrame reInfo;
    tReFrame reEqState;
//    tReFrame reWar;        no War topic in MCCB
    QString isDevOKList;

    /***Get DevInfo from Device****/
    for(it = this->m->KeyValBegin(); it != m->KeyValEnd(); ++it)
    {

         /************ Analysis Data **************************/
         // for Info
         reInfo.rePayload = DevDataToPayload(it.key(), Info);//(QString Topic, QString ClientID, QString DataType, QString PayLoad)
         reInfo.reTopic = "Info";
         this->m->AddSeqNum();
         emit mesgResponseInfoSignal(this->m->getSeqNum(), reInfo.reTopic, reInfo.rePayload);    // send RECMD to remote and local

//         // for Warn
//         reWar.rePayload = DevDataToPayload(it.key(), War);
//         reWar.reTopic = "War";
//         this->m->seq_Num++;
//         emit mesgResponseInfoSignal(this->m->seq_Num, reWar.reTopic, reWar.rePayload);

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




