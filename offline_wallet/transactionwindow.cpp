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

void TransactionWindow::set_transaction(const QString& path)
{
    ui->trasaction->setText(ju::get_json(path));
}

void TransactionWindow::on_clouse_clicked()
{
    this->close();
}
