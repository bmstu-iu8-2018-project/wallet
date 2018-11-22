#ifndef PASSWORDFORWALLET_H
#define PASSWORDFORWALLET_H

#include "informationwindow.h"
#include "includes/OfflineWallet.h"
#include "includes/usb_monitor.h"
#include <includes/JsonUtils.h>
#include <QDir>
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

    void on_test_network_toggled(bool checked);

    void on_main_network_toggled(bool checked);

    void on_back_clicked();

private:
    Ui::PasswordForWallet *ui;

    InformationWindow *infWindow;

    usb_monitor* mon;

    static QString name_;
    static char name_device_;

    void finde_usb_device();

    static void device_added(const char letter);

    QString create_private_dir(const QString& path);

    void change_window();

    void on_create_wallet();

    void save_wallet_data(const QString& name, const QString& pass);

    void save_authorization_data(const QString& path, const QString& name, const QString& pass);

    void save_private_data(const QString& path, const OfflineWallet& wallet);

    void save_public_data(const QString& path, const OfflineWallet& wallet);

    QString get_public_data_path();

    void mark_device(const QString& fileName);

    bool check_test_network();

    bool check_main_network();

    bool check_network();
};

#endif // PASSWORDFORWALLET_H
