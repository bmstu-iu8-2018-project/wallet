#include <mainwindow.hpp>
#include <ui_mainwindow.h>
#include <includes/JsonUtils.hpp>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    find_usb_device();
    chek_device();
    ui->setupUi(this);

    ui->wallets->addItem("(select wallet)");
    ui->wallets->addItems(str_list_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

char MainWindow::name_device_;
QString MainWindow::mark_path_;

void MainWindow::change_window()
{
    infWindow = new InformationWindow();
    infWindow->setWindowTitle("Online wallet");
    infWindow->set_name(ui->wallets->currentText());
    infWindow->show();
}

void MainWindow::on_choose_clicked()
{
     if (ui->wallets->currentText() != ui->wallets->itemText(0))
     {
         mark_path_ = get_device_path() + ui->wallets->currentText() + '/';
         change_window();
         this->close();
     }
     else
         QMessageBox::warning(this, "Message", "Select wallet!");

     qDebug() << mark_path_;
}

void MainWindow::device_added(char letter)
{
    name_device_ = letter;
}

void MainWindow::find_usb_device()
{
    mon = usb_monitor::create();
    mon->on_device_add(device_added);

    mon->mount_existing_devices();
    mon->start();
}

QString MainWindow::get_device_path()
{
    QString path;
    path.push_back(name_device_);
    path += ":/";
    return path;
}

QString MainWindow::get_public_data_path()
{
    QDir dir(mark_path_);
    return dir.path();
}

void MainWindow::chek_device()
{
    std::set<wchar_t> set_device = mon->get_flash_disks(1);
    QMessageBox::StandardButton box;
    while (set_device.empty())
    {
        if (set_device.empty())
        {
            box = QMessageBox::warning(this, "Message", "Insert USB device!",
                                            QMessageBox::Ok | QMessageBox::Close);
           // if (box == QMessageBox::Close)
             //   break;
           // else if (box == QMessageBox::Escape)
             //   QApplication::quit();
        }
        set_device = mon->get_flash_disks(1);
    }

    if (box == QMessageBox::Ok)
    {
        mon->stop();
        chec_mark_on_device(get_device_path());
    }
    else if (box == QMessageBox::Close)
    {
        QMessageBox::critical(this, "Message", "Missing USB device!");
        chek_device();
    }
    else
    {
        chec_mark_on_device(get_device_path());
    }
}

void MainWindow::chec_mark_on_device(const QString& path)
{
    bool flag = false;

    QDirIterator it(path, QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "mark.dat")
        {
            flag = true;
            QDir dir(it.filePath());
            dir.cdUp();
            str_list_.push_back(dir.dirName());
        }
    }

    if (!flag)
    {
        QMessageBox::warning(this, "Message", "You inserted an incorrect USB flash drive!\nTry again");
        chek_device();
        chec_mark_on_device(get_device_path());
    }
}
