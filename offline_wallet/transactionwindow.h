#ifndef TRANSACTIONWINDOW_H
#define TRANSACTIONWINDOW_H

#include <QWidget>
#include <includes/JsonUtils.h>

namespace Ui
{
class TransactionWindow;
}

class TransactionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget *parent = nullptr);
    ~TransactionWindow();

    void set_transaction(const QString& path);
private slots:
    void on_clouse_clicked();

private:
    Ui::TransactionWindow *ui;
};

#endif // TRANSACTIONWINDOW_H
