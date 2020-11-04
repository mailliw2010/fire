#ifndef GSVR_APP_PUBLIC_H
#define GSVR_APP_PUBLIC_H

#include <QString>
#include <QObject>
#include<QtMqtt/QMqttClient>
#include "gsvr_app_mqtt.h"

/**Frame Define**/
#define Fr_Head        0x1D
#define Fr_End         0x7F
#define Fr_RDIV         0x1E
#define Fr_DDIV         0x1F

namespace _pubvar {
    extern QString Topic_Head;     // must be declear, not define
    extern QString ExtTopic_Head;

}

class MqttPublic
{
public:

    typedef struct{
      QString reTopic;
      QString rePayload;

    }tReFrame;

    MqttPublic()
    {
        TopicType    <<
        "FrameInfo"    <<
        "ReSerPara"  <<
        "GetPara"    <<
        "ReGetPara"  <<
        "Info"       <<
        "ReInfo"     <<
        "War"        <<
        "ReWar"      <<
        "Cmd"        <<
        "ReCmd"      <<
        "Clear"      <<
        "ReClear"    <<
        "TimeSyn"    <<
        "SetBVal"    <<
        "ReadBVal"   <<
        "ReReadBVal" <<
        "ReSetBVal"  <<
        "SetBEn"     <<
        "ReadBEn"    <<
        "ReReadBEn"  <<
        "ReSetBEn"   <<
        "SetBCor"    <<
        "ReadBCor"   <<
        "ReReadBCor" <<
        "ReSetBCor"  <<
        "EqState"    <<
        "GwState"    <<
        "UpInfo"     <<
        "GetFile"    <<
        "ReGetFile"  <<
        "ReUpOk"     <<
        "FileData"   <<
        "ReFileData" <<
        "Hand"       <<
        "UpPic"      <<
        "ReUpPic"    <<
        "PutPic"     <<
        "RePutPic"   <<
        "UpPicOk";

        MessageCmd << "ON" << "OFF" << "ENE" << "FAUL" << "WAR" << "COUNT" ;

//        ProductID = "FIRE";
//        AreaID = "CQ-BS";
//        AreaID_Num = "3";

//        Topic_Head = ProductID+"/" + AreaID+"/" + AreaID_Num + "/";

    }
    QStringList TopicType;  //
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
      gmqtt* remote;
      gmqtt* local;

    }tMqttClient;

    typedef struct{
        /**File Info**/
        QString FileName;
        QString FileSize;
        QString CreateTime;
        QString ModifyTime;
        QByteArray FileBuffer;
        QString FileStrBuffer;

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




    QStringList MessageCmd;
    enum ENUM_MessageCmd {
        ON_ = 0,
        OFF_,
        ENE,
        FAUL,
        WAR,
        COUNT
    };



};




#endif // MQTTPUBLIC_H
