﻿//#pragma execution_character_set(“utf-8”)
#include "gsvr_app_config.h"


#include "ui_serialwindow.h"
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <math.h>
#include <iostream>
#include <QMessageBox>
#include <QFile>
#include <QTcpSocket>
#include <QTextCodec>
#include <qstring.h>
#include <QMap>
#include "gsvr_app_server.h"
#include "globalFun.h"
#include "gsvr_thread_listen.h"
#include "globalFun.h"
#include "dlt645/dlt645-private.h"
#include "dlt645/dlt645-rtu-private.h"

#if defined(_WIN32)
// windows time
#else
#include <sys/time.h>
#endif
#include <iostream>


bool SerialWindow::flag = false;


QString SerialWindow::emi_sendData = "";
int SerialWindow::SendNum = 0;
int SerialWindow::ReceiveNum = 0;




SerialWindow::SerialWindow(MainWindow* mwin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialWindow),
    m(mwin)
{

    /**Config From Config File**/
    ui->setupUi(this);  // initial ui interface. includeing create lineEdit



    m_Client.local = m->getMqttClientLocal();
    m_Client.remote = m->getMqttClientRemote();

    m_ConfigFile = QCoreApplication::applicationDirPath() + "/Config.ini";
  // SaveToConfigFile(); //after m_ConfSet construct;
    qDebug() << "Initialing ConfigFile~~~~~~:" << m_ConfigFile;

    ConfigFromFile();    // config



    ui->editLog->setMaximumBlockCount(65500);  // set max count ;
  //  setFixedSize(1280,500);   //固定窗口大小
    this->ui->lineEditTopic->setText(_pubvar::Topic_Head + "#");



   /**Only for tempory use!*/
//    infoModel *devinfo21 = new infoModel;
//    devMccb->insert(0x21, devinfo21);
//    infoModel *devinfo22 = new infoModel;
//    devMccb->insert(0x22, devinfo22);
//    infoModel *devinfo23 = new infoModel;
//    devMccb->insert(0x23, devinfo23);

//    auto it = devMccb->find(0x23);
//    qDebug() << "devinfo23 = " <<devinfo23;
//    qDebug() << "it.value(23) = " <<it.value();


    m_serialPort = new QSerialPort(this);
//    ui->pushButton_comopen->setText("关闭");   //set state open
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readMyCom()));
    timer->start(10);  //send emit timeout() every 10ms.

    QRegExp rx("[A-Fa-f,0-9]*");
    //setWindowIcon(":/resource/resource/img/desktop.ico");
    /* Set Validator forlineEdit_emi_write*/
    ui->lineEdit_emi_write->setPlaceholderText("Please Input Hex Data,Even");
    QRegExp rx2("[A-Fa-f,0-9]*");
    pReg2 = new QRegExpValidator(rx2, ui->lineEdit_emi_write);
    ui->lineEdit_emi_write->setValidator(pReg2);

    /* Set Validator for ComBo Box */
    QRegExp rx3("[A-Fa-f,0-9]*");
    pReg3 = new QRegExpValidator(rx3, ui->comboBox_emi_write);
    ui->comboBox_emi_write->setValidator(pReg3);
    ui->radioButton_emi->setCheckable(true);
    ui->radioButton_emi->setChecked(false);    //initial
    ui->radioButton_emi_2->setChecked(true);


    QFile file_data("send_data.txt");
   if (file_data.open(QIODevice::ReadOnly |QIODevice::Unbuffered | QIODevice::Text ))    // write only mode, can clear automaticlly.
   {
      QTextStream stream(&file_data);
      stream.setCodec("GBK");   //support chinese
      QString line;
 //     stream.seek(0);    // append to cover the file
      while(!stream.atEnd())
      {
          line =  stream.readLine();
           line.remove('\n');
           ui->comboBox_emi_write->addItem(line);

      }
      file_data.close();
   }

   /************************cycle to find available ports**********/
    foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "portName   :" << Info.portName();// enable Debug infor
        qDebug() << "Description    :" << Info.description();
        qDebug() << "Manufacture:" << Info.manufacturer();

        QSerialPort serial;
        serial.setPort(Info);

        if(serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_portName->addItem(Info.portName());
            serial.close();  // close the port

        }

    }

}


