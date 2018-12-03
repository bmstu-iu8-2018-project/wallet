#ifndef INFORMATIONWINDOW_HPP
#define INFORMATIONWINDOW_HPP

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <includes/JsonUtils.hpp>
#include <includes/usb_monitor.hpp>
#include <maketransactionwindow.hpp>

namespace Ui
{
    class InformationWindow;
}

class InformationWindow : public QWidget
{
    Q_OBJECT

 public:
    explicit InformationWindow(QWidget* parent = nullptr);

    ~InformationWindow();

    void set_name(const QString& name);

    static QString get_address();

 private slots:
    void on_make_transaction_clicked();

    void on_exit_clicked();

    void on_boxRequests_currentIndexChanged(int index);

private:
    static QString address_;

    Ui::InformationWindow* ui;

    MakeTransactionWindow* transWindow;

    void init_box_requests();

    void change_window();

    enum class Requests
    {
        address_balance = 1,
        spent_transactions,
        unspent_transactions,
        received_transactions,
        transaction
    };
};

#endif  // INFORMATIONWINDOW_HPP
