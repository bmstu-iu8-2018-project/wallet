#ifndef USBMONITOR_H
#define USBMONITOR_H

#include <QObject>
#include <QMessageBox>
#include <QMutex>
#include <QMutexLocker>
#include <includes/usb_monitor.hpp>

class UsbMonitor : public QObject
{
    Q_OBJECT
    // Свойство, управляющее работой потока
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)

    bool m_running;
    usb_monitor *mon;
    QMutex mutex;

public:
    UsbMonitor();
    ~UsbMonitor() = default;
    bool running() const;

public slots:
    void start_find();
    void setRunning(bool running);

signals:
    void finished();
    void runningChanged(bool running);
};

#endif // USBMONITOR_H
