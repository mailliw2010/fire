#include "gsvr_thread_listen.h"
#include <QString>
#include <QtMqtt/QMqttClient>
#include "ui_serialwindow.h"
#include "gsvr_app_server.h"
#include "globalFun.h"
#include<QtConcurrent/QtConcurrent>

using namespace _pubvar;
extern QMutex uart_mutex;
ThreadMqttListen::ThreadMqttListen(MainWindow* mwin,  int index):_index(index),m(mwin)
{
    m_serial = m->serial;

}


ThreadMqttListen::~ThreadMqttListen()
{
    qDebug() << "deleting ThreadMqttListen";



}









void ThreadMqttListen::ExecUpFileFunc(QString topicEnd, QStringList MesgHead, QStringList Payload, ENUM_TopicType cmd)
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
            if(m->UpFileInfo->FileSize != Payload.at(1) || m->UpFileInfo->CreateTime != Payload.at(2) || m->UpFileInfo->ModifyTime != Payload.at(3))
            {
                m->UpFileInfo->FileName = Payload.at(0);   //file name
                m->UpFileInfo->FileSize = Payload.at(1);    //file size
//                m->UpFileInfo->CreateTime = Payload.at(2);    //file create time
//                m->UpFileInfo->ModifyTime = Payload.at(3);    //file modify time
                m->UpFileInfo->CreateTime = "2020-09-20 17:14:39";    //file create time
                m->UpFileInfo->ModifyTime = "2020-09-20 17:14:39";    //file modify time


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
                m->UpFileInfo->FileName = Payload.at(0);   //file name
                m->UpFileInfo->FrameEachSize = Payload.at(1).toInt();
                m->UpFileInfo->FrameEndSize = Payload.at(2).toInt();
                m->UpFileInfo->FrameAllSize = Payload.at(3).toInt();
                m->UpFileInfo->FrameCount = Payload.at(4).toInt();
                m->UpFileInfo->FrameNo = 0;

                isOK = "OK";

                reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + isOK + Fr_DDIV;

             }
            else
            {
                isOK = "ERR";
                reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + isOK + Fr_DDIV + \
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

        if( Payload.at(0) == m->UpFileInfo->FileName)
        {
            if(m->UpFileInfo->FrameNo == Payload.at(1).toInt())
            {
                /**when in last frame, FrameNo begins from 0,so +1**/
//                qDebug() << "The FrameNo = " << m->UpFileInfo->FrameNo << ", GetNo = " << Payload.at(1).toInt() << "FrameCount = " << m->UpFileInfo->FrameCount;
                if((m->UpFileInfo->FrameNo + 1) == m->UpFileInfo->FrameCount /*&& Payload.at(2).size() == m->UpFileInfo->FrameEndSize*/)
                {
//                    qDebug() << "FrameNo = " << m->UpFileInfo->FrameNo << ", LastFrameSize = " << m->UpFileInfo->FrameEndSize;

                    /*check sum**/
                    QByteArray pdata;
                    pdata.reserve((int)m->FrameInfo.FrameEachSize);
                    if(1)
                    {
                        SerialWindow::StringToHex(Payload[2], pdata);
                        m->UpFileInfo->FileBuffer.append(pdata);
                        qInfo() << "Receiving [Last] Update Frame..., No = " << m->UpFileInfo->FrameNo;
                        m->UpFileInfo->FrameNo++;

                        /*Judge whether Filesize is right**/
//                        qDebug() << "FileBuffer.size() = " << m->UpFileInfo->FileBuffer.size();
//                        qDebug() << "m->UpFileInfo->FileSize = " << m->UpFileInfo->FileSize;

                        if(m->UpFileInfo->FileBuffer.size() == m->UpFileInfo->FileSize.toInt())
                        {
                            /**write to file!**/
                            QFile file(m->UpFileInfo->FileName);
                            file.open(QIODevice::ReadWrite);
                            file.write(m->UpFileInfo->FileBuffer, m->UpFileInfo->FileBuffer.size());
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
                        reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;

                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                    }
                    else  // crc err
                    {
                        isOK = "ERR";
                        ErrInfo = "CRC ERR";

                        reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + QString::number(m->UpFileInfo->FrameNo,10) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                        reFrame.reTopic = "Re" + topicEnd;
                        this->m->seq_Num++;
                        emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD
                    }


                    pdata.clear();
                    pdata.squeeze();  // clear buffer


                }
                else /**when not in last frame**/
                {
                    /*check sum**/
                    QByteArray pdata;
                    pdata.reserve((int)m->FrameInfo.FrameEachSize);   //resize to FrameEachSize

                 //   Payload.at(4).toUShort() = checksum_16(pdata.data(),m->UpFileInfo->FrameEachSize);

                    if(1)
                    {
                        SerialWindow::StringToHex(Payload[2], pdata);
                        m->UpFileInfo->FileBuffer.append(pdata);
//                        qDebug() << "the Filebuffer:****" << m->UpFileInfo->FileBuffer.size();
                        qInfo() << "Receiving Update Frame..., No = " << m->UpFileInfo->FrameNo;
                        m->UpFileInfo->FrameNo++;
                        isOK = "OK";
                        ErrInfo = "";


                    }
                    else  // crc err
                    {

                        isOK = "ERR";
                        ErrInfo = "Real FrameNo = " + QString::number(m->UpFileInfo->FrameNo,10);


                    }

                    pdata.clear();
                    pdata.squeeze();  // clear buffer

                    reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + Payload.at(1) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                    reFrame.reTopic = "Re" + topicEnd;
                    this->m->seq_Num++;
                    emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD

                }
            }
            else   // if the first frame is not sync
            {
                isOK = "ERR";
                ErrInfo = "Real FrameNo = " + QString::number(m->UpFileInfo->FrameNo,10);
                this->m->seq_Num++;
                reFrame.rePayload = m->UpFileInfo->FileName + Fr_DDIV + Payload.at(1) + Fr_DDIV + isOK + Fr_DDIV + ErrInfo;
                reFrame.reTopic = "Re" + topicEnd;
                emit mesgResponseSignal(this->m->seq_Num, reFrame.reTopic, reFrame.rePayload);    // send RECMD

            }

        }
    }








}







