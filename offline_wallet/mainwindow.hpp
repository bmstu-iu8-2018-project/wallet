#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDir>
#include <QDirIterator>
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

 private slots:
  void on_authorization_clicked();

  void on_go_to_create_wallet_clicked();

 private:
  static QString name_;

  Ui::MainWindow* ui;

  InformationWindow* infWindow;

  PasswordForWallet* passWindow;

  void change_window();

  QString load_password(const QString& path);
};

#endif  // MAINWINDOW_HPP
