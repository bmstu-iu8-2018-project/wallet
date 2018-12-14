#-------------------------------------------------
#
# Project created by QtCreator 2018-10-07T16:10:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Offline_wallet

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += BOOST_LOG_DYN_LINK

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

INCLUDEPATH += C:\Temp\openssl\include
INCLUDEPATH += C:\boost_1_68_0\boost_mingw_53_32\include\boost-1_68

LIBS += C:\Temp\openssl\lib\libeay32.lib
LIBS += C:\Temp\openssl\lib\ssleay32.lib

LIBS += "-LC:/boost_1_68_0/boost_mingw_53_32/lib" \
                -llibboost_system-mgw53-mt-d-x32-1_68.dll

LIBS += -lhid -lsetupapi -lws2_32

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    informationwindow.cpp \
    passwordforwallet.cpp \
    includes/CryptoUtils.cpp \
    includes/Key.cpp \
    includes/OfflineWallet.cpp \
    includes/usb_monitor.cpp \
    transactionwindow.cpp \
    includes/JsonUtils.cpp \
    includes/http_client_async_ssl.cpp \
    includes/NetworkUtils.cpp \
    includes/Script.cpp \
    includes/Transaction.cpp \
    includes/TxIn.cpp \
    includes/TxOut.cpp \
    includes/LoggingCategories.cpp

HEADERS += \
    includes/CryptoUtils.hpp \
    includes/http_client_async_ssl.hpp \
    includes/JsonUtils.hpp \
    includes/Key.hpp \
    includes/NetworkUtils.hpp \
    includes/OfflineWallet.hpp \
    includes/usb_monitor.hpp \
    informationwindow.hpp \
    mainwindow.hpp \
    passwordforwallet.hpp \
    transactionwindow.hpp \
    includes/Script.hpp \
    includes/Transaction.hpp \
    includes/TxIn.hpp \
    includes/TxOut.hpp \
    includes/LoggingCategories.hpp

FORMS += \
        mainwindow.ui \
    informationwindow.ui \
    passwordforwallet.ui \
    transactionwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
