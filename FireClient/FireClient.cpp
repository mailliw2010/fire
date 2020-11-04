//#pragma execution_character_set(“utf-8”)
#include "FireClient.h"


#include "ui_FireForm.h"
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <math.h>
#include <iostream>
#include <QMessageBox>
#include <QFile>
#include <QTcpSocket>
#include <QTextCodec>
#include <qstring.h>
#include <QMap>
#include <QThread>
#include "FireClient.h"
#include "globalFun.h"
#include "mqttpublic.h"
#if defined(_WIN32)
#else
#include <sys/time.h>
#endif
#include <iostream>





FireForm::FireForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FireForm)
  
{
 //   this->setParent(parent);
    ui->setupUi(this);  // initial ui interface. includeing create lineEdit
    this->setFixedSize(1480, 615);
    this->ui->lineEditTopic->setText(Topic_Head + "#");   // subscribe topic
    this->ui->lineEditTopic_2->setText(Topic_Head + "Cmd"); // publish topic
    this->setWindowTitle("Mqtt Client");


    /****************Initial Mqtt Remote Client**************************/

   m_client = new QMqttClient(this);
   m_client->setClientId(REMOTE_CLIENT_ID);
   m_client->setHostname(ui->comboBoxHost->currentText());   //set client host name; QString
   m_client->setPort((quint16)ui->spinBoxPort->value());       //set port; int






   connect(ui->comboBoxHost, &QComboBox::currentTextChanged, m_client, &QMqttClient::setHostname);  // put hostname to object m_client;
   connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), this, &FireForm::setClientPort); // put hostname to object m_client;
   connect(m_client, &QMqttClient::stateChanged, this, &FireForm::updateLogStateChange);  //signal: stateChanged, slot: updateLogStateChange
   connect(m_client, &QMqttClient::disconnected, this, &FireForm::brokerDisconnected);    // disconnect
                                                        // lambda function [anoymous function]  capture 'this' by value.
   // i cant see emit messageReceived(xx), how to emit the signal??? in define cpp file  "emit m_clientPrivate->m_client->messageReceived(message, topic)";

           //data transfer between diferent thread, must have 5th parameter:QueuedConnection!!!!

//   connect(m_client, &QMqttClient::pingResponseReceived, this, [this]() {     // no parameter
//       const QString content = QDateTime::currentDateTime().toString()
//                   + QLatin1String(" PingResponse")
//                   + QLatin1Char('\n');
//       ui->editLog->insertPlainText(content);
//   });


    connect(m_client, &QMqttClient::messageReceived, this, &FireForm::RecvHandle,Qt::QueuedConnection);
   /****************End of Initial Mqtt Remote Client**************************/

    ui->editLog->setMaximumBlockCount(65500);  // set max count ;
    ui->editLog->setReadOnly(true);
  //  ui->editLog->selectionChanged();






}

FireForm::~FireForm()
{




}



FireClient::FireClient()
{

    this->setClientId(REMOTE_CLIENT_ID);
    this->setHostname("183.66.138.122");   //set client host name; QString
    this->setPort(1883);       //set port; int
    this->setUsername("admin");
    this->setPassword("public");
    this->connectToHost();

}


void FireForm::RecvHandle(const QByteArray &message, const QMqttTopicName &topic)
{


#if 1

    /**this is a useful tool!**/

    /***Remove Fr_Head and Fr_End ******/
    QString Message(message.chopped(1));    //byte array to string, remove Fr_Head by chopped(1);


    /***Topic Transfer******/
    QStringList TopicContent = topic.name().trimmed().split("/");
    if(TopicContent.count() != 4)
    {
        qDebug() << "The Topic ERR:" << topic.name();      return;
    }



    /***Message Transfer******/
    QStringList MessageContent = Message.trimmed().split(Fr_RDIV);
    if(MessageContent.count() != 2)
    {
        qDebug() << "The Message ERR:" << Message;
        return;
    }

    QStringList HeadList = MessageContent[0].trimmed().split(Fr_DDIV);
    QString HeadStr = HeadList.join("|");
    QStringList PayloadList = MessageContent[1].trimmed().split(Fr_DDIV);
    QString PayloadStr = PayloadList.join("|");


    const QString content = "TOPIC:" + TopicContent[3] + " @ Header:" + HeadStr + \
            " @ Payload:" + PayloadStr + "\n";


    if(ui->editLog->blockCount() == 65490)
    {
        ui->editLog->clear();
    }
    QTextCursor cursor = ui->editLog->textCursor();
    cursor.movePosition(QTextCursor::End);


    ui->editLog->setTextCursor(cursor);
    ui->editLog->insertPlainText(content);   //put content to editlog


    QList<QTextEdit::ExtraSelection> extraSelections;//提供一种方式显示选择的文本
    extraSelections = ui->editLog->extraSelections();//获取之前高亮的设置
    QTextEdit::ExtraSelection selection;

    if(TopicContent[3].indexOf("Re") == 0)
    {
        selection.format.setBackground(Qt::white);   // set Gray

    }
    else {
        selection.format.setBackground(Qt::lightGray);   // set Gray
    }





    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = ui->editLog->textCursor();
    selection.cursor.movePosition(QTextCursor::Up);//光标移动到某一行。此处移动到上一行，上一行将高亮。
    extraSelections.append(selection);
    ui->editLog->setExtraSelections(extraSelections);

#endif
}



