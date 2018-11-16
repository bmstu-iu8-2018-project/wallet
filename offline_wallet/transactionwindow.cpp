#include "transactionwindow.h"
#include "ui_transactionwindow.h"

TransactionWindow::TransactionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionWindow)
{
    ui->setupUi(this);
}

TransactionWindow::~TransactionWindow()
{
    delete ui;
}

void TransactionWindow::setTransaction(const QString& path)
{
    ui->trasaction->setText(JsonWallet::get_json(path));
}

void TransactionWindow::on_clouse_clicked()
{
    this->close();
}
