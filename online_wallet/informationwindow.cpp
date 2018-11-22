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
}

void InformationWindow::set_name(const QString& name)
{
    ui->name->setText(name);
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
