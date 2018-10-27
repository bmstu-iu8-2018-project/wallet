#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    finde_usb_device();
    chek_device();
    ui->setupUi(this);
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

    chec_mark_on_device();
}

void MainWindow::chec_mark_on_device()
{
    QString path = get_public_data_path();

    QDir currentFolder(path);

    currentFolder.setFilter(QDir::Files);
    currentFolder.setSorting(QDir::Name);

    QFileInfoList folderitems(currentFolder.entryInfoList());
    bool flag = false;
    foreach (QFileInfo i_file, folderitems)
    {
        QString i_filename(i_file.fileName());
        if (i_filename == "." || i_filename == ".." || i_filename.isEmpty())
            continue;

        if (i_filename == "mark.dat")
        {
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        QMessageBox::warning(this, "Message", "You inserted an incorrect USB flash drive!\nTry again");
        chek_device();
        chec_mark_on_device();
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
