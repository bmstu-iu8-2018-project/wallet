#include "informationwindow.h"
#include "ui_informationwindow.h"
#include "includes/jsonwallet.h"
#include "passwordforwallet.h"

InformationWindow::InformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationWindow)
{
    ui->setupUi(this);

    QString path_private_data = get_path_private_dir();

    ui->name->setText(JsonWallet::get_name(path_private_data + "/authorization_data.json"));
    ui->addres->setText(JsonWallet::get_address(path_private_data + "/address_public_key.json"));
    ui->public_key->setText(JsonWallet::get_public_key(path_private_data + "/address_public_key.json"));
}

QString InformationWindow::get_path_private_dir()
{
    QDir dir(QDir::currentPath());
    dir.cd("Private data");
    dir.cd(PasswordForWallet::get_wallet_name());
    return dir.path();
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
