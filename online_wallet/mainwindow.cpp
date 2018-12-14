#include <ui_mainwindow.h>
#include <includes/JsonUtils.hpp>
#include <includes/LoggingCategories.hpp>
#include <mainwindow.hpp>

QString MainWindow::mark_path_;

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
    change_window();
    this->close();
  }
}
