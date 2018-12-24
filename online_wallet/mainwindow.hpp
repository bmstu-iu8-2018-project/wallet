#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QComboBox>
#include <QDir>
#include <QDirIterator>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QStringListModel>
#include <informationwindow.hpp>
#include <maketransactionwindow.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

  ~MainWindow();

  static QString get_public_data_path();

  static QString get_key();

 private slots:
  void on_wallets_currentIndexChanged(int index);

 private:
  static QString mark_path_;

  static QString password_;

  Ui::MainWindow* ui;

  usb_monitor* mon;

  InformationWindow* infWindow;

  QStringList str_list_;

  void find_usb_device();

  void chek_device();

  void chec_mark_on_device(const QString& path);

  void change_window();

  QString get_device_path();

  QString init_dialog_password();
};

#endif  // MAINWINDOW_HPP
