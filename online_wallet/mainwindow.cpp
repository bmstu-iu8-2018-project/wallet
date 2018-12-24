#include <ui_mainwindow.h>
#include <includes/JsonUtils.hpp>
#include <includes/LoggingCategories.hpp>
#include <mainwindow.hpp>

QString MainWindow::mark_path_;
QString MainWindow::password_;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  find_usb_device();
  chek_device();

  ui->setupUi(this);

  ui->wallets->addItem("(select wallet)");
  ui->wallets->addItems(str_list_);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::change_window() {
  infWindow = new InformationWindow();
  infWindow->setWindowTitle("Online wallet");
  infWindow->set_name(ui->wallets->currentText());
  infWindow->show();
}

QString MainWindow::get_device_path() {
  return QDir::drives().last().dir().path();
}

QString MainWindow::get_key() {
  return password_;
}

void MainWindow::find_usb_device() {
  mon = usb_monitor::create();
  mon->mount_existing_devices();
  mon->start();
}

void MainWindow::chek_device() {
  std::set<wchar_t> set_device = mon->get_flash_disks(1);
  while (set_device.empty()) {
    if (set_device.empty())
      QMessageBox::warning(this, "Message", "Insert marked USB device!",
                           QMessageBox::NoButton);
    set_device = mon->get_flash_disks(1);
  }

  mon->stop();
  chec_mark_on_device(get_device_path());
}

void MainWindow::chec_mark_on_device(const QString& path) {
  bool flag = false;

  QDirIterator it(path, QStringList() << "*.dat", QDir::Files,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    it.next();

    if (it.fileName() == "mark.dat") {
      flag = true;
      QDir dir(it.filePath());
      dir.cdUp();
      str_list_.push_back(dir.dirName());
    }
  }

  if (!flag) {
    QMessageBox::warning(
        this, "Message",
        "You inserted an incorrect USB flash drive!\nTry again");
    chek_device();
    chec_mark_on_device(get_device_path());
  }
}

QString MainWindow::get_public_data_path() {
  QDir dir(mark_path_);
  return dir.path();
}

void MainWindow::on_wallets_currentIndexChanged(int index) {
  if (index != 0) {
    mark_path_ =
        get_device_path() + ui->wallets->currentText() + QDir::separator();
    password_ = init_dialog_password();
    if (password_ == "false") {
      ui->wallets->setCurrentIndex(0);
    } else {
      change_window();
      this->close();
    }
  }
}

QString MainWindow::init_dialog_password() {
  QDialog dialog(this);
  QFormLayout form(&dialog);

  std::unique_ptr<QLineEdit> tx_id(new QLineEdit(&dialog));
  tx_id->setEchoMode(QLineEdit::Password);
  form.addRow("Password :", tx_id.get());

  QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                             Qt::Horizontal, &dialog);

  form.addRow(&buttonBox);
  QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
  QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

  dialog.setLayout(&form);
  dialog.setMinimumSize(420, 120);
  int rc = dialog.exec();

  if ((rc == QDialog::Accepted) && tx_id->text().isEmpty()) {
    QMessageBox::warning(this, "Message", "Enter password!");
    qWarning(logWarning()) << "Not enter password";
    init_dialog_password();
  } else if (rc == QDialog::Rejected) {
    return "false";
  }

  return tx_id->text();
}
