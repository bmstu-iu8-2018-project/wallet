#include <mainwindow.hpp>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto mainWindow = new MainWindow;
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Online wallet");
    mainWindow->show();

    return app.exec();
}
