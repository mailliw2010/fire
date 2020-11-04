#ifndef GSVR_APP_CONFIG_H
#define GSVR_APP_CONFIG_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include<QFileDevice>
#include <QTcpSocket>
#include<QString>
#include "dlt645/dlt645.h"

#include <QDialog>
#include <QRegExpValidator>
#include <QMutex>
#include <QtMqtt/QMqttClient>
#include<QSettings>

#include "gsvr_info_mccb.h"
#include "gsvr_app_public.h"

#if defined(_WIN32)
#pragma comment(lib,"ws2_32.lib")
#endif


//#define BUG_REPORT(_cond, _format, _args ...) \
//    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

//#define ASSERT_TRUE(_cond, _format, __args...) {  \
//    if (_cond) {                                  \
//        printf("");                           \
//    } else {                                      \
//        BUG_REPORT(_cond, _format, ## __args);    \
//    }                                             \
//};

namespace Ui {
class SerialWindow;
}

class MainWindow;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class SerialWindow : public QDialog, public MqttPublic     // SerialWindow is paishenglei dialog, so it can be closed!
{
    Q_OBJECT

public:
    friend class MainWindow;
    explicit SerialWindow(MainWindow* mwin, QWidget *parent = nullptr);  //gouzaohanshu
    ~SerialWindow();     //gouxihanshu
    void autoSubscribe();
    void closeEvent(QCloseEvent *event);
   // int ModbusWriteOneRegister(int devid, int addr, uint16_t cmd);
//    int ModbusReadMultiRegister(int devid, int addr, int num, uint16_t *reg);
    static char ConvertHexChar(char ch);
    static void StringToHex(QString str, QByteArray &senddata);

    void Set2IniFile(QString, QString, QVariant);
    QVariant GetFromInitFile(QString, QString);
    dlt645_t* getserialObj(dlt645_t *tmpctx);


private:
    void ConfigFromFile();
    void SaveToConfigFile();
    void CopyParaToUi();
    void UseDefaultConfig();




signals:
    void serialopenSinal(QString = nullptr);
    void publishSignal(const QString &topic, const QByteArray &message);
    void subscribeSignal();
    void unsubscribeSignal();
    void connectSignal(QMqttClient *client);

public slots:

//   void ModbusCtrlSwitch(const int devid, const int cmd);  //0xff, open; 0x00, close
//   int ModbusGetStatus(const int devid);   // -1: read error; 0: right






private slots:

    void on_pushButton_comopen_clicked();
    void readMyCom();
    void on_pushButton_emi_write_clicked();
    void DeleteCurrentIndex();
    void on_pushButton_clicked();
    void on_pushButton_sendNum_clicked();
    void on_pushButton_recvNum_clicked();
    void on_buttonPublish_clicked();
    void on_buttonConnect_clicked();
    void on_buttonSubscribe_clicked();
    void on_buttonUnsubscribe_clicked();
    void on_buttonConnect_local_clicked();
    void on_pushButton_emi_write_pressed();


    void on_buttonSave_clicked();


private:
    MainWindow *m;   //create a pointer to mainwindow.
    tMqttClient m_Client;
    QSerialPort *m_serialPort;

    static QString emi_sendData;  // sended data to emi device
    QTimer *timer;    // this timer is for serial
    QRegExpValidator *pReg2;
    QRegExpValidator *pReg3;
    int combo_index;
    Ui::SerialWindow *ui;


    QString m_ConfigFile;
    QSettings *m_ConfSet;
    QByteArray requestData;
    dlt645_t *ctx;
    static int SendNum;
    static int ReceiveNum;
    static  bool flag;


};




#define DEBUG_MOD   0


#endif // SERIALWINDOW_H
