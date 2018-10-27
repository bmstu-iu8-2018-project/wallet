#ifndef MAKETRANSACTIONWINDOW_H
#define MAKETRANSACTIONWINDOW_H

#include <QWidget>

namespace Ui {
class MakeTransactionWindow;
}

class MakeTransactionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MakeTransactionWindow(QWidget *parent = nullptr);
    ~MakeTransactionWindow();

private:
    Ui::MakeTransactionWindow *ui;
};

#endif // MAKETRANSACTIONWINDOW_H
