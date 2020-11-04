QT       += core gui network
#QT       += mqtt
QT       += serialport
QT       +=webenginewidgets
QT       +=concurrent
QT       +=sql
RC_ICONS = mylogo.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += console c++11   # print to windows console
CONFIG   += c++11 (Qt5)
DEFINES += QT_MESSAGELOGCONTEXT    # add this to debug message:File and Line

#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_INFO_OUTPUT


if(contains(DEFINES, PLATFORM_WIN)){

DEFINES += WIN32_LEAN_AND_MEAN  #解决文件包含顺序错误问题，这两个头文件是：#include <winsock2.h>和#include<windows.h>

}


TARGET = FireService
TEMPLATE = app   # set subdirs: multi projects; set app: single projects

#UI_DIR = ../../FireElecMonitor/FireService  # must have, so that ui file can update every time in shadow mode


LIBS += -L/opt/Qt5.12.0/5.12.0/gcc_64/lib -lQt5Mqtt
#INCLUDEPATH += /opt/Qt5.12.0/5.12.0/gcc_64/include   not need
INCLUDEPATH += /opt/Qt5.12.0/5.12.0/gcc_64/src/mqtt

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please cons
#ult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=\
        globalFun.cpp \
        modbus/modbus-data.c \
        modbus/modbus-rtu.c \
        modbus/modbus.c \
    gsvr_app_main.cpp \
    gsvr_app_server.cpp \
    gsvr_app_config.cpp \
    gsvr_app_public.cpp \
    gsvr_thread_listen.cpp \
    gsvr_thread_report.cpp \
    gsvr_app_mqtt.cpp \
    gsvr_app_modbus.cpp \
    gsvr_info_break.cpp \
    gsvr_app_log.cpp \
    gsvr_app_view.cpp

HEADERS  += \
    globalFun.h \
    modbus/config.h \
    modbus/modbus-private.h \
    modbus/modbus-rtu-private.h \
    modbus/modbus-rtu.h \
    modbus/modbus-tcp-private.h \
    modbus/modbus-version.h \
    modbus/modbus.h \
  #  modbus/rtu/mbcrc.h \
  #  modbus/rtu/mbrtu.h \
    gsvr_app_server.h \
    gsvr_app_config.h \
    gsvr_app_public.h \
    gsvr_thread_listen.h \
    gsvr_thread_report.h \
    gsvr_app_mqtt.h \
    gsvr_app_modbus.h \
    gsvr_info_break.h \
    gsvr_app_log.h \
    gsvr_app_view.h

    infomodel.h
FORMS    += \
    config_win.ui \
    main_win.ui

target.path = $$[QT_INSTALL_EXAMPLES]/mqtt/simpleclient
INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    js/echarts.js \
    js/line-tooltip-touch.html \
    js/index.html \
    js/index.html \
    js/line-smooth.html \
    js/line-simple.html \
    js/line-simple.html \
    js/test.html \
    js/line-gradient.html \
    js/dataset-encode1.html \
    README.md
