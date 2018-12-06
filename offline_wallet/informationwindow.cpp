#include <informationwindow.hpp>
#include <mainwindow.hpp>
#include <passwordforwallet.hpp>
#include <ui_informationwindow.h>
#include <includes/JsonUtils.hpp>
#include <includes/LoggingCategories.hpp>

QString InformationWindow::mark_path_;

InformationWindow::InformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationWindow)
{
    qInfo(logInfo()) << "Initialize information window";
    ui->setupUi(this);

    fs_model_ = new QFileSystemModel(this);
    fs_model_->setFilter(QDir::Files | QDir::Dirs);

    QString path_private_data = get_path_private_dir();

    ui->name->setText(ju::get_name(path_private_data + QDir::separator() + "authorization_data.json"));
    ui->addres->setText(ju::get_address(path_private_data + QDir::separator() + "address_public_key.json"));
    ui->public_key->setText(ju::get_public_key(path_private_data + QDir::separator() + "address_public_key.json"));
    qInfo(logInfo()) << "Wallet name: " << ui->name->text();
    qInfo(logInfo()) << "Wallet address: " << ui->addres->text();
}

QString InformationWindow::get_path_private_dir()
{    
    QDir dir(QDir::homePath() + QDir::separator() + "Private data");

    if (!MainWindow::get_wallet_name().isEmpty() && PasswordForWallet::get_wallet_name().isEmpty())
        dir.cd(MainWindow::get_wallet_name());
    else if (MainWindow::get_wallet_name().isEmpty() && !PasswordForWallet::get_wallet_name().isEmpty())
        dir.cd(PasswordForWallet::get_wallet_name());
    else
        dir.cd(MainWindow::get_wallet_name());

    return dir.path();
}

InformationWindow::~InformationWindow()
{
    delete ui;
}

void InformationWindow::finde_usb_device()
{
    mon = usb_monitor::create();
    mon->mount_existing_devices();
    mon->start();
    qInfo(logInfo()) << "Find usb device ...";
}

bool InformationWindow::chek_device()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);

    if (set_device.empty())
        QMessageBox::warning(this, "Message", "Insert USB device!");
    else
        if (chec_mark_on_device(get_device_path()))
            return true;
        else
        {
            QMessageBox::warning(this, "Message",
                                 "You inserted an incorrect USB flash drive!\nTry again");
            qWarning(logWarning()) << "Inserted an incorrect USB flash drive";
        }

    mon->mount_existing_devices();
    qInfo(logInfo()) << "Usb device found" << QDir::drives().last().dir().Name;
    return false;
}

bool InformationWindow::chec_mark_on_device(const QString& path)
{
    QDirIterator it(path, QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "mark.dat")
        {
            mark_path_ = it.filePath();
            if (get_name_wallet() ==  ui->name->text())
                return true;
        }
    }
    return false;
}

QString InformationWindow::get_name_wallet()
{
    QDir dir(mark_path_);
    dir.cdUp();
    return dir.dirName();
}

QString InformationWindow::get_device_path()
{
    return QDir::drives().last().dir().path();
}

QDir InformationWindow::get_transactions_dir()
{
    QDir dir(mark_path_);
    dir.cdUp();
    dir.cd("Transactions");
    return dir;
}

bool InformationWindow::transactions_empty()
{
    QDir dir(get_transactions_dir());
    if (dir.isEmpty())
        return true;

    return false;
}

void InformationWindow::change_window(const QDir& dir)
{
    transWindow = new TransactionWindow();
    transWindow->setWindowTitle("Transaction");
    transWindow->setTxIdBox(dir);
    transWindow->show();
}

void InformationWindow::on_exit_clicked()
{
    auto mainWindow = new MainWindow();
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Offline Wallet");
    mainWindow->show();
    deleteLater();
    this->close();
}

void InformationWindow::on_view_tx_clicked()
{
    finde_usb_device();
    if (chek_device())
        change_window(get_transactions_dir());
}
