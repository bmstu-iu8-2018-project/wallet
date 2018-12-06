#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <informationwindow.hpp>
#include <passwordforwallet.hpp>
#include <includes/JsonUtils.hpp>
#include <QMainWindow>
#include <QMessageBox>
#include <QtAlgorithms>
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
    static QString name_;

    Ui::MainWindow *ui;

    InformationWindow *infWindow;

    PasswordForWallet *passWindow;

    void change_window();

    QString load_password(const QString& path);
};

#endif // MAINWINDOW_HPP
