#-------------------------------------------------
#
# Project created by QtCreator 2018-10-25T14:41:58
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Online_wallet
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

LIBS += C:\Temp\openssl\lib\libeay32.lib
LIBS += C:\Temp\openssl\lib\ssleay32.lib

LIBS += "-LC:/boost_1_68_0/boost_mingw_53_32/lib" \
                -llibboost_system-mgw53-mt-x32-1_68.dll

LIBS += -lhid -lsetupapi -lws2_32

INCLUDEPATH += C:\Temp\openssl\include
INCLUDEPATH += C:\boost_1_68_0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    maketransactionwindow.cpp \
    includes/usb_monitor.cpp \
    includes/CryptoUtils.cpp \
    informationwindow.cpp \
    includes/http_client_async_ssl.cpp \
    includes/NetworkUtils.cpp \
    includes/JsonUtils.cpp

HEADERS += \
    includes/CryptoUtils.hpp \
    includes/http_client_async_ssl.hpp \
    includes/JsonUtils.hpp \
    includes/NetworkUtils.hpp \
    includes/usb_monitor.hpp \
    informationwindow.hpp \
    mainwindow.hpp \
    maketransactionwindow.hpp

FORMS += \
        mainwindow.ui \
    maketransactionwindow.ui \
    informationwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
