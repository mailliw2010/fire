
#include "gsvr_app_server.h"
#include "gsvr_thread_listen.h"
#include "gsvr_thread_report.h"
#include <QApplication>
#include "globalFun.h"
#include <QDebug>
#include<QFile>
#include <stdlib.h>
#include<string.h>

#include "gsvr_app_log.h"
extern QtMessageHandler gDefaultHandler;



/***
class mainwindow:
serialwindow *settingserial
ThreadMqttListen *threadMqttListen
ThreadMqttListen *threadMqttReport
QMqttClient *m_client;

class serialwindow:



class infomodel


class threadMqttListen



class threadMqttReport


****/


void BuildTimeInfo()
{
    printf("Build Time!\n");
    QDate buildDate = QLocale( QLocale::English ).toDate( QString( __DATE__ ).replace( "  ", " 0" ), "MMM dd yyyy");
    QTime buildTime = QTime::fromString( __TIME__, "hh:mm:ss" );
    qInfo() << "#######Build Time#######";
    qInfo() << "#" << buildDate.toString();
    qInfo() << "#" << buildTime.toString();
}



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    /******Set the Font and Icon*********/

#ifdef _ARM_LINUX_
    QFont font  = a.font();
    font.setPointSize(5);
    a.setFont(font);0
#endif

    qDebug() << "Entering With Log Mode!\n";
    gDefaultHandler = qInstallMessageHandler(outputMessage);
    qInstallMessageHandler(outputMessage);   // add this to save qDebug/qWarning/qCritical to log
    BuildTimeInfo();               // build time Info
    MainWindow w;
    qDebug() << "the Main thread = " << QThread::currentThreadId();

   /*******************use qss file*********************/
    QFile styleFile("://resource/resource/qss/Aqua.qss");
    if(styleFile.open(QIODevice::ReadOnly))
    {
        QString setStyleSheet(styleFile.readAll());
        a.setStyleSheet(setStyleSheet);
        styleFile.close();
    }
    else
    {
        qDebug("Open failed");
    }


    return a.exec();
}
