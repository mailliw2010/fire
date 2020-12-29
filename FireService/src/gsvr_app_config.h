#ifndef GSVR_APP_CONFIG_H
#define GSVR_APP_CONFIG_H


#include "modbus/modbus.h"


#include "gsvr_info_break.h"
#include "gsvr_app_public.h"
#include "globalFun.h"
#include "gsvr_app_public.h"
#include"rapidjson/document.h"
#include<map>
#include<string>
#if defined(_WIN32)
#pragma comment(lib,"ws2_32.lib")
#endif




#include <string>
#include <map>
namespace rr
{
class RrConfig
{
public:
    RrConfig()
    {
    }
    ~RrConfig()
    {
    }
    bool ReadConfig(const std::string & filename);
    std::string ReadString(const char* section, const char* item, const char* default_value);
    int ReadInt(const char* section, const char* item, const int& default_value);
    float ReadFloat(const char* section, const char* item, const float& default_value);
private:
    bool IsSpace(char c);
    bool IsCommentChar(char c);
    void Trim(std::string & str);
    bool AnalyseLine(const std::string & line, std::string& section, std::string & key, std::string & value);

private:
    //std::map<std::string, std::string> settings_;
    std::map<std::string, std::map<std::string, std::string> >settings_;
};
}








//#define BUG_REPORT(_cond, _format, _args ...) \
//    printf("\nLine %d: assertion error for '%s': " _format "\n", __LINE__, # _cond, ## _args)

//#define ASSERT_TRUE(_cond, _format, __args...) {  \
//    if (_cond) {                                  \
//        printf("");                           \
//    } else {                                      \
//        BUG_REPORT(_cond, _format, ## __args);    \
//    }                                             \
//};




class ParaItems: public MqttPublic
{

public:
    /************************Config Items*************************/
    /**mqtt***/

    tclientInfo ClocalInfo;
    tclientInfo CremoteInfo;

    std::string ProductID;
    std::string ProductAREA;
    int ProductNUM;
    std::string topic;

    std::string ExtProductID;
    std::string ExtProductAREA;
    std::string ExtProductNUM;
    std::string topic_ext;

    uint32_t DeviceIDStart;
    uint32_t DeviceIDEnd;

    std::string REMOTE_CLIENT_ID;
    std::string LOCAL_CLIENT_ID;
    tFrameInfo FrameInfo;
    int debugmode;


    /**Serial Variable**/    // value is set in Config file!!
#if defined (_WIN32)   // windows
    QString DeviceName = "COM6";
#elif defined (__x86_64)  // x86 gcc
    std::string DeviceName = "/dev/pts/1";
#else  // _ARM_LINUX_    // arm-linux-gcc
    std::string DeviceName = "/dev/ttyUSB0";
#endif
    int BaurdRate;
    int DataBit;
    int StopBit;
    int Serial_Wait_Time;   //ms;


#if defined (_WIN32)   // windows
    QString UrlName = "C:/inetpub/wwwroot/views/switch.html";
#elif defined (__x86_64)  // x86 gcc
   // /home/forlinx/web/index.html   --old web
   // /home/forlinx/wwwroot/views/index.html  --new web
    std::string UrlName = "/home/wwwroot/views/switch.html";
#else    // default is  _ARM_LINUX_    // arm-linux-gcc
    std::string UrlName = "/home/forlinx/web/index.html";
#endif

};




class MainServer;     //   c++中两个类互相引用的问题 https://www.cnblogs.com/weixliu/p/3957227.html
class ConfigWin : public MqttPublic     // ConfigWin is paishenglei dialog, so it can be closed!
{

public:
    friend class MainServer;
    friend class View;
    explicit ConfigWin(std::string dir);  //gouzaohanshu
    ~ConfigWin();     //gouxihanshu
    void initialize();
    void autoSubscribe();

    static char ConvertHexChar(char ch);
    static void StringToHex(const std::string& str, std::vector<uint8_t> &senddata);


 ;
    void getserialObj(modbus_t* &tmpctx);
    ParaItems * getpara();




    int Para_Set_FrameAll(uint32_t frameEachSize, uint32_t overtime, uint32_t resendCount, uint32_t autoUploadTime);
    int Para_Set_FrameEachSize(uint32_t frameEachSize);
    int Para_Set_FrameOverTime(uint32_t overtime);
    int Para_Set_FrameReSendCount(uint32_t resendCount);
    int Para_Set_FrameAutoUploadTime(uint32_t autoUploadTime);



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




private:
    ConfigWin(const ConfigWin&);
    ConfigWin& operator=(const ConfigWin&);
    void ConfigFromFile();
    void SaveToConfigFile();
    void CopyParaToUi();
    void UseDefaultConfig();
    int parse_json();
    rapidjson::Value& get_json_val(const char* skey_1, const char* skey_2);
    void printdebugmsg();


//signals:




private:

    ParaItems *para;
    tMqttClient m_Client;
    std::string config_dir;
    rapidjson::Document doc;


};




#define DEBUG_MOD   0


#endif
