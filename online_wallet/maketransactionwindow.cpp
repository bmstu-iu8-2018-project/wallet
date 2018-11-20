#include "maketransactionwindow.h"
#include "ui_maketransactionwindow.h"
#include "mainwindow.h"
#include "includes/jsonwallet.h"
#include "includes/CryptoUtils.h"
#include <QDir>
#include <QJsonArray>
#include <QMessageBox>
#include <QDirIterator>
#include <QThread>

MakeTransactionWindow::MakeTransactionWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MakeTransactionWindow)
{
    ui->setupUi(this);
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

// QJsonArray MakeTransactionWindow::create_array_in()

// QJsonArray MakeTransactionWindow::create_array_out()


void MakeTransactionWindow::create_json_transaction(QDir dir)
{
    QVariantMap raw_transactoin;

    QVariantMap in_map;
    in_map.insert("prev_out", "null");
    in_map.insert("scriptSig", "...");
    QJsonObject object_in = QJsonObject::fromVariantMap(in_map);

    QJsonArray array_in;
    array_in.append(object_in);

    raw_transactoin.insert("in", array_in);

    QVariantMap out_map;
    out_map.insert("value", ui->value->text());
    out_map.insert("scriptPubkey", "...");
    out_map.insert("address", ui->address->text());
    QJsonObject object_out = QJsonObject::fromVariantMap(out_map);

    QJsonArray array_out;
    array_out.append(object_out);

    raw_transactoin.insert("out", array_out);

    QJsonObject json_obj = QJsonObject::fromVariantMap(raw_transactoin);
    QJsonDocument json_doc(json_obj);
    QString json_string = json_doc.toJson();

    std::string hash = cu::SHA256(cu::SHA256(json_string.toUtf8().constData()));
    raw_transactoin.insert("hash", QString::fromUtf8(hash.c_str()));

    JsonWallet::record_to_json(raw_transactoin, dir.path() + "/transaction.json");
}

void MakeTransactionWindow::build_transaction()
{
    QDir dir(get_path_transaction());


    QStringList listFiles = dir.entryList(QDir::Dirs);
    number_trans_ = listFiles.length() - 1;

    QString tr = "transaction" + QString::number(number_trans_);
    dir.mkdir(tr);
    dir.cd(tr);

    create_json_transaction(dir);
}

QString MakeTransactionWindow::get_address()
{
    return ui->address->text();
}

void MakeTransactionWindow::set_json(const QString& str)
{
    ui->json_trans->setText(JsonWallet::get_json(str));
}

void MakeTransactionWindow::on_build_transaction_clicked()
{
    build_transaction();
    QMessageBox::information(this, "Message", "Transaction successfully created!");
    ui->address->clear();
    ui->value->clear();
}

void MakeTransactionWindow::on_clouse_clicked()
{
    this->close();
}
