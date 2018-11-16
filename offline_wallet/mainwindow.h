#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <informationwindow.h>
#include <passwordforwallet.h>
#include <includes/jsonwallet.h>
#include <QMainWindow>
#include "QMessageBox"
#include <QString>
#include <QtDebug>
#include <QDir>
#include <QDirIterator>

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
    static QString get_wallet_name();

 private slots:
    void on_authorization_clicked();

    void on_go_to_create_wallet_clicked();

private:
    Ui::MainWindow *ui;

    InformationWindow *infWindow;

    PasswordForWallet *passWindow;

    static QString name_;

    void change_window();

    QString load_password(const QString& path);
};

#endif // MAINWINDOW_H
