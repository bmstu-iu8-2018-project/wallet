#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include "includes/JsonUtils.h"
#include "includes/usb_monitor.h"
#include "maketransactionwindow.h"

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

 private slots:
    void on_make_transaction_clicked();

    void on_exit_clicked();

    void on_request_clicked();

 private:
    Ui::InformationWindow* ui;

    MakeTransactionWindow* transWindow;

    void init_box_requests();

    void change_window();

    struct Requests
    {
        const QString address_balanse = "address balance";
        const QString spent_transactions = "spent transactions";
        const QString unspent_transactions = "unspent transactions";
        const QString received_transactions = "received transactions";
        const QString transaction_inputs = "transaction inputs";
        const QString transaction_outputs = "transaction outputs";
        const QString transaction = "transaction";
    };
};

#endif  // INFORMATIONWINDOW_H
