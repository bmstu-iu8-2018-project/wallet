#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QStringListModel>
#include <informationwindow.h>
#include "maketransactionwindow.h"
#include "includes/usb_monitor.h"

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

    static QString get_public_data_path();

private slots:
    void on_choose_clicked();

private:
    Ui::MainWindow *ui;

    usb_monitor *mon;

    InformationWindow *infWindow;

    static QString mark_path_;

    static char name_device_;

    QStringList str_list_;

    void change_window();

    void find_usb_device();

    void chek_device();

    void chec_mark_on_device(const QString& path);

    static void device_added(char letter);

    QString get_device_path();
};

#endif // MAINWINDOW_H
