#ifndef GSVR_APP_PUBLIC_H
#define GSVR_APP_PUBLIC_H

#include "gsvr_app_mqtt.h"
#include <memory>

/**Frame Define**/
#define Fr_Head        0x1D
#define Fr_End         0x7F
#define Fr_RDIV         0x1E
#define Fr_DDIV         0x1F

namespace _mqtt_topic {
    extern std::string Topic_Head;     // must be declear, not define
    extern std::string ExtTopic_Head;

}

std::string to_string_n(float a_value, const int n = 5) ;

std::vector<std::string> split_by_char(std::string &str, char ch);

// they must put in .h file , not know why yet
typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis);
constexpr hash_t operator"" _hash(char const* p, size_t);
hash_t hash_(char const* str);


constexpr hash_t hash_compile_time(char const* str, hash_t last_value)
{
    return *str ? hash_compile_time(str+1, (*str ^ last_value) * prime) : last_value;
}

constexpr hash_t operator"" _hash(char const* p, size_t)
{
    return hash_compile_time(p);

}

hash_t hash_(char const* str);

//template<class T1, class... Args>
int hello_world(int i, std::string s);

class MqttPublic
{
public:

    typedef struct{
      std::string reTopic;
      std::string rePayload;

    }tReFrame;

    MqttPublic()
    {
//        TopicType    <<
//        "FrameInfo"  <<
//        "ReSerPara"  <<
//        "GetPara"    <<
//        "ReGetPara"  <<
//        "Info"       <<
//        "ReInfo"     <<
//        "War"        <<
//        "ReWar"      <<
//        "Cmd"        <<
//        "ReCmd"      <<
//        "Clear"      <<
//        "ReClear"    <<
//        "TimeSyn"    <<
//        "SetBVal"    <<
//        "ReadBVal"   <<
//        "ReReadBVal" <<
//        "ReSetBVal"  <<
//        "SetBEn"     <<
//        "ReadBEn"    <<
//        "ReReadBEn"  <<
//        "ReSetBEn"   <<
//        "SetBCor"    <<
//        "ReadBCor"   <<
//        "ReReadBCor" <<
//        "ReSetBCor"  <<
//        "EqState"    <<
//        "GwState"    <<
//        "UpInfo"     <<
//        "GetFile"    <<
//        "ReGetFile"  <<
//        "ReUpOk"     <<
//        "FileData"   <<
//        "ReFileData" <<
//        "Hand"       <<
//        "UpPic"      <<
//        "ReUpPic"    <<
//        "PutPic"     <<
//        "RePutPic"   <<
//        "UpPicOk";

//        MessageCmd << "ON" << "OFF" << "ENE" << "FAUL" << "WAR" << "COUNT" ;

//        ProductID = "FIRE";
//        AreaID = "CQ-BS";
//        AreaID_Num = "3";

//        Topic_Head = ProductID+"/" + AreaID+"/" + AreaID_Num + "/";

    }

    enum ENUM_TopicType {
        FrameInfo = 0,
        ReSerPara,
        GetPara,
        ReGetPara,
        Info,
        ReInfo,
        War,
        ReWar,
        Cmd,
        ReCmd,
        Clear,
        ReClear,
        Fire_TimeSyn,
        SetBVal,
        ReadBVal,
        ReReadBVal,
        ReSetBVal,
        SetBEn,
        ReadBEn,
        ReReadBEn,
        ReSetBEn,
        SetBCor,
        ReadBCor,
        ReReadBCor,
        ReSetBCor,
        EqState,
        GwState,
        UpInfo,
        GetFile,
        ReGetFile,
        ReUpOk,
        FileData,
        ReFileData,
        Hand,
        UpPic,
        ReUpPic,
        PutPic,
        RePutPic,
        UpPicOk
    };


    typedef struct{
      std::shared_ptr<gmqtt> remote;
      std::shared_ptr<gmqtt> local;

    }tMqttClient;

    typedef struct{

        /**File Info**/
        std::string FileName;
        std::string FileSize;
        std::string CreateTime;
        std::string ModifyTime;
        std::vector<uint8_t> FileBuffer;
        std::string FileStrBuffer;

        /**Frame Info**/
        int FrameEachSize;
        int FrameEndSize;
        int FrameAllSize;
        int FrameCount;
        int FrameNo;
    }tUpFileInfo;

    typedef struct{
        uint32_t FrameEachSize;     //1-204800000
        uint32_t OverTime;   //1-7200 s
        uint32_t ReSendCount;  //1-5
        uint32_t AutoUploadTime;   //1—86400 s
    }tFrameInfo;


    typedef struct{
        std::string ID;
        std::string AREA;
        std::string NUM;
    }tTopicHead;

    typedef struct{
      uint32_t devStart;
      uint32_t devEnd;
    }tDevSerialNum;


    typedef struct{
        std::string host;
        int port;
        std::string user;
        std::string pass;
        int keepalivetime;      // new

    }tclientInfo;
    int maxBuffer = 1024;
    std::string persistDir = "savedBuff";

    typedef struct{
      std::string name;
      int baurd;
      int databit;
      int stopbit;
    }tserialpara;



    enum ENUM_MessageCmd {
        ON_ = 0,
        OFF_,
        ENE,
        FAUL,
        WAR,
        COUNT
    };



    std::vector<std::string> TopicType;  //

};




#endif // MQTTPUBLIC_H