void SerialWindow::closeEvent(QCloseEvent *event)
{

    m->grabKeyboard();   // get focus on mainwindow, so ctrl + c can be used
}
void SerialWindow::UseDefaultConfig()
{

    /**Uart Config**/
    auto DeviceName =  ui->comboBox_portName->currentText();
    auto BaurdRate =  ui->comboBox_buartRate->currentText().toInt();
    auto DataBit = ui->lineEdit_ModDataBit->text().toInt();
    auto StopBit = ui->lineEdit_ModStopBit->text().toInt();
    this->m->Para_Set_SerialP(DeviceName, BaurdRate, DataBit, StopBit);
    ResponseTimeOut =  ui->lineEdit_TimeOut->text().toInt();


    /**Public Mqtt Config**/
    auto ProductID = ui->lineEdit_ProductID->text();
    auto ProductAREA = ui->lineEdit_ProductAREA->text();
    auto ProductNUM = ui->lineEdit_ProductNUM->text();
    this->m->Para_Set_ProductInfo(ProductID, ProductAREA, ProductNUM);

    auto ExtProductID =  ui->lineEdit_ExtProductID->text();
    auto ExtProductAREA = ui->lineEdit_ExtProductAREA->text();
    auto ExtProductNUM = ui->lineEdit_ExtProductNUM->text();
    this->m->Para_Set_ExtProductInfo(ExtProductID, ExtProductAREA, ExtProductNUM);


    auto devstart = ui->lineEdit_DevStart->text().toInt();
    auto devend = ui->lineEdit_DevEnd->text().toInt();
    this->m->Para_Set_DeviceIDSerial(devstart, devend);


    auto FrameEach =  ui->lineEdit_FrameEach->text().toInt();
    auto TimeOut = ui->lineEdit_TimeOut->text().toInt();
    auto ResendNum = ui->lineEdit_PeriodTime->text().toInt();
    auto UpInfoPeriod_S = ui->lineEdit_ResendNum->text().toInt();
    this->m->Para_Set_FrameAll(FrameEach, TimeOut, ResendNum, UpInfoPeriod_S);

    /**Local Mqtt Config**/
    auto LocalHost = ui->lineEditHost_local->text();
    auto LocalPort = ui->spinBoxPort_local->value();
    auto LocalPass = ui->lineEdit_LocalPass->text();
    auto LocalUser = ui->lineEdit_LocalUserN->text();
    this->m->Para_Set_ClientLocalInfo(LocalPort, LocalHost, LocalUser, LocalPass);



    /**Remote Mqtt Config**/
    auto RemHost = ui->lineEditHost->text();
    auto RemPort = ui->spinBoxPort_Remote->value();
    auto RemUser = ui->lineEdit_RemoteUserN->text();
    auto RemPass = ui->lineEdit_RemotePass->text();
    this->m->Para_Set_ClientRemoteInfo(RemPort, RemHost, RemUser, RemPass);



    /**Sys Config**/
    if(ui->DebugMode->isChecked() == false)
    {
        m->Para_Set_Mode(0);  //Release Mode

    }
    else {
        m->Para_Set_Mode(1);  //Debug Mode
    }
}

