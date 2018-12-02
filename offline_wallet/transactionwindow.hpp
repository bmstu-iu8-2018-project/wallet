#ifndef TRANSACTIONWINDOW_HPP
#define TRANSACTIONWINDOW_HPP

#include <QWidget>
#include <QDirIterator>
#include <QDir>
#include <includes/Transaction.hpp>
#include <includes/JsonUtils.hpp>

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

    void setTxIdBox(const QDir& dir);

private slots:
    void on_clouse_clicked();

    void on_tx_id_box_currentIndexChanged(int index);

    void on_sign_clicked();

private:
    Ui::TransactionWindow *ui;

    QDir tx_dir_;

    Transaction tx;

    void init_inputs_table();

    void init_outputs_table();

    void remove_table();

    void setInputs(const std::vector<TxIn>& inputs);

    void setOutputs(const std::vector<TxOut>& outputs);

    QString get_private_key();

    void update_signed_tx();
};

#endif // TRANSACTIONWINDOW_HPP
