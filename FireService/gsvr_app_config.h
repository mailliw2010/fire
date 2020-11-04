#ifndef GSVR_APP_CONFIG_H
#define GSVR_APP_CONFIG_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include<QFileDevice>
#include <QTcpSocket>
#include<QString>
#include "modbus/modbus.h"

#include <QDialog>
#include <QRegExpValidator>
#include <QMutex>
#include <QtMqtt/QMqttClient>
#include<QSettings>

#include "gsvr_info_break.h"
#include "gsvr_app_public.h"
#include "gsvr_app_view.h"
#include "globalFun.h"
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
class ConfigWin;
}


class ParaItems: public MqttPublic
{

public:
    /************************Config Items*************************/
    /**mqtt***/

    tclientInfo ClocalInfo;
    tclientInfo CremoteInfo;

    QString ProductID;
    QString ProductAREA;
    QString ProductNUM;

    QString ExtProductID;
    QString ExtProductAREA;
    QString ExtProductNUM;

    uint32_t DeviceIDStart;
    uint32_t DeviceIDEnd;

    QString REMOTE_CLIENT_ID;
    QString LOCAL_CLIENT_ID;
    tFrameInfo FrameInfo;
    int debugmode;


    /**Serial Variable**/    // value is set in Config file!!
#if defined (_WIN32)   // windows
    QString DeviceName = "COM6";
#elif defined (__x86_64)  // x86 gcc
    QString DeviceName = "/dev/pts/1";
#else  // _ARM_LINUX_    // arm-linux-gcc
    QString DeviceName = "/dev/ttymxc0";
#endif
    int BaurdRate;
    int DataBit;
    int StopBit;


#if defined (_WIN32)   // windows
    QString UrlName = "C:/inetpub/wwwroot/views/switch.html";
#elif defined (__x86_64)  // x86 gcc
   // /home/forlinx/web/index.html   --old web
   // /home/forlinx/wwwroot/views/index.html  --new web
    QString UrlName = "/home/forlinx/wwwroot/views/switch.html";
#else    // default is  _ARM_LINUX_    // arm-linux-gcc
    QString UrlName = "/home/forlinx/web/index.html";
#endif

};


class MainServer;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class ConfigWin : public QDialog, public MqttPublic     // ConfigWin is paishenglei dialog, so it can be closed!
{
    Q_OBJECT

public:
    friend class MainServer;
    friend class View;
    explicit ConfigWin(QWidget *parent = nullptr, View* mwin = nullptr);  //gouzaohanshu
    ~ConfigWin();     //gouxihanshu
    void initialize();
    void autoSubscribe();
   // int ModbusWriteOneRegister(int devid, int addr, uint16_t cmd);
//    int ModbusReadMultiRegister(int devid, int addr, int num, uint16_t *reg);
    static char ConvertHexChar(char ch);
    static void StringToHex(const QString& str, QByteArray &senddata);

    void Set2IniFile(const QString&, const QString&, const QVariant&);
    QVariant GetFromInitFile(const QString&, const QString&);
    void getserialObj(modbus_t* &tmpctx);




    int Para_Set_ProductInfo(const QString& ProductID, const QString& ProductAREA, const QString& ProductNUM);
    int Para_Set_ExtProductInfo(const QString& ProductID, const QString& ProductAREA, const QString& ProductNUM);
    int Para_Set_DeviceIDSerial(uint32_t start, uint32_t end);
    int Para_Set_FrameAll(uint32_t frameEachSize, uint32_t overtime, uint32_t resendCount, uint32_t autoUploadTime);
    int Para_Set_FrameEachSize(uint32_t frameEachSize);
    int Para_Set_FrameOverTime(uint32_t overtime);
    int Para_Set_FrameReSendCount(uint32_t resendCount);
    int Para_Set_FrameAutoUploadTime(uint32_t autoUploadTime);
    int Para_Set_ClientLocalInfo(quint16 port, const QString& host, const QString& user, const QString& pass);
    int Para_Set_ClientRemoteInfo(quint16 port, const QString& host, const QString& user, const QString& pass);
    int Para_Set_Mode(int mode);
    int Para_Set_SerialP(QString devname, int baurd, int databit, int stopbit);

    ConfigWin::tTopicHead Para_Get_ProductInfo();
    tTopicHead Para_Get_ExtProductInfo();
    tDevSerialNum Para_Get_DeviceIDSerial();
    uint32_t Para_Get_FrameEachSize();
    uint32_t Para_Get_FrameOverTime();
    uint32_t Para_Get_FrameReSendCount();
    uint32_t Para_Get_FrameAutoUploadTime();
    tclientInfo Para_Get_ClientRemoteInfo();
    tclientInfo Para_Get_ClientLocalInfo();
    int Para_Get_Mode();
    tserialpara Para_Get_SerialP();




protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);



private:
    ConfigWin(const ConfigWin&);
    ConfigWin& operator=(const ConfigWin&);
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
    View* m_view;
    ParaItems* para;
    tMqttClient m_Client;
    QSerialPort *m_serialPort;

    static QString emi_sendData;  // sended data to emi device
    QTimer *timer;    // this timer is for serial
    QRegExpValidator *pReg2;
    QRegExpValidator *pReg3;
    int combo_index;
    Ui::ConfigWin *ui;


    QString m_ConfigFile;
    QSettings *m_ConfSet;
    QByteArray requestData;
    modbus_t *ctx;
    static int SendNum;
    static int ReceiveNum;
    static  bool flag;


};




#define DEBUG_MOD   0


#endif
