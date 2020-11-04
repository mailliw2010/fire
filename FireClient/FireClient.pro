QT       += core gui network
#QT       += mqtt
QT       += serialport
QT       +=webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG   += c++11 (Qt5)
TARGET = FireClient
TEMPLATE = app   # set subdirs: multi projects; set app: single projects

UI_DIR = ../../FireElecMonitor/FireClient  # must have, so that ui file can update every time in shadow mode

LIBS += -L/opt/Qt5.12.0/5.12.0/gcc_64/lib -lQt5Mqtt
INCLUDEPATH += /opt/Qt5.12.0/5.12.0/gcc_64/include/QtMqtt
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

SOURCES += main.cpp\
        globalFun.cpp \
    FireClient.cpp

HEADERS  += \
    globalFun.h \
  #  modbus/rtu/mbcrc.h \
  #  modbus/rtu/mbrtu.h \
    mqttpublic.h \
    FireClient.h

    infomodel.h
FORMS    += \
    FireForm.ui

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
