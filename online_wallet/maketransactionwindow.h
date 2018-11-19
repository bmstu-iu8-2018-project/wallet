#ifndef MAKETRANSACTIONWINDOW_H
#define MAKETRANSACTIONWINDOW_H

#include <QWidget>
#include <QDir>

namespace Ui
{
class MakeTransactionWindow;
}

class MakeTransactionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MakeTransactionWindow(QWidget *parent = nullptr);

    ~MakeTransactionWindow();

    QString get_address();

    void set_json(const QString& str);

private slots:
    void on_build_transaction_clicked();

    void on_clouse_clicked();

private:
    Ui::MakeTransactionWindow *ui;

    size_t number_trans_ = 0;

    QJsonDocument *json_doc_;

    void build_transaction();

    QString get_path_transaction();

    void create_json_transaction(QDir dir);

    QJsonArray create_array_in();

    QJsonArray create_array_out();

};

#endif // MAKETRANSACTIONWINDOW_H
