#include <passwordforwallet.hpp>
#include <ui_passwordforwallet.h>
#include <mainwindow.hpp>

QString PasswordForWallet::name_;
char PasswordForWallet::name_device_;

PasswordForWallet::PasswordForWallet(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordForWallet)
{
    ui->setupUi(this);
    find_usb_device();
}

PasswordForWallet::~PasswordForWallet()
{
    delete ui;
}

void PasswordForWallet::find_usb_device()
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

QString PasswordForWallet::create_private_dir(const QString& name)
{
    QDir dir(QDir::homePath() + QDir::separator() + "Private data");
    if (!dir.exists())
        dir.mkpath(".");

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

void PasswordForWallet::save_authorization_data(
        const QString& path, const QString& name, const QString& pass)
{
    QVariantMap json_authorization_data_map;
    json_authorization_data_map.insert("name", name);
    json_authorization_data_map.insert("password", pass);
    ju::record_to_json(json_authorization_data_map,
                           path + QDir::separator() + "authorization_data.json");
}

void PasswordForWallet::save_private_data(
        const QString& path, const OfflineWallet& wallet)
{
    QVariantMap json_private_map;
    json_private_map.insert("private_key",
                            QString::fromUtf8(wallet.get_private_key().c_str()));

    ju::record_to_json(json_private_map, path + QDir::separator() + "wallet.dat.json");
}

QString PasswordForWallet::get_public_data_path()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);

    while (set_device.empty())
    {
        if (set_device.empty())
            QMessageBox::warning(this, "Message", "Insert USB device!");

        set_device = mon->get_flash_disks(1);
    }

    return QDir::drives().last().dir().path();
}

void PasswordForWallet::save_public_data(
        const QString& path, const OfflineWallet& wallet)
{
    // get path for save public_key and address
    QString path_addres_public = get_public_data_path();

    QVariantMap json_addres_public_map;
    json_addres_public_map.insert("address",
                                  QString::fromUtf8(wallet.get_address().c_str()));
    json_addres_public_map.insert("public_key",
                                   QString::fromUtf8(wallet.get_public_key().c_str()));
    ju::record_to_json(json_addres_public_map, path_addres_public + QDir::separator() + "address.dat.json");
    ju::record_to_json(json_addres_public_map, path + QDir::separator() + "address_public_key.json");

    mark_device(path_addres_public + QDir::separator() + "mark.dat");

    QDir dir(path_addres_public);
    dir.mkdir("Transactions");
}

void PasswordForWallet::mark_device(const QString& fileName)
{
    QFile mark(fileName);
    if( mark.open( QIODevice::WriteOnly ) )
    {
        QTextStream stream( &mark );
        stream << QObject::trUtf8( "master device" );
        mark.close();
    }
}

bool PasswordForWallet::check_test_network()
{
    if (ui->test_network->isChecked())
        return true;

    return false;
}

bool PasswordForWallet::check_main_network()
{
    if (ui->main_network->isChecked())
        return true;

    return false;
}

bool PasswordForWallet::check_network()
{
    if (check_test_network() || check_main_network())
        return true;

    return false;
}

void PasswordForWallet::save_wallet_data(
        const QString& name, const QString& pass)
{
    QString path_private_data = create_private_dir(name);

    save_authorization_data(path_private_data, name, pass);

    // generate key
    OfflineWallet  wallet;

    if (check_test_network())
    {
        wallet.create_wallet(TEST_NETWORK);
    }
    else if (check_main_network())
    {
        wallet.create_wallet(MAIN_NETWORK);
    }

    // save private key on offline device
    save_private_data(path_private_data, wallet);
    save_public_data(path_private_data, wallet);

    QMessageBox::information(this, "Message", "Wallet successfully created!");

    // Open window with wallet information
    change_window();
}

void PasswordForWallet::on_create_wallet()
{
    name_ = ui->lineEdit_name->text();

    if (ui->lineEdit_pass1->text() == ui->lineEdit_pass2->text())
    {

        if (ui->lineEdit_name->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Enter login!");
        }
        else if (ui->lineEdit_pass1->text().isEmpty() &&
                 ui->lineEdit_pass2->text().isEmpty())
        {
            QMessageBox::warning(this, "Error", "Enter password!");
        }
        else if (check_network())
        {
            save_wallet_data(ui->lineEdit_name->text(), ui->lineEdit_pass1->text());
            this->close();    // close the main window
        }
        else
        {
            QMessageBox::warning(this, "Error", "Select the type of wallet!");
        }
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

void PasswordForWallet::on_test_network_toggled(bool checked)
{
    if(checked)
        ui->main_network->setEnabled(false);
    else
        ui->main_network->setEnabled(true);
}

void PasswordForWallet::on_main_network_toggled(bool checked)
{
    if(checked)
        ui->test_network->setEnabled(false);
    else
        ui->test_network->setEnabled(true);
}

void PasswordForWallet::on_back_clicked()
{
    auto mainWindow = new MainWindow();
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Offline Wallet");
    mainWindow->show();
    deleteLater();
    this->close();
}
