#include "passwordforwallet.h"
#include "ui_passwordforwallet.h"
#include "mainwindow.h"
#include "jsonwallet.h"
#include <QDir>

PasswordForWallet::PasswordForWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordForWallet)
{
    ui->setupUi(this);
    finde_usb_device();
}

QString PasswordForWallet::name_;
char PasswordForWallet::name_device_;

PasswordForWallet::~PasswordForWallet()
{
    delete ui;
}

void PasswordForWallet::finde_usb_device()
{
    mon = usb_monitor::create();
    mon->on_device_add(device_added);

    mon->mount_existing_devices();
    mon->start();
}

void PasswordForWallet::device_added(char letter)
{
    name_device_ = letter;
}

QString PasswordForWallet::create_private_dir(QString name)
{
    QDir dir(QDir::currentPath());
    dir.mkdir("Private data");
    dir.cd("Private data");
    dir.mkdir(name);
    dir.cd(name);
    return dir.path();
}

QString PasswordForWallet::get_wallet_name()
{
    return name_;
}

void PasswordForWallet::change_window()
{
    infWindow = new InformationWindow();
    infWindow->setWindowTitle("Wallet information");
    infWindow->show();
}

void PasswordForWallet::save_authorization_data(QString path, QString name, QString pass)
{
    QVariantMap json_authorization_data_map;
    json_authorization_data_map.insert("name", name);
    json_authorization_data_map.insert("password", pass);
    JsonWallet::record_to_json(json_authorization_data_map,
                           path + "/authorization_data.json");
}

void PasswordForWallet::save_private_data(QString path, const OfflineWallet& wallet)
{
    QVariantMap json_private_map;
    json_private_map.insert("private_key",
                            QString::fromUtf8(wallet.get_private_key().c_str()));

    JsonWallet::record_to_json(json_private_map, path + "/wallet.dat.json");
}

QString PasswordForWallet::get_public_data_path()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);

    if (set_device.empty())
        QMessageBox::information(this, "Message", "Insert USB device!");

    QString path;
    path.push_back(name_device_);
    path += ":/";
    return path;
}

void PasswordForWallet::save_public_data(QString path, const OfflineWallet& wallet)
{
    // get path for save public_key and address
    QString path_addres_public = get_public_data_path();

    QVariantMap json_addres_public_map;
    json_addres_public_map.insert("address",
                                  QString::fromUtf8(wallet.get_address().c_str()));
    json_addres_public_map.insert("public_key",
                                  QString::fromUtf8(wallet.get_public_key().c_str()));
    JsonWallet::record_to_json(json_addres_public_map, path_addres_public + "address.dat.json");
    JsonWallet::record_to_json(json_addres_public_map, path + "/address_public_key.json");
}


void PasswordForWallet::save_wallet_data(QString name, QString pass)
{
    QString path_private_data = create_private_dir(name);

    save_authorization_data(path_private_data, name, pass);

    // generate key
    OfflineWallet  wallet;
    wallet.create_wallet();

    // save private key on offline device
    save_private_data(path_private_data, wallet);

    save_public_data(path_private_data, wallet);

    QMessageBox::information(this, "Message", "Wallet successfully created!");

    // Open window with wallet information
    change_window();
}


void PasswordForWallet::on_create_wallet()
{
    QString password1 = ui->lineEdit_pass1->text();
    QString password2 = ui->lineEdit_pass2->text();
    QString name = ui->lineEdit_name->text();
    name_ = name;

    if (password1 == password2)
    {
        save_wallet_data(name, password1);
        this->close();    // close the main window
    }
    else
    {
        QMessageBox::warning(this, "Error", "The keys do not match, try again!");
    }
}

void PasswordForWallet::on_create_wallet_clicked()
{
    on_create_wallet();
}

