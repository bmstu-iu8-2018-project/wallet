#ifndef MESSAGER_H
#define MESSAGER_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>

class Messager : public QObject
{
    Q_OBJECT
 public:
    explicit Messager(QObject *parent = nullptr);

 public slots:
    void message();
};

#endif // MESSAGER_H
