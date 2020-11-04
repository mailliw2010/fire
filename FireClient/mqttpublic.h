#ifndef MQTTPUBLIC_H
#define MQTTPUBLIC_H

#include <QString>
#include <QObject>


#define Fr_Head        0x1D
#define Fr_End         0x7F
#define Fr_RDIV        0x1E
#define Fr_DDIV        0x1F

class MyTopic
{
public:

    typedef struct{
      QString reTopic;
      QString reMessage;

    }tReFrame;

    MyTopic()
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

        ProductID = "FIRE";
        AreaID = "CQ-BS";
        AreaID_Num = "5";

        Topic_Head = ProductID+"/" + AreaID+"/" + AreaID_Num + "/";

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



    QStringList MessageCmd;
    enum ENUM_MessageCmd {
        ON_ = 0,
        OFF_,
        ENE,
        FAUL,
        WAR,
        COUNT
    };





    QString ProductID;
    QString AreaID;
    QString AreaID_Num;

    QString Topic_Head;

};






#endif // MQTTPUBLIC_H
