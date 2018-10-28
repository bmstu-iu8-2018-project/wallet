#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maketransactionwindow.h"
#include "includes/usb_monitor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_make_transaction_clicked();

private:
    Ui::MainWindow *ui;
    MakeTransactionWindow *transWindow;
    usb_monitor* mon;

    QString mark_path_;

    static char name_device_;

    void change_window();

    void finde_usb_device();

    void chek_device();

    void chec_mark_on_device(QString path);

    static void device_added(char letter);

    QString get_device_path();

    QString get_name_wallet();

    QString get_public_data_path();
};

#endif // MAINWINDOW_H
