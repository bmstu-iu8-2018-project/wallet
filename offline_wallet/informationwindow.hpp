#pragma once
#ifndef INFORMATIONWINDOW_HPP
#define INFORMATIONWINDOW_HPP

#include <QWidget>
#include <QListView>
#include <QFileSystemModel>
#include <includes/usb_monitor.hpp>
#include <transactionwindow.hpp>

namespace Ui
{
    class InformationWindow;
}

class InformationWindow : public QWidget
{
    Q_OBJECT

 public:
    explicit InformationWindow(QWidget *parent = nullptr);

    ~InformationWindow();

private slots:
    void on_update_trans_clicked();

    void on_transactoin_list_doubleClicked(const QModelIndex &index);

    void on_exit_clicked();

private:
    Ui::InformationWindow *ui;

    TransactionWindow *transWindow;

    usb_monitor *mon;

    QFileSystemModel *fs_model_;

    static QString mark_path_;

    static char name_device_;

    QString get_path_private_dir();

    void finde_usb_device();

    static void device_added(char letter);

    void chek_device();

    void chec_mark_on_device(const QString& path);

    QString get_device_path();

    QString get_transactions_path();

    static QString get_name_wallet();

    void change_window(const QString& path);

    bool transactions_empty();
};

#endif // INFORMATIONWINDOW_HPP
