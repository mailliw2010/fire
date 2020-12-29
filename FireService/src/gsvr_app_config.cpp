//#pragma execution_character_set(“utf-8”)
#include "gsvr_app_config.h"

#include <math.h>
#include <iostream>
#include "gsvr_app_server.h"
#include "globalFun.h"
#include "gsvr_thread_listen.h"
#include "gsvr_app_log.h"
#include "globalFun.h"
#include "modbus/modbus-private.h"
#include "modbus/modbus-rtu-private.h"
#include"rapidjson/document.h"
#include<fstream>
#if defined(_WIN32)
// windows time
#else
#include <sys/time.h>
#endif
#include <iostream>





namespace rr
{

bool RrConfig::IsSpace(char c)
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

bool RrConfig::IsCommentChar(char c)
{
    switch (c) {
    case '#':
        return true;
    default:
        return false;
    }
}

void RrConfig::Trim(std::string & str)
{
    if (str.empty())
    {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size())
    {
        str = "";
        return;
    }
    start_pos = i;
    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool RrConfig::AnalyseLine(const std::string & line, std::string& section, std::string & key, std::string & value)
{
    if (line.empty())
        return false;
    int start_pos = 0, end_pos = line.size() - 1, pos, s_startpos, s_endpos;
    if ((pos = line.find("#")) != -1)
    {
        if (0 == pos)
        {
            return false;
        }
        end_pos = pos - 1;
    }
    if (((s_startpos = line.find("[")) != -1) && ((s_endpos = line.find("]"))) != -1)
    {
        section = line.substr(s_startpos+1, s_endpos -1);
        return true;
    }
    std::string new_line = line.substr(start_pos, start_pos + 1 - end_pos);
    if ((pos = new_line.find('=')) == -1)
        return false;
    key = new_line.substr(0, pos);
    value = new_line.substr(pos + 1, end_pos + 1 - (pos + 1));
    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(value);
    if ((pos = value.find("\r")) > 0)
    {
        value.replace(pos, 1, "");
    }
    if ((pos = value.find("\n")) > 0)
    {
        value.replace(pos, 1, "");
    }
    return true;
}

bool RrConfig::ReadConfig(const std::string & filename)
{
    settings_.clear();
    std::ifstream infile(filename.c_str());
    if (!infile) {
        return false;
    }
    std::string line, key, value, section;
    std::map<std::string, std::string> k_v;
    std::map<std::string, std::map<std::string, std::string> >::iterator it;
    while (getline(infile, line))
    {
        if (AnalyseLine(line, section, key, value))
        {
            it = settings_.find(section);
            if (it != settings_.end())
            {
                k_v[key] = value;
                it->second = k_v;
            }
            else
            {
                k_v.clear();
                settings_.insert(std::make_pair(section, k_v));
            }
        }
        key.clear();
        value.clear();
    }
    infile.close();
    return true;
}

std::string RrConfig::ReadString(const char* section, const char* item, const char* default_value)
{
    std::string tmp_s(section);
    std::string tmp_i(item);
    std::string def(default_value);
    std::map<std::string, std::string> k_v;
    std::map<std::string, std::string>::iterator it_item;
    std::map<std::string, std::map<std::string, std::string> >::iterator it;
    it = settings_.find(tmp_s);
    if (it == settings_.end())
    {
        return def;
    }
    k_v = it->second;
    it_item = k_v.find(tmp_i);
    if (it_item == k_v.end())
    {
        return def;
    }
    return it_item->second;
}

int RrConfig::ReadInt(const char* section, const char* item, const int& default_value)
{
    std::string tmp_s(section);
    std::string tmp_i(item);
    std::map<std::string, std::string> k_v;
    std::map<std::string, std::string>::iterator it_item;
    std::map<std::string, std::map<std::string, std::string> >::iterator it;
    it = settings_.find(tmp_s);
    if (it == settings_.end())
    {
        return default_value;
    }
    k_v = it->second;
    it_item = k_v.find(tmp_i);
    if (it_item == k_v.end())
    {
        return default_value;
    }
    return atoi(it_item->second.c_str());
}

float RrConfig::ReadFloat(const char* section, const char* item, const float& default_value)
{
    std::string tmp_s(section);
    std::string tmp_i(item);
    std::map<std::string, std::string> k_v;
    std::map<std::string, std::string>::iterator it_item;
    std::map<std::string, std::map<std::string, std::string> >::iterator it;
    it = settings_.find(tmp_s);
    if (it == settings_.end())
    {
        return default_value;
    }
    k_v = it->second;
    it_item = k_v.find(tmp_i);
    if (it_item == k_v.end())
    {
        return default_value;
    }
    return atof(it_item->second.c_str());
}
}











ConfigWin::ConfigWin(std::string dir):config_dir(dir)

{

    LOG(INFO) << "Constructing ConfigWin";
    para = new ParaItems;
    // must config first!
    ConfigFromFile();


}


void ConfigWin::initialize()
{

    LOG(INFO) << "Initializing ConfigWin";

}


void ConfigWin::printdebugmsg()
{
    LOG(INFO) <<  "para->FrameInfo.OverTime=" << para->FrameInfo.OverTime;

    LOG(INFO) <<  "para->ClocalInfo.host=" << para->ClocalInfo.host;
    LOG(INFO) <<  "para->ClocalInfo.port="  <<  para->ClocalInfo.port;
    LOG(INFO) <<  "para->ClocalInfo.user=" <<  para->ClocalInfo.user;
    LOG(INFO) <<  "para->ClocalInfo.pass=" <<  para->ClocalInfo.pass;
    LOG(INFO) <<  "para->DeviceIDStart=" <<  para->DeviceIDStart;
    LOG(INFO) <<  "para->DeviceIDEnd=" <<  para->DeviceIDEnd;
    LOG(INFO) <<  "para->ProductAREA=" << para->ProductAREA;
    LOG(INFO) <<  "para->ProductID=" << para->ProductID;
    LOG(INFO) <<  "para->ProductNUM=" << para->ProductNUM;
    LOG(INFO) <<  "para->FrameInfo.ReSendCount=" << para->FrameInfo.ReSendCount;
    LOG(INFO) <<  "para->FrameInfo.AutoUploadTime=" << para->FrameInfo.AutoUploadTime;
    LOG(INFO) <<  "para->FrameInfo.FrameEachSize=" << para->FrameInfo.FrameEachSize;
    LOG(INFO) <<  "para->FrameInfo.OverTime=" << para->FrameInfo.OverTime;
    LOG(INFO) <<  "para->BaurdRate=" << para->BaurdRate;
    LOG(INFO) <<  "para->DataBit="  << para->DataBit;
    LOG(INFO) <<  "para->StopBit=" << para->StopBit;
    LOG(INFO) <<  "para->DeviceName=" << para->DeviceName;


}

int ConfigWin::parse_json()
{
    std::ifstream in(config_dir.data(), std::ios::in);
    std::istreambuf_iterator<char> beg(in), end;
    std::string strdata(beg, end);
    LOG(INFO) << strdata;
    in.close();

    try {
        if(doc.Parse(strdata.data()).HasParseError()){

            throw -1;
        }
        if(!doc.IsObject()){

            throw -1;
        }

        throw 0;

    } catch (int errnum) {
        if(errnum < 0){
            LOG(ERROR) << "parse error!use default setting";
            //default settings
            UseDefaultConfig();

        }else{
            para->Serial_Wait_Time = get_json_val("modbus","timeout").GetUint();
            para->ClocalInfo.host = get_json_val("mqtt_local","host").GetString();
            para->ClocalInfo.port = get_json_val("mqtt_local","port").GetInt();
            para->ClocalInfo.user = get_json_val("mqtt_local","user").GetString();
            para->ClocalInfo.pass = get_json_val("mqtt_local","password").GetString();
            para->DeviceIDStart = get_json_val("mqtt_public","devid_s").GetUint();
            para->DeviceIDEnd = get_json_val("mqtt_public","devid_e").GetUint();
            para->ProductAREA = get_json_val("mqtt_public","pro_area").GetString();
            para->ProductID = get_json_val("mqtt_public","pro_id").GetString();
            para->ProductNUM = get_json_val("mqtt_public","pro_num").GetInt();
            para->FrameInfo.ReSendCount = get_json_val("mqtt_public","resend_num").GetUint();
            para->FrameInfo.AutoUploadTime = get_json_val("mqtt_public","info_per").GetUint();
            para->FrameInfo.FrameEachSize = 1024;
            para->FrameInfo.OverTime = get_json_val("mqtt_public","timeout").GetUint();

            para->BaurdRate = get_json_val("serial","baurd").GetInt();
            para->DataBit = get_json_val("serial","databit").GetInt();
            para->StopBit = get_json_val("serial","stopbit").GetInt();
            para->DeviceName = get_json_val("serial","dev_dir").GetString();

            printdebugmsg();

        }
        // assign to topic and _mqtt_topic
        para->topic = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/#";
        _mqtt_topic::Topic_Head = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/";
    }

}


rapidjson::Value& ConfigWin::get_json_val(const char* skey_1, const char* skey_2)
{

    if(doc.HasMember(skey_1)){
        rapidjson::Value &opt = doc[skey_1];

        if(doc.HasMember(skey_1)){
            if(skey_2 != nullptr){
                if(opt.HasMember(skey_2)){
                    return opt[skey_2];
                }
                else{
                    LOG(WARNING) << skey_2 << " does not has a value!";
                }
            }
            else{
                return opt[skey_1];
            }

        }

    }
    else{
        LOG(WARNING) << skey_1 << " does not has a value!";
    }
    LOG(INFO) << "fsdfdf";
//    return doc["null"];


}


void ConfigWin::UseDefaultConfig()
{
    para->BaurdRate = 9600;
    para->DataBit = 8;
    para->StopBit = 1;
    para->FrameInfo.OverTime = 800;




    para->ClocalInfo.host = "183.66.138.122";
    para->ClocalInfo.keepalivetime = 5;
    para->ClocalInfo.port = 1883;
    para->ClocalInfo.user = "admin";
    para->ClocalInfo.pass = "public";

    //  para->CremoteInfo.host = "183.66.138.1222";
    para->CremoteInfo.keepalivetime = 5;
    para->CremoteInfo.pass = "public";
    para->CremoteInfo.port = 1883;
    para->CremoteInfo.user = "admin";
    para->DeviceIDStart = 1;
    para->DeviceIDEnd = 21;
    para->DeviceName = "/dev/ttyUSB0";
    para->LOCAL_CLIENT_ID = "123456";
    para->ProductAREA = "CQ-BS";
    para->ProductID = "FIRE";
    para->ProductNUM = 1;
    para->topic = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/#";
    _mqtt_topic::Topic_Head = para->ProductID + "/" + para->ProductAREA + "/" + std::to_string(para->ProductNUM) + "/";

    ResponseTimeOut = para->Serial_Wait_Time;

}

void ConfigWin::ConfigFromFile()
{
    parse_json();
}


void ConfigWin::CopyParaToUi()
{



}

void ConfigWin::SaveToConfigFile()
{


}




ConfigWin::~ConfigWin()
{


    LOG(INFO) << "deleting para";
    delete para;




}







/* I try add '+' in string, can it be handle correctly?*/
void ConfigWin::StringToHex(const std::string& str, std::vector<uint8_t> &senddata)  //StringToHex(source,desti);
{
    int high_hexbit, low_hexbit;
    size_t hexdatalen = 0;
    size_t len = str.length();  //len must be even;
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
        hstr = str[i];  // from 00-0xff
      //  qDebug() << hstr;
        if(hstr == ' ' || hstr == '+')   // we want to escape '+'
        {
            continue;
        }

        if(i >=len)
            break;

        lstr = str[++i];

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

//  "0x30" --> 0x30
char ConfigWin::ConvertHexChar(char ch)
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














ParaItems * ConfigWin::getpara() {
    return para;

}





int ConfigWin::Para_Set_FrameAll(uint32_t frameEachSize, uint32_t overtime, uint32_t resendCount, uint32_t autoUploadTime)
{
    int flag  = 0;

    if(frameEachSize >= 1 && frameEachSize <= 204800000)
    {
        para->FrameInfo.FrameEachSize = frameEachSize;
    }
    else {
        flag |= 1<<0;
    }

    if(overtime >= 1 && overtime <= 7200)
    {
        para->FrameInfo.OverTime = overtime;
    }
    else {
          flag |= 1<<1;
    }

    if(resendCount >= 1 && resendCount <= 5)
    {
        para->FrameInfo.ReSendCount = resendCount;

    }
    else {

         flag |= 1<<2;
    }


    if(autoUploadTime >= 1 && autoUploadTime <= 86400)   // 1s - 1 day
    {
        para->FrameInfo.AutoUploadTime = autoUploadTime;

    }
    else {
          flag |= 1<<3;
    }

    if((flag & 0x01) != 0)
    {
        LOG(INFO) << "Set para out of range:frameEachSize";

    }
    if((flag & 0x02) != 0)
    {
        LOG(INFO) << "Set para out of range:overtime";
    }
    if((flag & 0x04) != 0)
    {
        LOG(INFO) << "Set para out of range:ReSendCount";
    }
    if((flag & 0x08) != 0)
    {
        LOG(INFO) << "Set para out of range:AutoUploadTime";

    }

    return flag;

}


int ConfigWin::Para_Set_FrameEachSize(uint32_t frameEachSize)
{
    int flag = 0;
    if(frameEachSize >= 1 && frameEachSize <= 204800000)
    {
        para->FrameInfo.FrameEachSize = frameEachSize;
    }
    else {
        flag &= 1<<0;
    }
    return flag;
}
int ConfigWin::Para_Set_FrameOverTime(uint32_t overtime)
{
    int flag = 0;
    if(overtime >= 1 && overtime <= 7200)
    {
        para->FrameInfo.OverTime = overtime;
    }
    else {
          flag &= 1<<1;
    }
    return flag;
}
int ConfigWin::Para_Set_FrameReSendCount(uint32_t resendCount)
{
    int flag = 0;
    if(resendCount >= 1 && resendCount <= 5)
    {
        para->FrameInfo.OverTime = resendCount;

    }
    else {
         flag &= 1<<2;
    }
    return flag;
}
int ConfigWin::Para_Set_FrameAutoUploadTime(uint32_t autoUploadTime)
{
    int flag = 0;
    if(autoUploadTime >= 10 && autoUploadTime <= 86400)   // 10ms - 1 day
    {
        para->FrameInfo.AutoUploadTime = autoUploadTime;

    }
    else {
          flag &= 1<<3;
    }
    return flag;
}






ConfigWin::tTopicHead ConfigWin::Para_Get_ProductInfo()
{

}

ConfigWin::tTopicHead ConfigWin::Para_Get_ExtProductInfo()
{




}


ConfigWin::tDevSerialNum ConfigWin::Para_Get_DeviceIDSerial()
{


}



uint32_t ConfigWin::Para_Get_FrameEachSize()
{
    return para->FrameInfo.FrameEachSize;

}
uint32_t ConfigWin::Para_Get_FrameOverTime()
{
    return para->FrameInfo.OverTime;
}
uint32_t ConfigWin::Para_Get_FrameReSendCount()
{
    return para->FrameInfo.ReSendCount;
}
uint32_t ConfigWin::Para_Get_FrameAutoUploadTime()
{
    return para->FrameInfo.AutoUploadTime;
}



ConfigWin::tclientInfo ConfigWin::Para_Get_ClientRemoteInfo()
{
    return para->CremoteInfo;

}
ConfigWin::tclientInfo ConfigWin::Para_Get_ClientLocalInfo()
{
    return para->ClocalInfo;

}



int ConfigWin::Para_Get_Mode()
{
    return para->debugmode;
}

ConfigWin::tserialpara ConfigWin::Para_Get_SerialP()
{


}
