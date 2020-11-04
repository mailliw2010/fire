#ifndef GSVR_APP_VIEW_H
#define GSVR_APP_VIEW_H


#include <QMainWindow>
//#include "gsvr_app_config.h"
//#include "gsvr_app_server.h"

namespace Ui {    // unique namespace Ui, Ui::View is for ui
class View;
}

class ConfigWin;
class MainServer;
class View: public QMainWindow
{
    Q_OBJECT

public:
    friend class MainServer;
    explicit View(QWidget* parent = nullptr);
    void initialize();
    void ShowWindow();

    ConfigWin* getSetobj();

protected:
    void keyPressEvent(QKeyEvent* event);

private slots:
    void actionQuit_clicked();
//    void updateLogStateChange();
//    void brokerDisconnected();
    void actionsettings_clicked();

//    void actioncapture_clicked();
//    void actionTestButton_clicked();

private:
    Ui::View *ui;
    MainServer* m;
    ConfigWin *setting;  //all serial object


};



#endif // GSVR_APP_VIEW_H