void SerialWindow::ConfigFromFile()
{
    qDebug() << "Start config from file";
    QFileInfo file(m_ConfigFile);
    if(file.exists() == false){
        qWarning() << "Config file loss!!! Use Default Setting...";
        UseDefaultConfig();
        return;

//        QTimer::singleShot(0, qApp, SLOT(quit()));   // use this to exit

    }
    m_ConfSet = new QSettings(m_ConfigFile, QSettings::IniFormat);   // new a QSetting
    /**Uart Config**/

    auto DeviceName = GetFromInitFile("Uart", "DevName").toString();
    auto BaurdRate = GetFromInitFile("Uart", "Baurd").toInt();
    auto DataBit = GetFromInitFile("Uart", "DataBit").toInt();
    auto StopBit = GetFromInitFile("Uart", "StopBit").toInt();
    this->m->Para_Set_SerialP(DeviceName, BaurdRate, DataBit, StopBit);
    ResponseTimeOut = GetFromInitFile("Modbus", "ResponseTimeOut_MS").toInt();


    /**Public Mqtt Config**/

    auto ProductID = GetFromInitFile("MqttPublic", "ProductID").toString();

    auto ProductAREA = GetFromInitFile("MqttPublic", "ProductAREA").toString();
    auto ProductNUM = GetFromInitFile("MqttPublic", "ProductNUM").toString();
    this->m->Para_Set_ProductInfo(ProductID, ProductAREA, ProductNUM);

    auto ExtProductID = GetFromInitFile("MqttPublic", "ExtProductID").toString();
    auto ExtProductAREA = GetFromInitFile("MqttPublic", "ExtProductAREA").toString();
    auto ExtProductNUM = GetFromInitFile("MqttPublic", "ExtProductNUM").toString();
    this->m->Para_Set_ExtProductInfo(ExtProductID, ExtProductAREA, ExtProductNUM);


    auto devstart = GetFromInitFile("MqttPublic", "DeviceIDStart").toUInt();
    auto devend = GetFromInitFile("MqttPublic", "DeviceIDEnd").toUInt();
    this->m->Para_Set_DeviceIDSerial(devstart, devend);

    auto FrameEach = GetFromInitFile("MqttPublic", "FrameEach").toUInt();
    auto TimeOut = GetFromInitFile("MqttPublic", "TimeOut").toUInt();
    auto ResendNum = GetFromInitFile("MqttPublic", "ResendNum").toUInt();
    auto UpInfoPeriod_S = GetFromInitFile("MqttPublic", "UpInfoPeriod_S").toUInt();
    this->m->Para_Set_FrameAll(FrameEach, TimeOut, ResendNum, UpInfoPeriod_S);


    /**Local Mqtt Config**/
    auto LocalHost = GetFromInitFile("Mqtt-Local", "Host").toString();
    auto LocalPort = GetFromInitFile("Mqtt-Local", "Port").toUInt();
    qDebug( ) << "local portttttt " << LocalPort;
    auto LocalPass = GetFromInitFile("Mqtt-Local", "PassWord").toString();
    auto LocalUser = GetFromInitFile("Mqtt-Local", "User").toString();
    this->m->Para_Set_ClientLocalInfo(LocalPort, LocalHost, LocalUser, LocalPass);

    /**Remote Mqtt Config**/
    auto RemHost = GetFromInitFile("Mqtt-Remote", "Host").toString();
    auto RemPort = GetFromInitFile("Mqtt-Remote", "Port").toUInt();
    qDebug( ) << "RemPort portttttt " << RemPort;
    auto RemPass = GetFromInitFile("Mqtt-Remote", "PassWord").toString();
    auto RemUser = GetFromInitFile("Mqtt-Remote", "User").toString();
    this->m->Para_Set_ClientRemoteInfo(RemPort, RemHost, RemUser, RemPass);

    /**Sys Config*/
    if(GetFromInitFile("Mode", "DebugMode").toInt())
    {
        m->Para_Set_Mode(1);

    }
    else {
        m->Para_Set_Mode(0);
    }


    delete m_ConfSet;

    /**copy para to config ui**/
    CopyParaToUi();
  //



//#if defined (_WIN32)   // windows
//    QString DevName = "COM6";
//#elif defined (__x86_64)  // x86 gcc
//    QString DevName = "/dev/ttyUSB0";
//#else  // _ARM_LINUX_    // arm-linux-gcc
//    QString DevName = "/dev/ttymxc0";
//#endif




}


