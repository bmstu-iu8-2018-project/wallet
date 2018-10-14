#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include <informationwindow.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Authorization_clicked();

    void on_CreateWallet_clicked();

private:
    Ui::MainWindow *ui;
    InformationWindow *sWindow;

    void open_info_window();
};

#endif // MAINWINDOW_H
