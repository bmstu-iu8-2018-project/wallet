#include <mainwindow.hpp>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <threadmonitor.h>
#include <messager.h>

// Smart pointer to log file
QScopedPointer<QFile>   m_logFile;

// The implementation of the handler
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Open stream file writes
    QTextStream out(m_logFile.data());
    // Write the date of recording
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Write to the output category of the message and the message itself
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Clear the buffered data
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set the logging file
    // check which a path to file you use
    m_logFile.reset(new QFile(QDir::currentPath() + QDir::separator() + "log.log"));
    // Open the file logging
    m_logFile.data()->open(QFile::WriteOnly | QFile::Text);
    // Set handler
    qInstallMessageHandler(messageHandler);   

    app.setWindowIcon(QIcon(QDir::homePath() + QDir::separator()
                            + "Pictures" + QDir::separator() + "BitCoin.png"));
    auto mainWindow = new MainWindow;
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Online wallet");
    mainWindow->show();

    // Monitiring USb device
    ThreadMonitor * monitor = new ThreadMonitor();
    QThread worker_th;
    worker_th.start();
    monitor->moveToThread(&worker_th);

    Messager msg;
    QObject::connect(&worker_th, SIGNAL(started()), monitor, SLOT(start_find()));
    QObject::connect(&app, SIGNAL(aboutToQuit()), &worker_th, SLOT(quit()));
    QObject::connect(monitor, SIGNAL(usb_not_found()), &msg, SLOT(message()),
                     Qt::BlockingQueuedConnection);
    QObject::connect(monitor, SIGNAL(unknow_usb()), &msg, SLOT(message()),
                     Qt::BlockingQueuedConnection);

    return app.exec();
}