void SerialWindow::CopyParaToUi()
{
    /**Uart Config**/
    MainWindow::tserialpara tmpserial = m->Para_Get_SerialP();
    ui->comboBox_portName->setCurrentText(tmpserial.name);
    ui->comboBox_buartRate->setCurrentText(QString::number(tmpserial.baurd));
    ui->lineEdit_ModDataBit->setText(QString::number(tmpserial.databit));
    ui->lineEdit_ModStopBit->setText(QString::number(tmpserial.stopbit));
    ui->lineEdit_TimeOut->setText(QString::number(ResponseTimeOut));

    /**Public Mqtt Config**/

    MainWindow::tTopicHead topic = m->Para_Get_ProductInfo();
    ui->lineEdit_ProductID->setText(topic.ID);
    ui->lineEdit_ProductAREA->setText(topic.AREA);
    ui->lineEdit_ProductNUM->setText(topic.NUM);

    MainWindow::tTopicHead exttopic = m->Para_Get_ExtProductInfo();
    ui->lineEdit_ExtProductID->setText(exttopic.ID);
    ui->lineEdit_ExtProductAREA->setText(exttopic.AREA);
    ui->lineEdit_ExtProductNUM->setText(exttopic.NUM);

    MainWindow::tDevSerialNum dev = m->Para_Get_DeviceIDSerial();
    ui->lineEdit_DevStart->setText(QString::number(dev.devStart));
    ui->lineEdit_DevEnd->setText(QString::number(dev.devEnd));


    ui->lineEdit_FrameEach->setText(QString::number(m->Para_Get_FrameEachSize()));
    ui->lineEdit_TimeOut->setText(QString::number(m->Para_Get_FrameOverTime()));
    ui->lineEdit_ResendNum->setText(QString::number(m->Para_Get_FrameReSendCount()));
    ui->lineEdit_PeriodTime->setText(QString::number(m->Para_Get_FrameAutoUploadTime()));



    /**Local Mqtt Config**/
    MainWindow::tclientInfo cllocal = m->Para_Get_ClientLocalInfo();
    ui->lineEditHost_local->setText(cllocal.host);
    ui->spinBoxPort_local->setValue(cllocal.port);
    ui->lineEdit_LocalPass->setText(cllocal.pass);
    ui->lineEdit_LocalUserN->setText(cllocal.user);



    /**Remote Mqtt Config**/
    MainWindow::tclientInfo clremo = m->Para_Get_ClientRemoteInfo();
    ui->lineEditHost->setText(clremo.host);
    ui->spinBoxPort_Remote->setValue(clremo.port);
    ui->lineEdit_RemotePass->setText(clremo.pass);
    ui->lineEdit_RemoteUserN->setText(clremo.user);

    /**Sys Config**/
    if(m->Para_Get_Mode())
    {
        ui->DebugMode->setChecked(true);

    }
    else{
        ui->DebugMode->setChecked(false);
    }




}

