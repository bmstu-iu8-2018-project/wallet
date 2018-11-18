#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include "maketransactionwindow.h"
#include "includes/jsonwallet.h"
#include "includes/usb_monitor.h"


namespace Ui {
class InformationWindow;
}

class InformationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWindow(QWidget *parent = nullptr);

    ~InformationWindow();

    void set_name(const QString& name);

private slots:
    void on_make_transaction_clicked();

private:
    Ui::InformationWindow *ui;

    MakeTransactionWindow *transWindow;

    void change_window();

};

#endif // INFORMATIONWINDOW_H
