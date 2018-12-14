#include <ui_maketransactionwindow.h>
#include <includes/LoggingCategories.hpp>
#include <informationwindow.hpp>
#include <mainwindow.hpp>
#include <maketransactionwindow.hpp>

MakeTransactionWindow::MakeTransactionWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MakeTransactionWindow) {
  ui->setupUi(this);
  qInfo(logInfo()) << "Initialize make tx window";

  init_inputs_table();
  init_outputs_table();
}

void MakeTransactionWindow::init_inputs_table() {
  ui->inputs->setColumnCount(2);
  ui->inputs->setShowGrid(true);
  ui->inputs->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->inputs->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->inputs->setHorizontalHeaderLabels(QStringList() << "Previous output"
                                                      << "Index");
  ui->inputs->setAlternatingRowColors(Qt::AlignLeft);
  ui->inputs->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->inputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
}

void MakeTransactionWindow::init_outputs_table() {
  ui->outputs->setColumnCount(2);
  ui->outputs->setShowGrid(true);
  ui->outputs->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->outputs->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->outputs->setHorizontalHeaderLabels(QStringList() << "Address"
                                                       << "Ammount");
  ui->outputs->setAlternatingRowColors(Qt::AlignLeft);
  ui->outputs->horizontalHeader()->setSectionResizeMode(0,
                                                        QHeaderView::Stretch);
  ui->outputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
}

MakeTransactionWindow::~MakeTransactionWindow() {
  delete ui;
}

QString MakeTransactionWindow::get_path_transaction() {
  QDir dir(MainWindow::get_public_data_path());
  dir.cd("Transactions");
  return dir.path();
}

void MakeTransactionWindow::create_transaction(QDir dir) {
  if (vec_inputs_.size() == 0) {
    QMessageBox::warning(this, "Message", "Created inputs!");
  } else if (vec_outputs_.size() == 0) {
    QMessageBox::warning(this, "Message", "Created outputs!");
  } else {
    Transaction tx(1, vec_inputs_, vec_outputs_, 0);
    auto raw_tx = tx.get_hex_tx();

    QFile file_tx(dir.path() + "/tx.dat");
    file_tx.open(QFile::WriteOnly);
    file_tx.write(raw_tx.c_str());
  }
}

void MakeTransactionWindow::build_transaction() {
  QDir dir(get_path_transaction());

  QStringList listFiles = dir.entryList(QDir::Dirs);
  number_trans_ = listFiles.size() - 1;

  QString tr = "transaction" + QString::number(number_trans_);
  dir.mkdir(tr);
  dir.cd(tr);

  create_transaction(dir);
}

void MakeTransactionWindow::on_build_transaction_clicked() {
  build_transaction();
  QMessageBox::information(this, "Message",
                           "Transaction successfully created!");
  ui->inputs->setRowCount(0);
  ui->outputs->setRowCount(0);
  ui->input_label->setText(QString("Inputs (%1)").arg(0));
  ui->output_label->setText(QString("Outputs (%1)").arg(0));
}

void MakeTransactionWindow::on_clouse_clicked() {
  this->close();
}

void MakeTransactionWindow::on_create_input_clicked() {
  QDialog dialog(this);
  QFormLayout form(&dialog);

  auto unspend_inputs = ju::get_unload_inputs(InformationWindow::get_address());
  QStringList str_list = unspend_inputs.first;
  QVector<QPair<int, QString>> vec_info = unspend_inputs.second;

  std::unique_ptr<QComboBox> previous_output(new QComboBox(&dialog));
  form.addRow("&Previous output :", previous_output.get());

  previous_output->addItems(str_list);
  previous_output->setCurrentIndex(-1);

  std::unique_ptr<QLabel> index_tx(new QLabel(&dialog));
  form.addRow("&Index :", index_tx.get());

  std::unique_ptr<QLabel> value(new QLabel(&dialog));
  form.addRow("&Value :", value.get());

  connect(previous_output.get(),
          QOverload<int>::of(&QComboBox::currentIndexChanged),
          [&index_tx, &previous_output, &vec_info, &value](int index) {
            index_tx->setText(QString::number(
                vec_info[previous_output->currentIndex()].first));
            value->setText(vec_info[previous_output->currentIndex()].second);
          });

  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);

  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  dialog.setLayout(&form);
  dialog.setMinimumSize(710, 150);
  int rc = dialog.exec();

  if ((rc == QDialog::Accepted) && (previous_output->currentIndex() != -1)) {
    // create input
    TxIn input =
        create_input(previous_output->currentText(), index_tx->text().toUInt());
    vec_inputs_.push_back(input);

    // filling the table
    update_inputs_table(previous_output->currentText(), index_tx->text());
  } else if ((rc == QDialog::Accepted) &&
             (previous_output->currentIndex() == -1)) {
    QMessageBox::warning(this, "Message", "Select previous output!");
    qWarning(logWarning()) << "Previous output not selected";
  }
}