void SerialWindow::SaveToConfigFile()
{
    m_ConfSet = new QSettings(m_ConfigFile, QSettings::IniFormat);   // new a QSetting
    /**Uart Config**/
    Set2IniFile("Uart", "DevName", ui->comboBox_portName->currentText());
    Set2IniFile("Uart", "Baurd", ui->comboBox_buartRate->currentText().toInt());
    Set2IniFile("Uart", "DataBit", ui->lineEdit_ModDataBit->text().toInt());
    Set2IniFile("Uart", "StopBit", ui->lineEdit_ModStopBit->text().toInt());
    Set2IniFile("Modbus", "ResponseTimeOut_MS", ui->lineEdit_Modtimeout->text().toInt());


    /**Public Mqtt Config**/
    Set2IniFile("MqttPublic", "ProductID", ui->lineEdit_ProductID->text());
    Set2IniFile("MqttPublic", "ProductAREA", ui->lineEdit_ProductAREA->text());
    Set2IniFile("MqttPublic", "ProductNUM", ui->lineEdit_ProductNUM->text());
    Set2IniFile("MqttPublic", "ExtProductID", ui->lineEdit_ExtProductID->text());
    Set2IniFile("MqttPublic", "ExtProductAREA", ui->lineEdit_ExtProductAREA->text());
    Set2IniFile("MqttPublic", "ExtProductNUM", ui->lineEdit_ExtProductNUM->text());
    Set2IniFile("MqttPublic", "DeviceIDStart", ui->lineEdit_DevStart->text().toInt());
    Set2IniFile("MqttPublic", "DeviceIDEnd", ui->lineEdit_DevEnd->text().toInt());


//    Set2IniFile("MqttPublic", "FrameEach", ui->lineEdit_FrameEach->text().toInt());
//    Set2IniFile("MqttPublic", "TimeOut", ui->lineEdit_TimeOut->text().toInt());
//    Set2IniFile("MqttPublic", "UpInfoPeriod_S", ui->lineEdit_PeriodTime->text().toInt());
//    Set2IniFile("MqttPublic", "ResendNum", ui->lineEdit_ResendNum->text().toInt());

    int flag  = 0;

    if(ui->lineEdit_FrameEach->text().toInt() >= 1 && ui->lineEdit_FrameEach->text().toInt() <= 204800000)
    {
        Set2IniFile("MqttPublic", "FrameEach", ui->lineEdit_FrameEach->text().toInt());
    }
    else {
        flag |= 1<<0;
    }

    if(ui->lineEdit_TimeOut->text().toInt() >= 1 && ui->lineEdit_TimeOut->text().toInt() <= 7200)
    {
        Set2IniFile("MqttPublic", "TimeOut", ui->lineEdit_TimeOut->text().toInt());
    }
    else {
          flag |= 1<<1;
    }

    if(ui->lineEdit_PeriodTime->text().toInt() >= 1 && ui->lineEdit_PeriodTime->text().toInt() <= 50)
    {
        Set2IniFile("MqttPublic", "UpInfoPeriod_S", ui->lineEdit_PeriodTime->text().toInt());

    }
    else {

         flag |= 1<<2;
    }


    if(ui->lineEdit_ResendNum->text().toInt() >= 1 && ui->lineEdit_ResendNum->text().toInt() <= 86400)   // 1s - 1 day
    {
        Set2IniFile("MqttPublic", "ResendNum", ui->lineEdit_ResendNum->text().toInt());

    }
    else {
          flag |= 1<<3;
    }

    if((flag & 0x01) != 0)
    {
        qDebug() << "Set para out of range:frameEachSize";

    }
    if((flag & 0x02) != 0)
    {
        qDebug() << "Set para out of range:TimeOut";
    }
    if((flag & 0x04) != 0)
    {
        qDebug() << "Set para out of range:PeriodT";
    }
    if((flag & 0x08) != 0)
    {
        qDebug() << "Set para out of range:ReSendCount";

    }











    /**Local Mqtt Config**/
    Set2IniFile("Mqtt-Local", "Host", ui->lineEditHost_local->text());
    Set2IniFile("Mqtt-Local", "Port", ui->spinBoxPort_local->value());
    Set2IniFile("Mqtt-Local", "User", "");
    Set2IniFile("Mqtt-Local", "PassWord", "");
//    Set2IniFile("Mqtt-Local", "ReConnectTimsp_S", ui->lineEdit_ResendNum->text());

    /**Remote Mqtt Config**/
    Set2IniFile("Mqtt-Remote", "Host", ui->lineEditHost->text());
    Set2IniFile("Mqtt-Remote", "Port", ui->spinBoxPort_Remote->value());
    Set2IniFile("Mqtt-Remote", "User", ui->lineEdit_RemoteUserN->text());
    Set2IniFile("Mqtt-Remote", "PassWord", ui->lineEdit_RemotePass->text());
//    Set2IniFile("Mqtt-Remote", "ReConnectTimsp_S", 60);


    /**Sys Config**/
    if(ui->DebugMode->isChecked() == false)
    {
        Set2IniFile("Mode", "DebugMode", 0);  //Release Mode

    }
    else {
        Set2IniFile("Mode", "DebugMode", 1);  //Debug Mode
    }

    delete m_ConfSet;   // delete this to save to file

}




void SerialWindow::Set2IniFile(QString nodeName, QString keyName, QVariant valueName)
{

    m_ConfSet->setValue(QString("/%1/%2").arg(nodeName).arg(keyName), valueName);



}


QVariant SerialWindow::GetFromInitFile(QString nodeName,QString keyName)
{
    QVariant qvar = m_ConfSet->value(QString("/%1/%2").arg(nodeName).arg(keyName));
    return qvar;
}

SerialWindow::~SerialWindow()
{

    delete ui;
    ui = nullptr;
    qDebug() << "deleting the serialwindow ui";




}


