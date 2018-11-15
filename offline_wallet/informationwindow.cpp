#include "informationwindow.h"
#include "ui_informationwindow.h"
#include "mainwindow.h"
#include "includes/jsonwallet.h"
#include "passwordforwallet.h"

InformationWindow::InformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationWindow)
{
    ui->setupUi(this);

    fs_model_ = new QFileSystemModel(this);
    fs_model_->setFilter(QDir::Files | QDir::Dirs);

    QString path_private_data = get_path_private_dir();

    ui->name->setText(JsonWallet::get_name(path_private_data + "/authorization_data.json"));
    ui->addres->setText(JsonWallet::get_address(path_private_data + "/address_public_key.json"));
    ui->public_key->setText(JsonWallet::get_public_key(path_private_data + "/address_public_key.json"));
}

QString InformationWindow::mark_path_;
char InformationWindow::name_device_;

QString InformationWindow::get_path_private_dir()
{
    QDir dir(QDir::currentPath());
    dir.cd("Private data");

    if (PasswordForWallet::get_wallet_name().isEmpty())
        dir.cd(MainWindow::get_wallet_name());
    else
        dir.cd(PasswordForWallet::get_wallet_name());

    return dir.path();
}

InformationWindow::~InformationWindow()
{
    delete ui;
}

void InformationWindow::finde_usb_device()
{
    mon = usb_monitor::create();
    mon->on_device_add(device_added);

    mon->mount_existing_devices();
    mon->start();
}

void InformationWindow::device_added(char letter)
{
    name_device_ = letter;
}

void InformationWindow::chek_device()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);

    while (set_device.empty())
    {
        if (set_device.empty())
            QMessageBox::warning(this, "Message", "Insert USB device!");

        set_device = mon->get_flash_disks(1);
    }
    mon->stop();
    chec_mark_on_device(get_device_path());
}

void InformationWindow::chec_mark_on_device(QString path)
{
    bool flag = false;

    QDirIterator it(path, QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "mark.dat")
        {
            flag = true;
            mark_path_ = it.filePath();
            break;
        }
    }

    if (!flag)
    {
        QMessageBox::warning(this, "Message", "You inserted an incorrect USB flash drive!\nTry again");
        chek_device();
        chec_mark_on_device(get_device_path());
    }

}

QString InformationWindow::get_name_wallet()
{
    QDir dir(mark_path_);
    dir.cdUp();
    return dir.dirName();
}

QString InformationWindow::get_device_path()
{
    QString path;
    path.push_back(name_device_);
    path += ":/";
    return path;
}

QString InformationWindow::get_transactions_path()
{
    QString path;
    QDir dir(mark_path_);
    dir.cdUp();
    path = dir.path();
    path += "/Transactions/";
    return path;
}

void InformationWindow::on_sing_transaction_clicked()
{

}

void InformationWindow::on_update_trans_clicked()
{
    finde_usb_device();
    chek_device();
    if (get_name_wallet() == ui->name->text())
    {
        fs_model_->setRootPath(get_device_path());
        ui->transactoin_list->setModel(fs_model_);
    }
    else
    {
        QMessageBox::warning(this, "Message", "Bad");
    }
}

void InformationWindow::on_transactoin_list_doubleClicked(const QModelIndex &index)
{
    QListView *list_view = (QListView*)sender();
    QFileInfo file_info = fs_model_->fileInfo(index);
    if (file_info.fileName() == "..")
    {
        QDir dir = file_info.dir();
        dir.cdUp();
        list_view->setRootIndex(fs_model_->index(dir.absolutePath()));
    }
    else if (file_info.fileName() == ".")
    {
        list_view->setRootIndex(fs_model_->index(get_transactions_path()));
    }
    else if (file_info.isDir())
    {
        list_view->setRootIndex(index);
    }
}
