#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QScopedPointer>
#include <QTextStream>
#include <mainwindow.hpp>

// Smart pointer to log file
QScopedPointer<QFile> m_logFile;

// The implementation of the handler
void messageHandler(QtMsgType type,
                    const QMessageLogContext& context,
                    const QString& msg) {
  // Open stream file writes
  QTextStream out(m_logFile.data());
  // Write the date of recording
  out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
  // By type determine to what level belongs message
  switch (type) {
    case QtInfoMsg:
      out << "INF ";
      break;
    case QtDebugMsg:
      out << "DBG ";
      break;
    case QtWarningMsg:
      out << "WRN ";
      break;
    case QtCriticalMsg:
      out << "CRT ";
      break;
    case QtFatalMsg:
      out << "FTL ";
      break;
  }
  // Write to the output category of the message and the message itself
  out << context.category << ": " << msg << endl;
  out.flush();  // Clear the buffered data
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  // Set the logging file
  // check which a path to file you use
  m_logFile.reset(
      new QFile(QDir::currentPath() + QDir::separator() + "log.log"));
  // Open the file logging
  m_logFile.data()->open(QFile::Append | QFile::Text);
  // Set handler
  qInstallMessageHandler(messageHandler);

  app.setWindowIcon(QIcon(QDir::homePath() + QDir::separator() + "Pictures" +
                          QDir::separator() + "BitCoin.png"));
  auto mainWindow = new MainWindow;
  mainWindow->setAttribute(Qt::WA_DeleteOnClose);
  mainWindow->setWindowTitle("Offline Wallet");
  mainWindow->show();
  return app.exec();
}
