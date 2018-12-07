#include "threadmonitor.h"
#include <includes/LoggingCategories.hpp>
#include <QDir>
#include <QDirIterator>
#include <QThread>

ThreadMonitor::ThreadMonitor(QObject *parent) : QObject(parent)
{
    mon_ = usb_monitor::create();
    mon_->mount_existing_devices();
    mon_->start();
}

void ThreadMonitor::start_find()
{
    qInfo(logInfo()) << "Find usb device ...";
    std::set<wchar_t> set_device = mon_->get_flash_disks(1);

    while (true)
    {
        set_device = mon_->get_flash_disks(1);

        if (!set_device.empty())
            check_usb();
        else
            emit usb_not_found();
    }
}

void ThreadMonitor::check_usb()
{
    bool flag = false;
    QDirIterator it(QDir::drives().last().dir().path(),
                    QStringList() << "*.dat", QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        if (it.fileName() == "mark.dat")
        {
            flag = true;
            break;
        }
    }
    if (!flag)
        emit unknow_usb();
}
