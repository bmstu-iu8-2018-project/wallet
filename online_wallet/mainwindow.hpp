#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QComboBox>
#include <QStringListModel>
#include <informationwindow.hpp>
#include <maketransactionwindow.hpp>
#include <QMessageBox>
#include <QString>
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

    static QString get_public_data_path();

 private slots:
    void on_wallets_currentIndexChanged(int index);

 private:
    static QString mark_path_;

    Ui::MainWindow *ui;

    InformationWindow *infWindow;

    QStringList str_list_;

    void change_window();

    void init_wallets(const QString& path);

    QString get_device_path();
};

#endif // MAINWINDOW_HPP
