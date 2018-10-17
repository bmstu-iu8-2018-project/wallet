#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <informationwindow.h>
#include <passwordforwallet.h>
#include <QMainWindow>
#include "QMessageBox"
#include <QString>
#include <QtDebug>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend class PasswordForWallet;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Authorization_clicked();

    void on_CreateWallet_clicked();

private:
    Ui::MainWindow *ui;
    InformationWindow *infWindow;
    PasswordForWallet *passWindow;
};

#endif // MAINWINDOW_H
