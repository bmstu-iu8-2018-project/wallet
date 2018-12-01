#include <mainwindow.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto mainWindow = new MainWindow;
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Offline Wallet");
    mainWindow->show();

    return app.exec();
}
