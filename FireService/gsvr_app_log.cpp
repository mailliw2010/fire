#include "gsvr_app_log.h"
#include <QDebug>
#include <QMutex>
#include <QDateTime>
#include <QFile>

QtMessageHandler gDefaultHandler = NULL;

/**Log System**/
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;     //will this mutex influent efficient??
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug");
        break;
    case QtInfoMsg:
        text = QString("Info");
        break;

    case QtWarningMsg:
        text = QString("Warning");
        break;
    case QtCriticalMsg:
        text = QString("Critical");
        break;
    case QtFatalMsg:
        text = QString("Fatal");
    }
    QString context_info = QString("{File:%1 Line:%2}").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    /**Message Format*/
    QString message = QString("%1 [%2]: %3 %4").arg(current_date_time).arg(text).arg(msg).arg(context_info);
    QFile file("fire_log");
    if(file.size() > 100*1024*1024)
    {
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate);   // Truncate to fflush file;

    }
    else {
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    }

    QTextStream text_stream(&file);
    text_stream << message << "\n";
    file.flush();
    file.close();
    mutex.unlock();


    if(gDefaultHandler)
       {
           gDefaultHandler(type,context,msg);
       }



}