void FireForm::on_buttonPublish_clicked()
{
    /**this is a useful tool!**/
    static int TestSeqNum = 0;
    QStringList PayloadList = this->ui->lineEditMessage->text().trimmed().split("|");
    QString Payload = PayloadList.join(Fr_DDIV);
    QString MessageHead = QString::number(TestSeqNum++, 10) + Fr_DDIV + m_client->clientId() +\
           Fr_DDIV + "TXT" + Fr_DDIV + QString::number(PayloadList.count(), 10);

    QString MessageOut = Fr_Head + MessageHead + Fr_RDIV +Payload + Fr_End;

    ExecPublish(m_client, this->ui->lineEditTopic_2->text(), MessageOut.toUtf8());
   //d emit publishSignal(m_client, this->ui->lineEditTopic_2->text(), MessageOut.toUtf8());

}

void FireForm::on_buttonConnect_clicked()
{
    ExecConnect(m_client);
}

void FireForm::on_buttonSubscribe_clicked()
{

    ExecSubscribe();
}

void FireForm::on_buttonUnsubscribe_clicked()
{
   ExecUnSubscribe();
}




void FireForm::ExecPublish(QMqttClient *client, const QString &topic, const QByteArray &message)
{
    qDebug() << "ExecPublish Start!";
    if(client->state() == QMqttClient::Connected)   // publish only connect!
    {


        if (client->publish(topic, message) == -1)   //why 1st parameter is ok? different type
        {
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
            qDebug() << client->publish(topic, message);

        }



    }
    else
    {
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not Publish. Is there a valid connection?"));
            return;

    }









}



void FireForm::ExecConnect(QMqttClient *client)
{
    /********Set mqtt***************/
    qDebug() <<  "ExecConnect Start!";
    if(client->clientId() == REMOTE_CLIENT_ID)
    {

        if (client->state() == QMqttClient::Disconnected) {    //initial is disconnect;
            qDebug() <<  "connecting remote server...!";
            ui->comboBoxHost->setEnabled(false);                 // set disable
            ui->spinBoxPort->setEnabled(false);
            client->setUsername("admin");
            client->setPassword("public");

            client->connectToHost();  //connect to host(server)
            ui->buttonConnect->setText(tr("Disconnect"));
        } else {
            qDebug() <<  "disconnecting remote server...!";
            ui->comboBoxHost->setEnabled(true);
            ui->spinBoxPort->setEnabled(true);
            client->disconnectFromHost();
            ui->buttonConnect->setText("Connect");
        }



    }



}
void FireForm::ExecSubscribe()
{
    qDebug() << "ExecSubscribe Start!";
    static int flag = 0;
    auto subscription = m_client->subscribe(ui->lineEditTopic->text());    //subscribe one topic.
    if (!subscription) {   // if subscription is nullptr, means subscribe error.
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe client. Is there a valid connection?"));
        return;

    }







}


void FireForm::ExecUnSubscribe()
{
    qDebug() << "ExecUnSubscribe Start!";
    m_client->unsubscribe(ui->lineEditTopic->text());    //unsubscribe one topic.no return


}



void FireForm::brokerDisconnected()
{
    ui->comboBoxHost->setEnabled(true);
    ui->spinBoxPort->setEnabled(true);
    ui->buttonConnect->setText(tr("Connect"));
}

void FireForm::setClientPort(int p)
{
    m_client->setPort((quint16)p);

}


void FireForm::updateLogStateChange()
{
    // set contant type
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    ui->editLog->insertPlainText(content);  //show contents
}



void FireForm::on_editLog_selectionChanged()
{

}

void FireForm::on_radioButton_clicked()
{
    ui->lineEditTopic_2->setText(Topic_Head + "Cmd");
    ui->lineEditMessage->setText("ON|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18");
}

void FireForm::on_radioButton_2_clicked()
{
    ui->lineEditTopic_2->setText(Topic_Head + "Cmd");
    ui->lineEditMessage->setText("OFF|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18");
}

void FireForm::on_radioButton_3_clicked()
{
    ui->lineEditTopic_2->setText(Topic_Head + "Clear");
    ui->lineEditMessage->setText("ENE|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18");
}

void FireForm::on_radioButton_4_clicked()
{
    ui->lineEditTopic_2->setText(Topic_Head + "Clear");
    ui->lineEditMessage->setText("FAUL|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18");
}

void FireForm::on_buttonClear_clicked()
{
    ui->editLog->clear();
}