void ThreadMqttListen::ExecHandFunc(QString topicEnd, QStringList MesgHead, QStringList Payload, ENUM_TopicType cmd)
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





void ThreadMqttListen::ExecCmdFunc(QString topicEnd, QStringList MesgHead, QStringList Payload)
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
             m_serial->dev_ctrl(Payload.at(i).toULong(), MOpen);
             QThread::msleep(50);    // control , sleep for 50ms;
        }

        QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
        /******update status********/

        for(int i = 1; i <= SwitchCount; i++)
        {
            /*** update device data***/
            auto it = m->KeyValFind(Payload.at(i).toULong());
            if(m->IsKeyValExist(it) < 0)
            {
                return ;   //return  if devid does not exists!
            }

            // update dev status.
            m_serial->read_run_status(Payload.at(i).toULong(),&it.value()->currentData.runState);
            it.value()->DataInfoHandle();

            if(it.value()->InfoData.IsOpenState == 1)   // open
            {
                isOK = "OK";   //  open switch successful!

            }
            else {
                isOK = "ERR";  //  open switch err!
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
            m_serial->dev_ctrl(Payload.at(i).toULong(), MClose);
            QThread::msleep(50);

        }

        QThread::msleep(500);   // need sometimes to wait at least 500ms wait to update switch state, when control one swith, 500ms is needed
        /******update status********/
        for(int i = 1; i <= SwitchCount; i++)
        {
            /*** update device data***/

            auto it = m->KeyValFind(Payload.at(i).toULong());
            if(m->IsKeyValExist(it) < 0)
            {
                return ;   //return  if devid does not exists!
            }

            // update dev status.
            m_serial->read_run_status(Payload.at(i).toULong(),&it.value()->currentData.runState);
            it.value()->DataInfoHandle();
            if(it.value()->InfoData.IsOpenState == 0)   // close
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
   // MainWindow::ui->->editLog->insertPlainText(content);   //put content to editlog

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
    MainWindow::tTopicHead tmptopic = m->Para_Get_ProductInfo();
    if(tmptopic.ID != TopicFrame[0] || tmptopic.AREA != TopicFrame[1] || tmptopic.NUM != TopicFrame[2])
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


    QMap<uint8_t, infoMCCB*>::iterator it;
    /**if we get the message, the topic format is right!***/


    switch(TopicType.indexOf(TopicFrame[3]))
    {
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
       //     ExecClearFunc(TopicFrame[3], MessageHead, Payload);
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

        break;

        case ReadBVal:
        break;

        case SetBEn:
        break;

        case ReadBEn:
        break;

        case SetBCor:
        break;

        case ReadBCor:
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
