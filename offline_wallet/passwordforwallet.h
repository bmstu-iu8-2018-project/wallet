#ifndef PASSWORDFORWALLET_H
#define PASSWORDFORWALLET_H

#include "informationwindow.h"
#include "includes/OfflineWallet.h"
#include "includes/usb_monitor.h"
#include <sstream>
#include <windows.h>
#include "QMessageBox"
#include <QWidget>

namespace Ui
{
    class PasswordForWallet;
}

class PasswordForWallet : public QWidget
{
    Q_OBJECT

 public:
    explicit PasswordForWallet(QWidget *parent = nullptr);
    ~PasswordForWallet();

    static QString get_wallet_name();

 private slots:
    void on_create_wallet_clicked();

 private:
    Ui::PasswordForWallet *ui;

    InformationWindow *infWindow;

    usb_monitor* mon;

    static QString name_;
    static char name_device_;

    void finde_usb_device();

    static void device_added(char letter);

    QString create_private_dir(QString path);

    void change_window();

    void on_create_wallet();

    void save_wallet_data(QString name, QString pass);

    void save_authorization_data(QString path, QString name, QString pass);

    void save_private_data(QString path, const OfflineWallet& wallet);

    void save_public_data(QString path, const OfflineWallet& wallet);

    QString get_public_data_path();

};

#endif // PASSWORDFORWALLET_H
