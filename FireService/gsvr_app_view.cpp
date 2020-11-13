#include "gsvr_app_view.h"
#include "gsvr_app_server.h"
#include "gsvr_app_config.h"
#include "ui_main_win.h"

#include <QHBoxLayout>
#include "globalFun.h"

extern QMutex uart_mutex;

View::View(QWidget* parent):
    QMainWindow(parent),
    ui(new Ui::View)
{
    qDebug() << "Constructing View";
    setting = new ConfigWin(this, this);  // new ConfigWin after gmqtt! config use class mqtt to config mqtt


}

void View::initialize()
{
    qDebug() << "Initializing View";
    this->setting->initialize();
    ShowWindow();

}


ConfigWin* View::getSetobj()
{
    return setting;
}



void View::ShowWindow()
{
    ui->setupUi(this);  //Initial, ui
    /****Initial Window Profile******/
    qDebug() << "Initial View Profile...";
    this->grabKeyboard();     // make key always focus in mainwindow! this will diable setting.
    this->setContentsMargins(0,0,0,0);
    this->setWindowTitle("电气火灾监控系统");



if(setting->para->debugmode)
{
    qDebug() << "Entering Debug Mode...";
    this->ui->menuBar->setVisible(true);

}
else
{
    qDebug() << "Entering FullScreen Mode...";
    this->setWindowFlags(Qt::FramelessWindowHint);    //hide window, max/min button
}

    this->showFullScreen();    // control this to show full screen!
    /********webengine***************/
    qDebug() << "Initial Web Engine...";
    QWidget *widget = new QWidget(this);
    ui->m_webView->setParent(widget);
    setCentralWidget(widget);
    QHBoxLayout *windowLayout = new QHBoxLayout(this);
    windowLayout->setContentsMargins(0,0,0,0);
    windowLayout->addWidget(ui->m_webView);
    widget->setLayout(windowLayout);
if(setting->para->debugmode)
    this->menuBar()->show();
else
    this->menuBar()->hide();


    QUrl url = QUrl::fromLocalFile(setting->para->UrlName);
    ui->m_webView->load(QUrl(url));

    /**Initial event**/
    connect(ui->actionSettings,&QAction::triggered, this, &View::actionsettings_clicked);     //settings assigned parameters.
 //   connect(ui->actionCapture, &QAction::triggered, this, &View::actioncapture_clicked);
    connect(ui->actionQuit, &QAction::triggered, this, &View::actionQuit_clicked);
  //  connect(ui->actionTestButton, &QAction::triggered, this, &View::actionTestButton_clicked);



}


void View::actionsettings_clicked()
{

    qDebug() << "Opening Setting Window";
    this->releaseKeyboard();
    setting->setModal(true);
    setting->showNormal();


}



void View::actionQuit_clicked()
{
    QApplication::quit();  // use this to replace exit
}

//void View::updateLogStateChange()
//{
//    // set contant type
//    const QString content = QDateTime::currentDateTime().toString()
//                    + QLatin1String(": State Change")
//                    + QString::number(m->clientLocal->state())
//                    + QLatin1Char('\n');
//    setting->ui->editLog->insertPlainText(content);  //show contents
//}


/**useless without grabKeyboard*/
void View::keyPressEvent(QKeyEvent *event)
{
    // 捕捉 Ctrl + Alt + A 组合键;
       if (event->key() == Qt::Key_C  &&  event->modifiers() == Qt::ControlModifier)    // quit Application
       {

            QMutexLocker locker(&uart_mutex);
           qInfo() << "Quitting FireService Gracefully!\n";
           QApplication::quit();
       }




}
