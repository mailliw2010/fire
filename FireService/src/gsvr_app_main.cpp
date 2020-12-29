
#include "gsvr_app_server.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include "globalFun.h"
#include <stdlib.h>
#include<string.h>
#include <iomanip>
#include<chrono>
#include<iomanip>
#include "gsvr_app_log.h"
#include "gsvr_app_threadpool.h"

void BuildTimeInfo()
{
    const char version[] = "V1.0.0";
    char time_buf[32];
    auto t = time(0);
    strftime(time_buf, 32, "%Y-%m-%d %H:%M:%S", localtime(&t)); //format date and time.

    LOG(INFO) << "#######App Info###############";
    LOG(INFO) << "#Vers:" << version << "(by STL Library)";
    LOG(INFO) << "#Date:" << time_buf;
    LOG(INFO) << "###############################";
}


int main(int argc, char *argv[])
{


    Log_initial(argv[0]);
    /******Set the Font and Icon*********/

#ifdef _ARM_LINUX_
    QFont font  = a.font();
    font.setPointSize(5);
    a.setFont(font);0
#endif

    BuildTimeInfo();               // build time Info
    MainServer w;
    w.initialize();


    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        LOG(INFO) << "heart beat";

    }


}