void SerialWindow::on_pushButton_comopen_clicked()
{


    emit serialopenSinal();    //setting

    qDebug() << "ExecSerialOpen Start!";
    /********Set Serial***************/
    m_serialPort->setPortName(ui->comboBox_portName->currentText());
   // m_serialPort->setBaudRate(9600);
    m_serialPort->setBaudRate(ui->comboBox_buartRate->currentText().toInt());
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort->setStopBits(QSerialPort::OneStop);


   if(SerialWindow::flag == false)
   {
       if(!m_serialPort->open(QIODevice::ReadWrite))
       {

          QMessageBox msgBox;
          msgBox.information(this, "Err!","Port Can Not Open!!");
          return;

       }
       /***when open success***/
       qDebug() << "open success!";
       SerialWindow::flag = !SerialWindow::flag;   //when open success, flag = true;
//       ui->pushButton_comopen->setText("Close");
    }
    else
    {

        m_serialPort->close();
        SerialWindow::flag = !SerialWindow::flag;
//        ui->pushButton_comopen->setText("Open");
        delete timer;


    }

}



void SerialWindow::readMyCom()
{


   if(requestData != "")
   {
   //    qDebug() << "the readAll()is    :" << requestData;
       QDateTime currTime = QDateTime::currentDateTime();
       QString strDate = "[" + currTime.toString("hh:mm:ss") + "]:";
       qDebug() << strDate;
       ui->textEdit_read->append(strDate);
       ui->textEdit_read->append(requestData.toHex());
       ReceiveNum++;
      ui->pushButton_recvNum->setText(QString::number(ReceiveNum));
      requestData.clear();

   }


}


/* */
void SerialWindow::autoSubscribe()
{




}

/* I try add '+' in string, can it be handle correctly?*/
void SerialWindow::StringToHex(QString str, QByteArray &senddata)  //StringToHex(source,desti);
{
    int high_hexbit, low_hexbit;
    int hexdatalen = 0;
    int len = str.length();  //len must be even;
    if (len%2 != 0)   //size is odd
    {
        senddata.resize((len+1)/2);
  //      qDebug()<<"the odd size is "<<(len+1)/2;
    }
    else
     {
        senddata.resize(len/2);  // create the size of senddata
 //        qDebug()<<"the even size is "<<(len)/2;

    }


    char lstr,hstr;
    for(int i = 0; i < len; i++)
    {
        hstr = str[i].toLatin1();  // from 00-0xff
      //  qDebug() << hstr;
        if(hstr == ' ' || hstr == '+')   // we want to escape '+'
        {
            continue;
        }

        if(i >=len)
            break;

        lstr = str[++i].toLatin1();

        high_hexbit = ConvertHexChar(hstr);
        low_hexbit = ConvertHexChar(lstr);

        if((high_hexbit == 16) || (low_hexbit == 16))
                break;
        else
            high_hexbit = high_hexbit*16 + low_hexbit;

        senddata[hexdatalen] = (char)high_hexbit;

        hexdatalen++;
    }
    senddata.resize(hexdatalen);

}


char SerialWindow::ConvertHexChar(char ch)
{
    if((ch >='0') && (ch <= '9'))
        return ch - 0x30;
    else if((ch >= 'A') && (ch <= 'Z'))
        return ch -'A' + 10;
    else if((ch >= 'a') && (ch <= 'z'))
        return ch -'a' + 10;
    else
        return ch - ch;


}



