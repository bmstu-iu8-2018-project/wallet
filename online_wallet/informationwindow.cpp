#include <informationwindow.hpp>
#include <ui_informationwindow.h>
#include <mainwindow.hpp>
#include <includes/LoggingCategories.hpp>

QString InformationWindow::address_;

InformationWindow::InformationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InformationWindow)
{
    ui->setupUi(this);
    qInfo(logInfo()) << "Initialize information window";

    QString path_private_data = MainWindow::get_public_data_path();

    ui->address->setText(ju::get_address(path_private_data + QDir::separator() + "address.dat.json"));
    ui->public_key->setText(ju::get_public_key(path_private_data + QDir::separator() + "address.dat.json"));
    ui->balance->setText(ju::get_balance(ui->address->text()));

    qInfo(logInfo()) << "Wallet name: " << ui->name->text();
    qInfo(logInfo()) << "Wallet address: " << ui->address->text();
    qInfo(logInfo()) << "Wallet dalance: " << ui->balance->text();

    init_box_requests();

    address_ = ui->address->text();
}

QString InformationWindow::get_address()
{
    return address_;
}

void InformationWindow::set_name(const QString& name)
{
    ui->name->setText(name);
}

void InformationWindow::init_box_requests()
{
    ui->boxRequests->addItem("(select requests)");
    ui->boxRequests->addItem("address balanse");
    ui->boxRequests->addItem("spent transactions");
    ui->boxRequests->addItem("unspent transactions");
    ui->boxRequests->addItem("received transactions");
    ui->boxRequests->addItem("transaction");
}

InformationWindow::~InformationWindow()
{
    delete ui;
}

void InformationWindow::change_make_tx_window()
{
    transWindow = new MakeTransactionWindow();
    transWindow->setWindowTitle("Online wallet");
    transWindow->show();
}

void InformationWindow::on_make_transaction_clicked()
{
    change_make_tx_window();
}

void InformationWindow::on_exit_clicked()
{
    auto mainWindow = new MainWindow();
    mainWindow->setAttribute( Qt::WA_DeleteOnClose );
    mainWindow->setWindowTitle("Online Wallet");
    mainWindow->show();
    deleteLater();
    this->close();
}

void InformationWindow::on_boxRequests_currentIndexChanged(int index)
{
    switch (index)
    {
        case (0):
        {
            ui->requestBrowser->clear();
            break;
        }
        case (static_cast<int>(Requests::address_balance)):
        {
            ui->requestBrowser->clear();
            const auto balance_json_data = nu::get_address_balance(ui->address->text().toStdString());
            ui->requestBrowser->setText(balance_json_data.c_str());
            qInfo(logInfo()) << "Request received address_balance";
            break;
        }
        case (static_cast<int>(Requests::spent_transactions)):
        {
            ui->requestBrowser->clear();
            const auto spend_trans_json_data = nu::get_spent_transactions(ui->address->text().toStdString());
            ui->requestBrowser->setText(spend_trans_json_data.c_str());
            qInfo(logInfo()) << "Request received spent_transactions";
            break;
        }
        case (static_cast<int>(Requests::unspent_transactions)):
        {
            ui->requestBrowser->clear();
            const auto unspend_trans_json_data = nu::get_unspent_transactions(ui->address->text().toStdString());
            ui->requestBrowser->setText(unspend_trans_json_data.c_str());
            qInfo(logInfo()) << "Request received unspent_transactions";
            break;
        }
        case (static_cast<int>(Requests::received_transactions)):
        {
            ui->requestBrowser->clear();
            const auto received_trans_json_data = nu::get_received_transactions(ui->address->text().toStdString());
            ui->requestBrowser->setText(received_trans_json_data.c_str());
            qInfo(logInfo()) << "Request received received_transactions";
            break;
        }
        case (static_cast<int>(Requests::transaction)):
        {
            ui->requestBrowser->clear();
            auto tx_id = init_dialog_id();
            const auto trans_json_data = nu::get_transaction(tx_id.toStdString());
            ui->requestBrowser->setText(trans_json_data.c_str());
            qInfo(logInfo()) << "Request received transactions";
            break;
        }
    }
}

QString InformationWindow::init_dialog_id()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    std::unique_ptr<QLineEdit> tx_id (new QLineEdit(&dialog));
    form.addRow("Transaction id :", tx_id.get());

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    dialog.setLayout(&form);
    dialog.setMinimumSize(710, 120);
    dialog.exec();

    if (buttonBox.Ok && tx_id->text().isEmpty())
    {
        QMessageBox::warning(this, "Message", "Enter transaction id!");
        qWarning(logWarning()) << "Not enter transaction id";
        init_dialog_id();
    }

    return tx_id->text();
}

QDir InformationWindow::get_path_transaction()
{
    QDir dir(MainWindow::get_public_data_path());
    dir.cd("Transactions");
    return dir;
}

void InformationWindow::change_tx_window()
{
    txWindow = new TransactionWindow();
    txWindow->setWindowTitle("Online wallet");
    txWindow->setTxIdBox(get_path_transaction());
    txWindow->show();
}

void InformationWindow::on_view_tx_clicked()
{
    change_tx_window();
}
