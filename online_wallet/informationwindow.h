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

    enum class Requests
    {
        address_balanse = 1,
        spent_transactions,
        unspent_transactions,
        received_transactions,
        transaction_inputs,
        transaction_outputs,
        transaction
    };
};

#endif  // INFORMATIONWINDOW_H
