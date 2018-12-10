#ifndef MAKETRANSACTIONWINDOW_HPP
#define MAKETRANSACTIONWINDOW_HPP

#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QFormLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>
#include <QWidget>
#include <includes/CryptoUtils.hpp>
#include <includes/JsonUtils.hpp>
#include <includes/Transaction.hpp>
#include <memory>

namespace Ui {
class MakeTransactionWindow;
}

class MakeTransactionWindow : public QWidget {
  Q_OBJECT

 public:
  explicit MakeTransactionWindow(QWidget* parent = nullptr);

  ~MakeTransactionWindow();

 private slots:
  void on_build_transaction_clicked();

  void on_clouse_clicked();

  void on_create_input_clicked();

  void on_create_output_clicked();

 private:
  Ui::MakeTransactionWindow* ui;

  size_t number_trans_ = 0;

  void build_transaction();

  QString get_path_transaction();

  void init_inputs_table();

  void init_outputs_table();

  void create_transaction(QDir dir);

  TxIn create_input(const QString& txid, unsigned int index);

  void update_inputs_table(const QString& txid, const QString& index);

  TxOut create_output(const QString& value, const QString& address);

  void update_outputs_table(const QString& value, const QString& address);

  outputs vec_outputs_;

  inputs vec_inputs_;
};

#endif  // MAKETRANSACTIONWINDOW_HPP
