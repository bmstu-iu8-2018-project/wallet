#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow:: name_;

void MainWindow::on_authorization_clicked()
{
    QString login = ui->login->text();
    QString password = ui->pass->text();

    QDir dir(QDir::currentPath());

    QDirIterator it(dir.path(), QDir::Dirs, QDirIterator::Subdirectories);

    bool flag = false;
    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "Private data")
        {
            flag = true;
            dir.cd("Private data");

            QDirIterator iter(dir.path(), QDir::Dirs, QDirIterator::Subdirectories);
            while (iter.hasNext())
            {
                iter.next();

                if (iter.fileName() == login)
                {
                    dir.cd(login);
                    break;
                }
            }
        }
    }

    if (!flag)
        QMessageBox::warning(this, "Error", "Wallet is not created on the device!");

    QString login_of_file = JsonWallet::get_name(dir.path() + "/authorization_data.json");
    QString password_of_file = load_password(dir.path());

    if ((login == login_of_file) && (password == password_of_file) &&
            (!login.isEmpty()))
    {
        name_ = login_of_file;
        // Open window with wallet information
        infWindow = new InformationWindow();
        infWindow->setWindowTitle("Wallet information");
        infWindow->show();

        this->close();    // close the main window
    }
    else
    {
        QMessageBox::warning(this, "Error", "The authorization data do not match, try again!");
    }
}

QString MainWindow::load_password(const QString& path)
{
    QFile jsonFile(path + "/authorization_data.json");
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument document = QJsonDocument().fromJson(jsonFile.readAll());
    QJsonObject name = document.object();
    QJsonValue value = name.value("password");
    return value.toString();
}

QString MainWindow::get_wallet_name()
{
    return name_;
}


void MainWindow::change_window()
{
    passWindow = new PasswordForWallet();
    passWindow->setWindowTitle("Offline wallet");
    passWindow->show();
}

void MainWindow::on_go_to_create_wallet_clicked()
{
    change_window();
    this->close();    // close the main window
}
