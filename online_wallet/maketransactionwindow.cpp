#include <maketransactionwindow.hpp>
#include <ui_maketransactionwindow.h>
#include <mainwindow.hpp>
#include <informationwindow.hpp>

MakeTransactionWindow::MakeTransactionWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MakeTransactionWindow)
{
    ui->setupUi(this);
    init_inputs_table();
    init_outputs_table();
}

void MakeTransactionWindow::init_inputs_table()
{
    ui->inputs->setColumnCount(2);
    ui->inputs->setShowGrid(true);
    ui->inputs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->inputs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->inputs->setHorizontalHeaderLabels(QStringList()
                                          << "Previous output"
                                          << "Index");
    ui->inputs->setAlternatingRowColors(Qt::AlignLeft);
    ui->inputs->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->inputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
}

void MakeTransactionWindow::init_outputs_table()
{
    ui->outputs->setColumnCount(2);
    ui->outputs->setShowGrid(true);
    ui->outputs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->outputs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->outputs->setHorizontalHeaderLabels(QStringList()
                                          << "Address"
                                          << "Ammount");
    ui->outputs->setAlternatingRowColors(Qt::AlignLeft);
    ui->outputs->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->outputs->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
}

MakeTransactionWindow::~MakeTransactionWindow()
{
    delete ui;
}

QString MakeTransactionWindow::get_path_transaction()
{
    QDir dir(MainWindow::get_public_data_path());
    dir.cd("Transactions");
    return dir.path();
}

void MakeTransactionWindow::create_transaction(QDir dir)
{
    Transaction tx(1, 0, vec_inputs_, vec_outputs_);
    auto raw_tx = tx.get_hex_tx();

    QFile file_tx(dir.path() + "/tx.dat");
    file_tx.open(QFile::WriteOnly);
    file_tx.write(raw_tx.c_str());
}

void MakeTransactionWindow::build_transaction()
{
    QDir dir(get_path_transaction());

    QStringList listFiles = dir.entryList(QDir::Dirs);
    number_trans_ = listFiles.size() - 1;

    QString tr = "transaction" + QString::number(number_trans_);
    dir.mkdir(tr);
    dir.cd(tr);

    create_transaction(dir);
}

void MakeTransactionWindow::on_build_transaction_clicked()
{
    build_transaction();
    QMessageBox::information(this, "Message", "Transaction successfully created!");
    ui->inputs->setRowCount(0);
    ui->outputs->setRowCount(0);
    ui->input_label->setText(QString("Inputs (%1)").arg(0));
    ui->output_label->setText(QString("Outputs (%1)").arg(0));
}

void MakeTransactionWindow::on_clouse_clicked()
{
    this->close();
}

void MakeTransactionWindow::on_create_input_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    QLineEdit *previous_output = new QLineEdit(&dialog);
    form.addRow("Previous output :", previous_output);

    QLineEdit *index = new QLineEdit(&dialog);
    form.addRow("Index :", index);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(&form);
    dialog.setMinimumSize(710, 150);
    dialog.exec();

    if (buttonBox.Ok && previous_output->text().isEmpty())
    {
        QMessageBox::warning(this, "Message", "Enter previous output!");
    }
    else if (buttonBox.Ok && index->text().isEmpty())
    {
        QMessageBox::warning(this, "Message", "Enter index!");
    }
    else
    {
        // create input
        TxIn input;
        input.set_previous_output(previous_output->text().toStdString(), index->text().toUInt());
        input.set_default_sequance();
        Script input_script;
        input_script.create_pkh_script(InformationWindow::get_address().toStdString());
        input.set_pkh_script(input_script);

        vec_inputs_.push_back(input);

        // filling the table
        int rowNumber = ui->inputs->rowCount();
        ui->inputs->insertRow(rowNumber);
        ui->inputs->setItem(rowNumber, 0, new QTableWidgetItem(previous_output->text()));
        ui->inputs->setItem(rowNumber, 1, new QTableWidgetItem(index->text()));
        ui->inputs->resizeColumnsToContents();
        ui->inputs->item(rowNumber, 0)->setTextAlignment(Qt::AlignCenter);
        ui->inputs->item(rowNumber, 1)->setTextAlignment(Qt::AlignCenter);
        ui->input_label->setText(QString("Inputs (%1)").arg(vec_inputs_.size()));
    }

    delete previous_output;
    delete index;
}

void MakeTransactionWindow::on_create_output_clicked()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    QLineEdit *address = new QLineEdit(&dialog);
    form.addRow("Address :", address);

    QLineEdit *ammount = new QLineEdit(&dialog);
    form.addRow("Ammount :", ammount);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(&form);
    dialog.setMinimumSize(710, 150);
    dialog.exec();

    if (buttonBox.Ok && address->text().isEmpty())
    {
        QMessageBox::warning(this, "Message", "Enter address!");
    }
    else if (buttonBox.Ok && ammount->text().isEmpty())
    {
        QMessageBox::warning(this, "Message", "Enter ammount!");
    }
    else
    {
        // create output
        Script output_script;
        output_script.create_pkh_script(address->text().toStdString());
        TxOut output(ammount->text().toDouble(), output_script);

        vec_outputs_.push_back(output);

        // filling the table
        int rowNumber = ui->outputs->rowCount();
        ui->outputs->insertRow(rowNumber);
        ui->outputs->setItem(rowNumber,0, new QTableWidgetItem(address->text()));
        ui->outputs->setItem(rowNumber,1, new QTableWidgetItem(ammount->text()));
        ui->outputs->item(rowNumber, 0)->setTextAlignment(Qt::AlignCenter);
        ui->outputs->item(rowNumber, 1)->setTextAlignment(Qt::AlignCenter);
        ui->output_label->setText(QString("Outputs (%1)").arg(vec_outputs_.size()));
    }

    delete address;
    delete ammount;
}
