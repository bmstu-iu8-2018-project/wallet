#ifndef THREADMONITOR_H
#define THREADMONITOR_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <includes/usb_monitor.hpp>

class ThreadMonitor : public QObject
{
    Q_OBJECT
    usb_monitor *mon_;

public:
    explicit ThreadMonitor(QObject *parent = nullptr);
    ~ThreadMonitor() = default;

signals:
    void usb_not_found();
    void unknow_usb();

public slots:
    void start_find();
    void check_usb();
};

#endif // THREADMONITOR_H
