#include "informationwindow.h"
#include "ui_informationwindow.h"
#include "jsonwallet.h"

InformationWindow::InformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationWindow)
{
    ui->setupUi(this);

    QString path_private_data = "C:/Users/Sergei/Documents/Private data/";

    ui->name->setText(JsonWallet::get_name(path_private_data + "authorization_data.json"));
    ui->addres->setText(JsonWallet::get_address(path_private_data + "address_public_key.json"));
    ui->public_key->setText(JsonWallet::get_address(path_private_data + "address_public_key.json"));
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
