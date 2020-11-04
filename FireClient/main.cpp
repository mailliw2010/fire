#include "FireClient.h"
#include <QApplication>
#include<QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FireForm w;
    w.show();


    /*******************use qss file*********************/
     QFile styleFile("://resource/resource/qss/Aqua.qss");
     if(styleFile.open(QIODevice::ReadOnly))
     {
         QString setStyleSheet(styleFile.readAll());
         a.setStyleSheet(setStyleSheet);
         styleFile.close();
         qDebug("Open OK");
     }
     else
     {
         qDebug("Open failed");
     }

    return a.exec();
}