TxIn MakeTransactionWindow::create_input(const QString& txid,
                                         unsigned int index) {
  TxIn input;
  input.set_previous_output(txid.toStdString(), index);
  input.set_default_sequance();
  Script input_script;
  input_script.create_pkh_script(
      InformationWindow::get_address().toStdString());
  input.set_pkh_script(input_script);

  return input;
}

void MakeTransactionWindow::update_inputs_table(const QString& txid,
                                                const QString& index) {
  int rowNumber = ui->inputs->rowCount();
  ui->inputs->insertRow(rowNumber);
  ui->inputs->setItem(rowNumber, 0, new QTableWidgetItem(txid));
  ui->inputs->setItem(rowNumber, 1, new QTableWidgetItem(index));
  ui->inputs->resizeColumnsToContents();
  ui->inputs->item(rowNumber, 0)->setTextAlignment(Qt::AlignCenter);
  ui->inputs->item(rowNumber, 1)->setTextAlignment(Qt::AlignCenter);
  ui->input_label->setText(QString("Inputs (%1)").arg(vec_inputs_.size()));
  qInfo(logInfo()) << "Create input: " << vec_inputs_.size();
}

void MakeTransactionWindow::on_create_output_clicked() {
  QDialog dialog(this);
  QFormLayout form(&dialog);

  std::unique_ptr<QLineEdit> address(new QLineEdit(&dialog));
  form.addRow("&Address :", address.get());

  std::unique_ptr<QLineEdit> ammount(new QLineEdit(&dialog));
  form.addRow("&Ammount :", ammount.get());

  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);

  std::unique_ptr<QPushButton> my_address(new QPushButton("&My Address"));
  form.addRow(my_address.get(), &buttonBox);
  QObject::connect(my_address.get(), &QPushButton::clicked, [&address]() {
    address->setText(InformationWindow::get_address());
  });
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  dialog.setLayout(&form);
  dialog.setMinimumSize(710, 150);
  int rc = dialog.exec();

  if ((rc == QDialog::Accepted) && address->text().isEmpty()) {
    QMessageBox::warning(this, "Message", "Enter address!");
    qWarning(logWarning()) << "Address not entered";
  } else if ((rc == QDialog::Accepted) && ammount->text().isEmpty()) {
    QMessageBox::warning(this, "Message", "Enter ammount!");
    qWarning(logWarning()) << "Ammount not entered";
  } else if (rc == QDialog::Rejected) {
    dialog.close();
  } else {
    // create output
    TxOut output = create_output(ammount->text(), address->text());
    vec_outputs_.push_back(output);

    // filling the table
    update_outputs_table(ammount->text(), address->text());
  }
}

TxOut MakeTransactionWindow::create_output(const QString& value,
                                           const QString& address) {
  Script output_script;
  output_script.create_pkh_script(address.toStdString());
  TxOut output(value.toDouble(), output_script);

  return output;
}

void MakeTransactionWindow::update_outputs_table(const QString& value,
                                                 const QString& address) {
  int rowNumber = ui->outputs->rowCount();
  ui->outputs->insertRow(rowNumber);
  ui->outputs->setItem(rowNumber, 0, new QTableWidgetItem(address));
  ui->outputs->setItem(rowNumber, 1, new QTableWidgetItem(value));
  ui->outputs->item(rowNumber, 0)->setTextAlignment(Qt::AlignCenter);
  ui->outputs->item(rowNumber, 1)->setTextAlignment(Qt::AlignCenter);
  ui->output_label->setText(QString("Outputs (%1)").arg(vec_outputs_.size()));
  qInfo(logInfo()) << "Create output: " << vec_outputs_.size();
}
