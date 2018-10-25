#ifndef PASSWORDFORWALLET_H
#define PASSWORDFORWALLET_H

#include <QWidget>
#include "QMessageBox"
#include "informationwindow.h"

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
    void on_create_pass_clicked();

 private:
    Ui::PasswordForWallet *ui;
    InformationWindow *infWindow;
    static QString name_;

    QString create_private_dir(QString path);
    void change_window();
};

#endif // PASSWORDFORWALLET_H
