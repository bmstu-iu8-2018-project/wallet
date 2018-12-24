#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QtAlgorithms>
#include <QtDebug>
#include <includes/JsonUtils.hpp>
#include <informationwindow.hpp>
#include <passwordforwallet.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget* parent = nullptr);

  ~MainWindow();

  static QString get_wallet_name();

  static QString get_key();

 private slots:
  void on_authorization_clicked();

  void on_go_to_create_wallet_clicked();

  void on_recover_clicked();

 private:
  static QString name_;

  static QString password_;

  Ui::MainWindow* ui;

  InformationWindow* infWindow;

  PasswordForWallet* passWindow;

  void change_window();

  void copy_path(QString l, QString r);

  QString load_password(const QString& path, const QString& key);
};

#endif  // MAINWINDOW_HPP
