#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "includes/jsonwallet.h"
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    finde_usb_device();
    chek_device();
    ui->setupUi(this);

    QString path_private_data = get_public_data_path();

    ui->name->setText(get_name_wallet());
    ui->addres->setText(JsonWallet::get_address(path_private_data + "/address.dat.json"));
    ui->public_key->setText(JsonWallet::get_public_key(path_private_data + "/address.dat.json"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chek_device()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);

    while (set_device.empty())
    {
        if (set_device.empty())
            QMessageBox::warning(this, "Message", "Insert USB device!");

        set_device = mon->get_flash_disks(1);
    }

    chec_mark_on_device(get_device_path());
}

void MainWindow::chec_mark_on_device(QString path)
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


void MainWindow::finde_usb_device()
{
    mon = usb_monitor::create();
    mon->on_device_add(device_added);

    mon->mount_existing_devices();
    mon->start();
}

char MainWindow::name_device_;

void MainWindow::device_added(char letter)
{
    name_device_ = letter;
}

QString MainWindow::get_public_data_path()
{
    QDir dir(mark_path_);
    dir.cdUp();
    return dir.path();
}

QString MainWindow::get_name_wallet()
{
    QDir dir(mark_path_);
    dir.cdUp();
    return dir.dirName();
}

QString MainWindow::get_device_path()
{
    QString path;
    path.push_back(name_device_);
    path += ":/";
    return path;
}

void MainWindow::change_window()
{
    transWindow = new MakeTransactionWindow();
    transWindow->setWindowTitle("Online wallet");
    transWindow->show();
}

void MainWindow::on_make_transaction_clicked()
{
    change_window();
    this->close();    // close the main window
}
