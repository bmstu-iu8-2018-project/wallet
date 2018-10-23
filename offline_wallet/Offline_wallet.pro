#-------------------------------------------------
#
# Project created by QtCreator 2018-10-07T16:10:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Offline_wallet
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

INCLUDEPATH += C:\Temp\openssl\include

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    informationwindow.cpp \
    passwordforwallet.cpp \
    jsonwallet.cpp \
    includes/base58.cpp

HEADERS += \
        mainwindow.h \
    informationwindow.h \
    passwordforwallet.h \
    jsonwallet.h \
    includes/allocators.h \
    includes/base58.h \
    includes/Crypto.h \
    includes/hash.h \
    includes/Key.h \
    includes/OfflineWallet.h

FORMS += \
        mainwindow.ui \
    informationwindow.ui \
    passwordforwallet.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target