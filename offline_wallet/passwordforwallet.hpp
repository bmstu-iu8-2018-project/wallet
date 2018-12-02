#ifndef PASSWORDFORWALLET_HPP
#define PASSWORDFORWALLET_HPP

#include <informationwindow.hpp>
#include <includes/OfflineWallet.hpp>
#include <includes/usb_monitor.hpp>
#include <includes/JsonUtils.hpp>
#include <QDir>
#include <QMessageBox>
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
    static QString name_;

    static char name_device_;

    Ui::PasswordForWallet *ui;

    InformationWindow *infWindow;

    usb_monitor* mon;

    void find_usb_device();

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

#endif // PASSWORDFORWALLET_HPP
