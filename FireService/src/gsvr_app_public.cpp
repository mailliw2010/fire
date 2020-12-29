#include "gsvr_app_public.h"
#include "gsvr_app_log.h"
#include<sstream>
#include<iomanip>
std::string _mqtt_topic::Topic_Head;
std::string _mqtt_topic::ExtTopic_Head;




std::vector<std::string> split_by_char(std::string & str, char ch)
{
    std::stringstream ss(str);
    std::string tok;
    std::vector<std::string> ret;
    while(std::getline(ss, tok, ch))
    {
        if(tok >= "")       // when meet "",  treat as one vector
            ret.push_back(tok);
    }
    if(str.back() == ch){    // if the last one is ch, treat the last one as vector
        ret.push_back("");
    }
    return ret;
}


hash_t hash_(char const* str)
{
    hash_t ret{basis};

    while(*str){
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}

int hello_world(int i, std::string s)
{
    //i = 9;
    for(;;){
        LOG(INFO) << "hello " << i << std::this_thread::get_id();
        std::this_thread::sleep_for(std::chrono::seconds(1));


    }

    return i*i;
}

//http://www.cplusplus.com/reference/iomanip/setprecision/?kw=setprecision
std::string to_string_n(float a_value, const int n)
{
    std::ostringstream out;
//   setprecision is always work with setiosflags(ios::fixed), but without setiosflags, n = all data; like n = 5: 3.1415
    out << std::setprecision(n) << a_value;
    return out.str();
}
