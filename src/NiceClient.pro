#-------------------------------------------------
#
# Project created by QtCreator 2019-06-16T19:31:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NiceClient
TEMPLATE = app

CONFIG(debug, debug|release){
TARGET = NiceClient-debug            #debug程序名
BUILD_DIR = ../build/debug
}else {
TARGET = NiceClient                  #release程序名
BUILD_DIR = ../build/release
}
OBJECTS_DIR = $${BUILD_DIR}
MOC_DIR = $${BUILD_DIR}
RCC_DIR = $${BUILD_DIR}
UI_DIR  = $${BUILD_DIR}
DESTDIR = ../bin                    #运行程序目录

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        NetEngine.cpp \
        NetSocket.cpp \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        Def.h \
        MainWindow.h \
        NetEngine.h \
        NetSocket.h

FORMS += \
        MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
