#include "messager.h"
#include <QFormLayout>
#include <QLabel>
#include <memory>

Messager::Messager(QObject *parent) : QObject(parent)
{}

void Messager::message()
{
    QMessageBox::warning(nullptr, "Message",
                         "Insert marked USB device!",
                         QMessageBox::NoButton);
}
