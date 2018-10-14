#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);   
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_info_window()
{
    sWindow = new InformationWindow();
    sWindow->setWindowTitle("Wallet information");
    sWindow->show();
}

void MainWindow::on_Authorization_clicked()
{
    QString login = ui->login->text();
    QString password = ui->pass->text();

    // Name and password match check
    // if true - open window with wallet information

    open_info_window();
    this->close();    // close the main window
}

void MainWindow::on_CreateWallet_clicked()
{
    // QString login = ui->login->text();
    // QString password = ui->pass->text();

    // generate key

    QMessageBox::information(this, "Message", "Wallet successfully created!");

    // Open window with wallet information
    open_info_window();
    this->close();    // close the main window
}
