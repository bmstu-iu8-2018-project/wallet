#include <mainwindow.hpp>
#include <ui_mainwindow.h>
#include <includes/JsonUtils.hpp>
#include <includes/LoggingCategories.hpp>

QString MainWindow::mark_path_;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_wallets(get_device_path());
    qInfo(logInfo()) << "Initialize authorization window";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_window()
{
    infWindow = new InformationWindow();
    infWindow->setWindowTitle("Online wallet");
    infWindow->set_name(ui->wallets->currentText());
    infWindow->show();
}

QString MainWindow::get_device_path()
{
    return QDir::drives().last().dir().path();
}

QString MainWindow::get_public_data_path()
{
    QDir dir(mark_path_);
    return dir.path();
}

void MainWindow::init_wallets(const QString& path)
{
    QDirIterator it(path, QStringList() << "*.dat", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "mark.dat")
        {
            QDir dir(it.filePath());
            dir.cdUp();
            str_list_.push_back(dir.dirName());
        }
    }
    ui->wallets->addItem("(select wallet)");
    ui->wallets->addItems(str_list_);
}

void MainWindow::on_wallets_currentIndexChanged(int index)
{
    if (index != 0)
    {
        mark_path_ = get_device_path() + ui->wallets->currentText() + QDir::separator();
        change_window();
        this->close();
    }
}