void SerialWindow::on_pushButton_emi_write_clicked()
{
    uint8_t buffer[256] = "";
    QByteArray tmp;
    uint8_t *buff;
    int len = 0;
  //  if (flag == false)    //if open button is not clicked
    if (this->ctx->s < 0)
    {

        QMessageBox msgBox;
        msgBox.information(this, "Warning","Port Not Open!!");
        return;
    }


    if(ui->radioButton_emi->isChecked() == false)    // free send
    {
        if(ui->lineEdit_emi_write->text() == "")
        {
            qDebug() << "no data iniput";
            return;

        }

        emi_sendData = ui->lineEdit_emi_write->text().toUtf8().trimmed();  // source data
        ui->pushButton_sendNum->setText(QString::number(SendNum));

    }
    else   // group send
    {
        //if(ui->comboBox_emi_write->currentText().size() < 3)
        if(ui->comboBox_emi_write->currentText().at(0) == '#')    //判断#开头为cmd
        {
             qDebug() << "this is cmd\n";
             QMessageBox msgBox2;
              msgBox2.information(this, "Warning","Frame Format Error!!");
             return;
        }
        else
        {
            emi_sendData = ui->comboBox_emi_write->currentText().toUtf8();
            ui->pushButton_sendNum->setText(QString::number(SendNum));
        }
    }
    StringToHex(emi_sendData,tmp);    // Hex Data is saved to tmp
    qDebug() << "writing:" << emi_sendData;

    unsigned short crc = 0;
    char crc_L = 0;
    char crc_H = 0;
    //crc = CRC16_Modbus((unsigned char*)buff, len);
    crc = CRC16_Modbus((unsigned char*)tmp.data(), tmp.length());

    qDebug("crc = %04x\n", crc);


    crc_L = (char)(crc & 0xff);
    crc_H = (char)((crc >> 8) & 0xff);

    tmp.append(crc_L);
    tmp.append(crc_H);
    buff = (uint8_t *)tmp.data();
    len = tmp.length();



    /**Send Mesg from ctx***/
    this->ctx->backend->send(this->ctx, buff, len);


     SendNum++;     //calculate SendNumber


    qDebug() << "sending recv";


//    len = this->ctx->backend->receive(this->ctx, buffer);
     len = _dlt645_receive_msg(this->ctx, buffer, MSG_CONFIRMATION);
    qDebug() << "len = " << len;
//    for(int i = 0; i < )
//    printf("%02x ",buffer[i])
    if(len >= 0)
    {

        requestData.resize(len);

        memcpy(requestData.data(), buffer, len);
        qDebug() << "requestData = " << requestData;


    }





}



dlt645_t* SerialWindow::getserialObj(dlt645_t *tmpctx)
{
    ctx = tmpctx;
}


void SerialWindow::DeleteCurrentIndex()
{
    ui->comboBox_emi_write->removeItem(combo_index);
}







void SerialWindow::on_pushButton_clicked()
{

    qDebug() << "Reloading";
   ui->comboBox_emi_write->clear();   // clear all items
    QFile file_data("send_data.txt");
   //  QTextCodec *codec = QTextCodec::codecForName("GB2312");
   if (file_data.open(QIODevice::ReadOnly |QIODevice::Unbuffered | QIODevice::Text ))    // write only mode, can clear automaticlly.
   {
      QTextStream stream(&file_data);
      stream.setCodec("GBK");
      QString line;
 //     stream.seek(0);    // append to cover the file

      while(!stream.atEnd())
      {
          line =  stream.readLine();

           line.remove('\n');
           ui->comboBox_emi_write->addItem(line);


      }




      file_data.close();

   }

}



void SerialWindow::on_pushButton_sendNum_clicked()
{
    SendNum = 0;
    ui->pushButton_sendNum->setText(QString::number(SendNum));

}

void SerialWindow::on_pushButton_recvNum_clicked()
{
    ReceiveNum = 0;
    ui->pushButton_recvNum->setText(QString::number(ReceiveNum));

}






void SerialWindow::on_buttonPublish_clicked()
{
    /**this is a useful tool!**/
    static int TestSeqNum = 0;
    QStringList PayloadList = this->ui->lineEditMessage->text().trimmed().split("|");
    QString Payload = PayloadList.join(Fr_DDIV);
    QString MessageHead = QString::number(TestSeqNum++, 10) + Fr_DDIV + m_Client.remote->getclientID() +\
           Fr_DDIV + "TXT" + Fr_DDIV + QString::number(PayloadList.count(), 10);

    QString MessageOut = MessageHead + Fr_RDIV +Payload;
    qDebug() << MessageOut;


 //   emit publishSignal(this->ui->lineEditTopic_2->text(), MessageOut.toUtf8());

}

void SerialWindow::on_buttonConnect_clicked()
{
  //  emit connectSignal(m_Client->ClientRemote);

}

void SerialWindow::on_buttonSubscribe_clicked()
{
    emit subscribeSignal();
}

void SerialWindow::on_buttonUnsubscribe_clicked()
{
    emit unsubscribeSignal();
}

void SerialWindow::on_buttonConnect_local_clicked()
{
 //   emit connectSignal(m_Client->ClientLocal);
}

void SerialWindow::on_pushButton_emi_write_pressed()
{

}

void SerialWindow::on_buttonSave_clicked()
{
    QMessageBox msgBox;
    SaveToConfigFile();
    msgBox.information(this, "Info:","Save Config OK!", "Close");

}
