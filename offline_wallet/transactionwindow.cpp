#include <ui_transactionwindow.h>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QTextBrowser>
#include <includes/LoggingCategories.hpp>
#include <mainwindow.hpp>
#include <transactionwindow.hpp>

TransactionWindow::TransactionWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::TransactionWindow) {
  ui->setupUi(this);
  qInfo(logInfo()) << "Initialize tx_view window";
  init_inputs_table();
  init_outputs_table();
}

TransactionWindow::~TransactionWindow() {
  delete ui;
}

void TransactionWindow::setTxIdBox(const QDir& dir) {
  tx_dir_ = dir;
  ui->tx_id_box->addItem("Not chosen");
  QStringList list_dir =
      dir.entryList(QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot);
  ui->tx_id_box->addItems(list_dir);
}

void TransactionWindow::on_clouse_clicked() {
  this->close();
}

void TransactionWindow::init_inputs_table() {
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

void TransactionWindow::init_outputs_table() {
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

void TransactionWindow::remove_table() {
  ui->inputs->setRowCount(0);
  ui->outputs->setRowCount(0);
  ui->label_inputs->setText(QString("Inputs (%1)").arg(0));
  ui->label_outputs->setText(QString("Outputs (%1)").arg(0));
  ui->inputs->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->inputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->outputs->horizontalHeader()->setSectionResizeMode(0,
                                                        QHeaderView::Stretch);
  ui->outputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
  ui->status->setText("Unknown");
}

void TransactionWindow::setInputs(const std::vector<TxIn>& inputs) {
  for (const auto& it : inputs) {
    int row_input = ui->inputs->rowCount();
    ui->inputs->insertRow(row_input);
    ui->inputs->setItem(row_input, 0,
                        new QTableWidgetItem(it.get_prev_tx_hash().c_str()));
    ui->inputs->setItem(row_input, 1,
                        new QTableWidgetItem(QString::number(it.get_index())));
    ui->inputs->resizeColumnsToContents();
    ui->inputs->item(row_input, 0)->setTextAlignment(Qt::AlignCenter);
    ui->inputs->item(row_input, 1)->setTextAlignment(Qt::AlignCenter);
  }
  ui->label_inputs->setText(QString("Inputs (%1)").arg(inputs.size()));
}

void TransactionWindow::setOutputs(const std::vector<TxOut>& outputs) {
  for (const auto& it : outputs) {
    int row_output = ui->outputs->rowCount();
    ui->outputs->insertRow(row_output);
    QString addres = cu::get_addres_from_script(it.get_script().data()).c_str();
    ui->outputs->setItem(row_output, 0, new QTableWidgetItem(addres));
    QString value =
        QString::number(static_cast<double>(it.get_value()) / SATOSHI_COEF);
    ui->outputs->setItem(row_output, 1, new QTableWidgetItem(value));
    ui->outputs->item(row_output, 0)->setTextAlignment(Qt::AlignCenter);
    ui->outputs->item(row_output, 1)->setTextAlignment(Qt::AlignCenter);
  }
  ui->label_outputs->setText(QString("Outputs (%1)").arg(outputs.size()));
}

void TransactionWindow::on_tx_id_box_currentIndexChanged(int index) {
  remove_table();

  if (index != 0) {
    QString path = tx_dir_.path() + QDir::separator() +
                   ui->tx_id_box->currentText() + QDir::separator() + "tx.dat";
    tx = Transaction::parse(path.toStdString());

    setInputs(tx.get_inputs());

    setOutputs(tx.get_outputs());

    if (tx.is_signed())
      ui->status->setText("Signed");
    else
      ui->status->setText("Unsigned");

    qInfo(logInfo()) << "Selected tx: " << ui->tx_id_box->currentText();
    qInfo(logInfo()) << "Tx status: " << ui->status->text();
  }
}

QString TransactionWindow::get_private_key() {
  QDir dir(QDir::homePath() + QDir::separator() + "Private data");
  dir.cd(MainWindow::get_wallet_name());
  return ju::get_information(dir.path() + QDir::separator() + "wallet.dat.json",
                             "private_key");
}

void TransactionWindow::update_signed_tx() {
  QString path = tx_dir_.path() + QDir::separator() +
                 ui->tx_id_box->currentText() + QDir::separator() + "tx.dat";
  QFile file_tx(path);
  file_tx.open(QFile::WriteOnly);
  file_tx.write(tx.get_hex_tx().c_str());
}

void TransactionWindow::on_sign_clicked() {
  if (ui->status->text() == "Unsigned") {
    tx.sign(get_private_key().toStdString());
    update_signed_tx();
    ui->status->setText("Signed");
    QMessageBox::information(this, "Message",
                             "Transaction is successfully signed!");
    qInfo(logInfo()) << "This transaction is signed";
  } else if (ui->tx_id_box->currentIndex() == 0) {
    QMessageBox::warning(this, "Message", "Select transaction!");
    qWarning(logWarning()) << "Not selected transaction";
  } else {
    QMessageBox::information(this, "Message",
                             "This transaction is already signed!");
    qInfo(logInfo()) << "This transaction is already signed";
  }
}

void TransactionWindow::on_get_hex_clicked() {
  if (ui->tx_id_box->currentIndex() != 0) {
    QDialog dialog(this);
    QGridLayout grid(&dialog);

    std::unique_ptr<QTextBrowser> tx(new QTextBrowser(&dialog));

    grid.addWidget(tx.get());

    QString path = tx_dir_.path() + QDir::separator() +
                   ui->tx_id_box->currentText() + QDir::separator() + "tx.dat";
    QFile file_tx(path);
    file_tx.open(QFile::ReadOnly);
    tx->setText(file_tx.readAll());

    QDialogButtonBox buttonBox(QDialogButtonBox::Cancel, Qt::Horizontal,
                               &dialog);

    grid.addWidget(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(&grid);
    dialog.setMinimumSize(700, 300);
    int rc = dialog.exec();

    if (rc == QDialog::Rejected)
      dialog.close();
  } else {
    QMessageBox::warning(this, "Message", "Select transaction!");
    qWarning(logWarning()) << "Not selected transaction";
  }
}
