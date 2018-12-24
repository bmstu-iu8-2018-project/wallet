#include <ui_mainwindow.h>
#include <includes/Encoder.hpp>
#include <includes/LoggingCategories.hpp>
#include <mainwindow.hpp>

QString MainWindow::name_;
QString MainWindow::password_;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  qInfo(logInfo()) << "Initialize authorization window";
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_authorization_clicked() {
  QString login = ui->login->text();
  QString password = ui->pass->text();

  QDir dir(QDir::homePath() + QDir::separator() + "Private data");
  if (!dir.exists()) {
    QMessageBox::warning(this, "Error", "Wallet is not created on the device!");
    qWarning(logWarning()) << "Wallet is not created on the device";
  }

  QStringList list_dir =
      dir.entryList(QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot);
  auto it = qFind(list_dir.begin(), list_dir.end(), login);

  if (it != list_dir.end()) {
    dir.cd(login);
    QString login_of_file = ju::get_name(
        dir.path() + QDir::separator() + "authorization_data.dat", password);

    QString password_of_file = load_password(dir.path(), password);

    if (login.isEmpty() && password.isEmpty()) {
      QMessageBox::warning(this, "Error", "Enter login or passwword!");
      qWarning(logWarning()) << "Login or password not entered at login";
    } else if (login.isEmpty()) {
      QMessageBox::warning(this, "Error", "Enter login!");
      qWarning(logWarning()) << "Login not entered at login";
    } else if (password.isEmpty()) {
      QMessageBox::warning(this, "Error", "Enter password!");
      qWarning(logWarning()) << "Password not entered at login";
    } else if ((login == login_of_file) && (password == password_of_file)) {
      name_ = login_of_file;
      password_ = password;
      // Open window with wallet information
      infWindow = new InformationWindow();
      infWindow->setWindowTitle("Offline wallet");
      infWindow->show();

      qInfo(logInfo()) << "Made entry into the wallet: " << login_of_file;
      this->close();  // close the main window
    } else {
      QMessageBox::warning(this, "Error",
                           "The authorization data do not match, try again!");
      qWarning(logWarning()) << "Authorization data mismatch at login";
    }
  } else {
    QMessageBox::warning(this, "Error", "Wallet is not created on the device!");
    qWarning(logWarning()) << "Wallet is not created on the device";
  }
}

QString MainWindow::load_password(const QString& path, const QString& key) {
  QFile jsonFile(path + QDir::separator() + "authorization_data.dat");
  jsonFile.open(QFile::ReadOnly);
  QString ciphertext = jsonFile.readAll();
  auto text =
      Encoder::decrypt(ciphertext.toStdString(), cu::SHA256(key.toStdString()));
  QJsonDocument document = QJsonDocument().fromJson(text.c_str());
  QJsonObject name = document.object();
  QJsonValue value = name.value("password");
  return value.toString();
}

QString MainWindow::get_wallet_name() {
  return name_;
}

QString MainWindow::get_key() {
  return password_;
}

void MainWindow::change_window() {
  passWindow = new PasswordForWallet();
  passWindow->setWindowTitle("Offline wallet");
  passWindow->show();
}

void MainWindow::on_go_to_create_wallet_clicked() {
  change_window();
  this->close();  // close the main window
}

void MainWindow::copy_path(QString src, QString dst) {
  QDir dir(src);
  if (!dir.exists())
    dir.mkpath(".");

  foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    QString dst_path = dst + QDir::separator() + d;
    dir.mkpath(dst_path);
    copy_path(src + QDir::separator() + d, dst_path);
  }

  foreach (QString f, dir.entryList(QDir::Files)) {
    QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
  }
}

void MainWindow::on_recover_clicked() {
  QString backup_path =
      QFileDialog::getExistingDirectory(this, "Сhoose backup path", "");
  copy_path(backup_path, QDir::homePath() + QDir::separator() + "Private data");
}
