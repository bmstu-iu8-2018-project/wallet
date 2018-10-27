#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMessageBox::information(this, "Message", "Insert USB device!");
}

MainWindow::~MainWindow()
{
    delete ui;
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
