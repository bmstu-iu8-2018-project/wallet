#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "maketransactionwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_make_transaction_clicked();

private:
    Ui::MainWindow *ui;
    MakeTransactionWindow *transWindow;

    void change_window();
};

#endif // MAINWINDOW_H
