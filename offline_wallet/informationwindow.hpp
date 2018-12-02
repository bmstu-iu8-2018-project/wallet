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

    void on_exit_clicked();

    void on_view_tx_clicked();

private:
    static QString mark_path_;

    static char name_device_;

    Ui::InformationWindow *ui;

    TransactionWindow *transWindow;

    usb_monitor *mon;

    QFileSystemModel *fs_model_;

    QString get_path_private_dir();

    void finde_usb_device();

    static void device_added(char letter);

    void chek_device();

    void chec_mark_on_device(const QString& path);

    QString get_device_path();

    QDir get_transactions_dir();

    static QString get_name_wallet();

    void change_window(const QDir& dir);

    bool transactions_empty();
};

#endif // INFORMATIONWINDOW_HPP
