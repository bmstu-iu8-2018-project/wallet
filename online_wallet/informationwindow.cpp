#include "informationwindow.h"
#include "ui_informationwindow.h"
#include <mainwindow.h>

InformationWindow::InformationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWindow)
{
    ui->setupUi(this);

    QString path_private_data = MainWindow::get_public_data_path();

    ui->address->setText(ju::get_address(path_private_data + "/address.dat.json"));
    ui->public_key->setText(ju::get_public_key(path_private_data + "/address.dat.json"));
    ui->balance->setText(ju::get_balance(ui->address->text()));
    init_box_requests();
}

void InformationWindow::set_name(const QString& name)
{
    ui->name->setText(name);
}

void InformationWindow::init_box_requests()
{
    ui->boxRequests->addItem("(select requests)");
    Requests request;
    ui->boxRequests->addItem(request.address_balanse);
    ui->boxRequests->addItem(request.spent_transactions);
    ui->boxRequests->addItem(request.unspent_transactions);
    ui->boxRequests->addItem(request.received_transactions);
    ui->boxRequests->addItem(request.transaction_inputs);
    ui->boxRequests->addItem(request.transaction_outputs);
    ui->boxRequests->addItem(request.transaction);
}

InformationWindow::~InformationWindow()
{
    delete ui;
}

void InformationWindow::change_window()
{
    transWindow = new MakeTransactionWindow();
    transWindow->setWindowTitle("Online wallet");
    transWindow->show();
}

void InformationWindow::on_make_transaction_clicked()
{
    change_window();
}

void InformationWindow::on_exit_clicked()
{
    auto mainWindow = new MainWindow();
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Online Wallet");
    mainWindow->show();
    deleteLater();
    this->close();
}

void InformationWindow::on_request_clicked()
{
    Requests request;
    if (ui->boxRequests->currentText() == "(select requests)")
    {
        QMessageBox::warning(this, "Message", "Select requests!");
    }
    else if (ui->boxRequests->currentText() == request.address_balanse)
    {
        ui->requestBrowser->clear();
        const auto balance_json_data = nu::get_address_balance(ui->address->text().toStdString());
        ui->requestBrowser->setText(balance_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.spent_transactions)
    {
        ui->requestBrowser->clear();
        const auto spend_trans_json_data = nu::get_spent_transactions(ui->address->text().toStdString());
        ui->requestBrowser->setText(spend_trans_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.unspent_transactions)
    {
        ui->requestBrowser->clear();
        const auto unspend_trans_json_data = nu::get_unspent_transactions(ui->address->text().toStdString());
        ui->requestBrowser->setText(unspend_trans_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.received_transactions)
    {
        ui->requestBrowser->clear();
        const auto received_trans_json_data = nu::get_received_transactions(ui->address->text().toStdString());
        ui->requestBrowser->setText(received_trans_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.transaction_inputs)
    {
        ui->requestBrowser->clear();
       // const auto input_trans_json_data = nu::get_transaction_inputs("txid");
       // ui->requestBrowser->setText(input_trans_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.transaction_outputs)
    {
        ui->requestBrowser->clear();
       // const auto output_trans_json_data = nu::get_transaction_outputs("txid");
       // ui->requestBrowser->setText(output_trans_json_data.c_str());
    }
    else if (ui->boxRequests->currentText() == request.transaction)
    {
        ui->requestBrowser->clear();
       // const auto trans_json_data = nu::get_transaction("txid");
       // ui->requestBrowser->setText(trans_json_data.c_str());
    }
}
