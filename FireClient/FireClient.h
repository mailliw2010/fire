#ifndef FireForm_H
#define FireForm_H

#include <QWidget>
#include <QTimer>
#include <QTcpSocket>
#include<QString>
#include <QMainWindow>
#include <QDialog>
#include <QtMqtt/QMqttClient>


namespace Ui {
class FireForm;
}

class FireClient:public QMqttClient     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
{
    Q_OBJECT
public:
    explicit FireClient();

};


class FireForm : public QMainWindow     // FireForm is paishenglei dialog, so it can be closed!
{
    Q_OBJECT

public:
    explicit FireForm(QWidget *parent = nullptr);  //gouzaohanshu
    ~FireForm();     //gouxihanshu
    void autoSubscribe();

    Ui::FireForm *ui;
    QMqttClient *m_client;         // remote client;
    FireClient *m_client2;

    void brokerDisconnected();
    void setClientPort(int p);

    void ExecConnect(QMqttClient *client);
    void ExecPublish(QMqttClient *client, const QString &topic, const QByteArray &message);
    void ExecSubscribe();   // main thread
    void ExecUnSubscribe();   // main thread
    void updateLogStateChange();
    void RecvHandle(const QByteArray &message, const QMqttTopicName &topic);



signals:
    void serialopenSinal(QString = nullptr);
    void publishSignal(QMqttClient *client, const QString &topic, const QByteArray &message);
    void subscribeSignal();
    void unsubscribeSignal();
    void connectSignal(QMqttClient *client);




public slots:

private slots:

    void on_buttonPublish_clicked();

    void on_buttonConnect_clicked();

    void on_buttonSubscribe_clicked();

    void on_buttonUnsubscribe_clicked();


    void on_editLog_selectionChanged();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_buttonClear_clicked();

private:


//    QByteArray requestData;


    QTimer *timer;    // this timer is for serial

 //   uint16_t *tab_reg;   get data to infomodel->currentdata


    int combo_index;
 //   QMutex uart_mutex;
    QString ProductID = "FIRE";
    QString AreaID = "CQ-BS";
    QString AreaID_Num = "2";

    QString Topic_Head = ProductID+"/" + AreaID+"/" + AreaID_Num + "/";



};




#define DEBUG_MOD   0


#endif // FireForm_H
