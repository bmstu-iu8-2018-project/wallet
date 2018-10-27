#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_authorization_clicked()
{
    QString login = ui->login->text();
    QString password = ui->pass->text();

    QString login_of_file;
    QString password_of_file;

    if ((login == login_of_file) && (password == password_of_file))
    {
        // Open window with wallet information
        infWindow = new InformationWindow();
        infWindow->setWindowTitle("Wallet information");
        infWindow->show();

        this->close();    // close the main window
    }
    else
    {
        QMessageBox::warning(this, "Error", "The authorization data do not match, try again!");
    }
}

void MainWindow::change_window()
{
    passWindow = new PasswordForWallet();
    passWindow->setWindowTitle("Offline wallet");
    passWindow->show();
}

void MainWindow::on_go_to_create_wallet_clicked()
{
    change_window();
    this->close();    // close the main window
}
