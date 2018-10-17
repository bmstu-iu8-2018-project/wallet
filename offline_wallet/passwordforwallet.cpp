#include "passwordforwallet.h"
#include "ui_passwordforwallet.h"
#include "mainwindow.h"
#include "jsonwallet.h"
#include "includes/OfflineWallet.h"

PasswordForWallet::PasswordForWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordForWallet)
{
    ui->setupUi(this);
}

PasswordForWallet::~PasswordForWallet()
{
    delete ui;
}

void PasswordForWallet::on_create_pass_clicked()
{
    QString password1 = ui->lineEdit_pass1->text();
    QString password2 = ui->lineEdit_pass2->text();
    QString name = ui->lineEdit_name->text();

    if (password1 == password2)
    {
        QString path_private_data = "C:/Users/Sergei/Documents/Private data/";

        QVariantMap json_authorization_data_map;
        json_authorization_data_map.insert("name", name);
        json_authorization_data_map.insert("password", password1);
        JsonWallet::recordJson(json_authorization_data_map,
                               path_private_data + "authorization_data.json");

        // generate key
        OfflineWallet  wallet;
        wallet.create_wallet();

        // save private key on offline device
        QVariantMap json_private_map;
        json_private_map.insert("private_key",
                                QString::fromUtf8(wallet.get_private_key().c_str()));

        JsonWallet::recordJson(json_private_map, path_private_data + "private_key.json");

        // get path for save public_key and address
        QString path_addres_public = QFileDialog::getSaveFileName(this,
                                                      tr("Save file"), "data",
                                                      tr("Data file (*.json)") );
        QVariantMap json_addres_public_map;
        json_addres_public_map.insert("address",
                                      QString::fromUtf8(wallet.get_address().c_str()));
        json_addres_public_map.insert("public_key",
                                      QString::fromUtf8(wallet.get_public_key().c_str()));
        JsonWallet::recordJson(json_addres_public_map, path_addres_public);

        QMessageBox::information(this, "Message", "Wallet successfully created!");

        // Open window with wallet information
        infWindow = new InformationWindow();
        infWindow->setWindowTitle("Wallet information");
        infWindow->show();

        this->close();    // close the main window
    }
    else
    {
        QMessageBox::warning(this, "Error", "The keys do not match, try again!");
    }
}
