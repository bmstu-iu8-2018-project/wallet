#include "maketransactionwindow.h"
#include "ui_maketransactionwindow.h"

MakeTransactionWindow::MakeTransactionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MakeTransactionWindow)
{
    ui->setupUi(this);
}

MakeTransactionWindow::~MakeTransactionWindow()
{
    delete ui;
}
