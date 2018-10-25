#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QWidget>

namespace Ui
{
    class InformationWindow;
}

class InformationWindow : public QWidget
{
    Q_OBJECT

 public:
    explicit InformationWindow(QWidget *parent = nullptr);
    ~InformationWindow();

 private:
    Ui::InformationWindow *ui;

    QString get_path_private_dir();

};

#endif // INFORMATIONWINDOW_H
