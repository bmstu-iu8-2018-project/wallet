#-------------------------------------------------
#
# Project created by QtCreator 2018-10-25T14:41:58
#
#-------------------------------------------------

QT       += core gui

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
LIBS += -L"C:\boost_1_68_0\libs" -lpsapi
LIBS += -L"C:\boost_1_68_0\stage\lib" -lpsapi
LIBS += -lhid -lsetupapi

INCLUDEPATH += C:\Temp\openssl\include
INCLUDEPATH += C:\boost_1_68_0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    maketransactionwindow.cpp \
    includes/usb_monitor.cpp \
    includes/jsonwallet.cpp \
    includes/CryptoUtils.cpp \
    informationwindow.cpp

HEADERS += \
        mainwindow.h \
    maketransactionwindow.h \
    includes/usb_monitor.h \
    includes/jsonwallet.h \
    includes/CryptoUtils.h \
    informationwindow.h

FORMS += \
        mainwindow.ui \
    maketransactionwindow.ui \
    informationwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
