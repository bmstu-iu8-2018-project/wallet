#include <Windows.h>
#include <Winioctl.h>
#include <cfgmgr32.h>
#include <SetupAPI.h>
#include <Dbt.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <initguid.h>
#include <ntddstor.h>
#include <devguid.h>
#include <winioctl.h>
#include "usb_monitor.h"

#pragma comment(lib, "SetupAPI")

usb_monitor_exception::usb_monitor_exception(const std::string& message)
    :std::runtime_error(message)
{}

std::auto_ptr<usb_monitor> usb_monitor::instanse_;
const std::wstring usb_monitor::class_name(L"UsbMonWinClass");

usb_monitor::usb_monitor(bool monitor_hard_drives)
    :started_(false),
    safe_remove_on_timeout_(true),
    monitor_hard_drives_(monitor_hard_drives),
    mon_hwnd_(NULL)
{
    WNDCLASSEXW wndClass = {0};

    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandleW(NULL));
    wndClass.lpfnWndProc = reinterpret_cast<WNDPROC>(WinProcCallback);
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.lpszClassName = class_name.c_str();

    if(!RegisterClassExW(&wndClass))
        throw usb_monitor_exception("Cannot register window class");

    existing_usb_devices_ = get_flash_disks(monitor_hard_drives_);

    mon_hwnd_ = CreateWindowExW(WS_EX_CONTROLPARENT,
        class_name.c_str(),
        L"UsbMon",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        640, 480,
        NULL, NULL,
        GetModuleHandleW(NULL),
        NULL);

    if(mon_hwnd_ == NULL)
    {
        UnregisterClassW(class_name.c_str(), GetModuleHandleW(NULL));
        throw usb_monitor_exception("Cannot create window");
    }

    SetWindowLongPtrW(mon_hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

INT_PTR WINAPI usb_monitor::WinProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_DEVICECHANGE:
        {
            LONG_PTR data = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
            if(data)
                return reinterpret_cast<usb_monitor*>(data)->devices_changed(wParam, lParam);
        }
        return TRUE;
    }

    return TRUE;
}

BOOL usb_monitor::devices_changed(WPARAM wParam, LPARAM lParam)
{
    if(started_)
    {
        PDEV_BROADCAST_HDR pHdr = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
        switch(wParam)
        {
        case DBT_DEVICEARRIVAL:
            if(pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
                detect_changed_devices();
            break;

        case DBT_DEVICEQUERYREMOVEFAILED:
            if(pHdr->dbch_devicetype == DBT_DEVTYP_HANDLE)
            {
                PDEV_BROADCAST_HANDLE info = reinterpret_cast<PDEV_BROADCAST_HANDLE>(pHdr);

                notifications::iterator it = existing_notifications_.find(reinterpret_cast<size_t>(info->dbch_handle));
                if(it != existing_notifications_.end())
                {
                    if(!on_device_remove_failed_.empty())
                        on_device_remove_failed_(static_cast<char>((*it).second.second));
                }
            }
            break;

        case DBT_DEVICEQUERYREMOVE:
            if(pHdr->dbch_devicetype == DBT_DEVTYP_HANDLE)
            {
                PDEV_BROADCAST_HANDLE info = reinterpret_cast<PDEV_BROADCAST_HANDLE>(pHdr);

                notifications::iterator it = existing_notifications_.find(reinterpret_cast<size_t>(info->dbch_handle));
                if(it != existing_notifications_.end())
                {
                    if(!on_device_safe_removed_.empty())
                    {
                        if(!on_device_safe_removed_(static_cast<char>((*it).second.second)))
                            return BROADCAST_QUERY_DENY;

                        //Пользователь мог вызвать safe_eject внутри on_device_safe_removed, поэтому
                        //проверим этот момент еще раз
                        it = existing_notifications_.find(reinterpret_cast<size_t>(info->dbch_handle));
                    }

                    if(it != existing_notifications_.end())
                    {
                        if(safe_remove_on_timeout_ && (InSendMessageEx(NULL) & ISMEX_REPLIED))
                        {
                            try
                            {
                                safe_eject(static_cast<char>((*it).second.second));
                            }
                            catch(const usb_monitor_exception&)
                            {
                                //Ничего не делаем, WndProc хавает исключения
                            }
                        }
                        else
                        {
                            UnregisterDeviceNotification((*it).second.first);
                            CloseHandle(info->dbch_handle);
                            existing_notifications_.erase(it);
                        }
                    }
                }
            }
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            if(pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
                detect_changed_devices();
            break;
        }
    }

    return TRUE;
}

void usb_monitor::detect_changed_devices()
{
    std::vector<wchar_t> inserted, ejected;
    std::set<wchar_t> new_device_list = get_flash_disks(monitor_hard_drives_);
    std::set_difference(new_device_list.begin(), new_device_list.end(), existing_usb_devices_.begin(), existing_usb_devices_.end(),
        std::back_inserter(inserted));
    std::set_difference(existing_usb_devices_.begin(), existing_usb_devices_.end(), new_device_list.begin(), new_device_list.end(),
        std::back_inserter(ejected));

    existing_usb_devices_ = new_device_list;

    for(std::vector<wchar_t>::const_iterator it = inserted.begin(); it != inserted.end(); ++it)
        mount_device(*it);

    for(std::vector<wchar_t>::const_iterator it = ejected.begin(); it != ejected.end(); ++it)
        unmount_device(*it);
}

void usb_monitor::mount_device(wchar_t letter)
{
    for(notifications::iterator handle_it = existing_notifications_.begin(); handle_it != existing_notifications_.end(); ++handle_it)
        if((*handle_it).second.second == letter)
            return;

    wchar_t drive_name[3] = {0};
    drive_name[0] = letter;
    drive_name[1] = L':';
    UINT old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
    HANDLE device_handle = CreateFileW(
        drive_name,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    SetErrorMode(old_mode);

    //Не кидаем эксепшены, так как их все равно WindowProc съест
    if(device_handle == INVALID_HANDLE_VALUE)
        return;

    DEV_BROADCAST_HANDLE NotificationFilter = {0};
    NotificationFilter.dbch_size = sizeof(DEV_BROADCAST_HANDLE);
    NotificationFilter.dbch_devicetype = DBT_DEVTYP_HANDLE;
    NotificationFilter.dbch_handle = device_handle;

    HDEVNOTIFY token = RegisterDeviceNotificationW(mon_hwnd_, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
    if(!token)
    {
        CloseHandle(device_handle);
        return;
    }

    existing_notifications_.insert(std::make_pair(reinterpret_cast<size_t>(device_handle), std::make_pair(token, letter)));

    if(!on_device_added_.empty())
        on_device_added_(static_cast<char>(letter));
}

void usb_monitor::unmount_device(wchar_t letter, bool call_unsafe_callback)
{
    for(notifications::iterator handle_it = existing_notifications_.begin(); handle_it != existing_notifications_.end(); ++handle_it)
    {
        if((*handle_it).second.second == letter)
        {
            UnregisterDeviceNotification((*handle_it).second.first);
            CloseHandle(reinterpret_cast<HANDLE>((*handle_it).first));
            existing_notifications_.erase(handle_it);
            if(call_unsafe_callback && !on_device_removed_.empty())
                on_device_removed_(static_cast<char>(letter));

            break;
        }
    }
}

void usb_monitor::unmount_all_devices()
{
    std::set<wchar_t> devices(get_flash_disks(monitor_hard_drives_));
    for(std::set<wchar_t>::const_iterator it = devices.begin(); it != devices.end(); ++it)
        unmount_device(*it, false);
}

void usb_monitor::mount_existing_devices()
{
    std::set<wchar_t> devices(get_flash_disks(monitor_hard_drives_));
    for(std::set<wchar_t>::const_iterator it = devices.begin(); it != devices.end(); ++it)
        mount_device(*it);
}

void usb_monitor::on_device_add()
{
    on_device_added_.clear();
}

void usb_monitor::on_device_remove()
{
    on_device_removed_.clear();
}

void usb_monitor::on_device_safe_remove()
{
    on_device_safe_removed_.clear();
}

void usb_monitor::on_device_remove_fail()
{
    on_device_remove_failed_.clear();
}

usb_monitor* usb_monitor::create(bool monitor_hard_drives)
{
    if(!instanse_.get())
        instanse_.reset(new usb_monitor(monitor_hard_drives));

    return instanse_.get();
}

void usb_monitor::remove()
{
    instanse_.reset(0);
}

usb_monitor::~usb_monitor()
{
    started_ = false;

    SetWindowLongPtrW(mon_hwnd_, GWLP_USERDATA, 0);
    DestroyWindow(mon_hwnd_);
    UnregisterClassW(class_name.c_str(), GetModuleHandleW(NULL));

    for(notifications::iterator handle_it = existing_notifications_.begin(); handle_it != existing_notifications_.end(); ++handle_it)
    {
        UnregisterDeviceNotification((*handle_it).second.first);
        CloseHandle(reinterpret_cast<HANDLE>((*handle_it).first));
    }
}

std::set<wchar_t> usb_monitor::get_flash_disks(bool include_usb_hard_drives)
{
    std::set<wchar_t> devices;

    unsigned int disks = GetLogicalDrives();

    wchar_t drive_root[] = L"?:";

    for(int i = 31; i >= 0; i--)
    {
        if(disks & (1 << i))
        {
            drive_root[0] = static_cast<wchar_t>('A') + i;
            DWORD type = GetDriveTypeW(drive_root);

            if(type == DRIVE_REMOVABLE)
            {
                wchar_t buf[MAX_PATH];
                if(QueryDosDeviceW(drive_root, buf, MAX_PATH))
                    if(std::wstring(buf).find(L"\\Floppy") == std::wstring::npos)
                        devices.insert(static_cast<wchar_t>('A') + i);
            }
            else if(type == DRIVE_FIXED && include_usb_hard_drives)
            {
                try
                {
                    device_info info(get_device_info('A' + i));
                    HDEVINFO dev_info = SetupDiGetClassDevsW(&info.dev_class, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

                    if(dev_info != INVALID_HANDLE_VALUE)
                    {
                        SP_DEVINFO_DATA dev_data;
                        dev_data.cbSize = sizeof(dev_data);
                        if(SetupDiEnumDeviceInfo(dev_info, info.dev_number, &dev_data))
                        {
                            DWORD properties;
                            if(SetupDiGetDeviceRegistryPropertyW(dev_info, &dev_data, SPDRP_REMOVAL_POLICY, NULL, reinterpret_cast<PBYTE>(&properties), sizeof(properties), NULL)
                                &&
                                properties != CM_REMOVAL_POLICY_EXPECT_NO_REMOVAL)
                                devices.insert(static_cast<wchar_t>('A') + i);
                        }

                        SetupDiDestroyDeviceInfoList(dev_info);
                    }
                }
                catch(const usb_monitor_exception&)
                {
                }
            }
        }
    }

    return devices;
}

void usb_monitor::message_pump()
{
    MSG msg;
    int retVal;

    while((retVal = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if(retVal == -1)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void usb_monitor::start()
{
    started_ = true;
}

void usb_monitor::stop()
{
    started_ = false;
}


const usb_monitor::device_info usb_monitor::get_device_info(char letter)
{
    wchar_t volume_access_path[] = L"\\\\.\\X:";
    volume_access_path[4] = static_cast<wchar_t>(letter);

    HANDLE vol = CreateFileW(volume_access_path, 0,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if(vol == INVALID_HANDLE_VALUE)
        throw usb_monitor_exception("Cannot open device");


    STORAGE_DEVICE_NUMBER sdn;
    DWORD bytes_ret = 0;
    long DeviceNumber = -1;

    if(DeviceIoControl(vol,
        IOCTL_STORAGE_GET_DEVICE_NUMBER,
        NULL, 0, &sdn, sizeof(sdn),
        &bytes_ret, NULL))
        DeviceNumber = sdn.DeviceNumber;

    CloseHandle(vol);
    if(DeviceNumber == -1)
        throw usb_monitor_exception("Cannot get device number");

    wchar_t devname[] = L"?:";
    wchar_t devpath[] = L"?:\\";
    devname[0] = static_cast<wchar_t>(letter);
    devpath[0] = static_cast<wchar_t>(letter);
    wchar_t dos_name[MAX_PATH + 1];
    if(!QueryDosDeviceW(devname, dos_name, MAX_PATH))
        throw usb_monitor_exception("Cannot get device info");

    bool floppy = std::wstring(dos_name).find(L"\\Floppy") != std::wstring::npos;
    UINT drive_type = GetDriveTypeW(devpath);

    const GUID* guid;

    switch(drive_type)
    {
    case DRIVE_REMOVABLE:
        if(floppy)
            guid = &GUID_DEVINTERFACE_FLOPPY;
        else
            guid = &GUID_DEVINTERFACE_DISK;
        break;

    case DRIVE_FIXED:
        guid = &GUID_DEVINTERFACE_DISK;
        break;

    case DRIVE_CDROM:
        guid = &GUID_DEVINTERFACE_CDROM;
        break;

    default:
        throw usb_monitor_exception("Unknown device");
    }

    HDEVINFO dev_info = SetupDiGetClassDevsW(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if(dev_info == INVALID_HANDLE_VALUE)
        throw usb_monitor_exception("Cannot get device class");

    DWORD index = 0;
    BOOL ret = FALSE;

    BYTE buf[1024];
    PSP_DEVICE_INTERFACE_DETAIL_DATA_W pspdidd = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(buf);
    SP_DEVICE_INTERFACE_DATA spdid;
    SP_DEVINFO_DATA spdd;
    DWORD size;

    spdid.cbSize = sizeof(spdid);

    bool found = false;

    while(true)
    {
        ret = SetupDiEnumDeviceInterfaces(dev_info, NULL, guid, index, &spdid);
        if(!ret)
            break;

        size = 0;
        SetupDiGetDeviceInterfaceDetailW(dev_info, &spdid, NULL, 0, &size, NULL);

        if(size != 0 && size <= sizeof(buf))
        {
            pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!

            ZeroMemory(reinterpret_cast<PVOID>(&spdd), sizeof(spdd));
            spdd.cbSize = sizeof(spdd);

            BOOL res = SetupDiGetDeviceInterfaceDetailW(dev_info, &spdid, pspdidd, size, &size, &spdd);
            if(res)
            {
                HANDLE drive = CreateFileW(pspdidd->DevicePath, 0,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING, 0, NULL);
                if(drive != INVALID_HANDLE_VALUE)
                {
                    STORAGE_DEVICE_NUMBER sdn;
                    DWORD bytes_returned = 0;
                    if(DeviceIoControl(drive,
                        IOCTL_STORAGE_GET_DEVICE_NUMBER,
                        NULL, 0, &sdn, sizeof(sdn),
                        &bytes_returned, NULL))
                    {
                        if(DeviceNumber == static_cast<long>(sdn.DeviceNumber))
                        {
                            CloseHandle(drive);
                            found = true;
                            break;
                        }
                    }

                    CloseHandle(drive);
                }
            }
        }
        index++;
    }

    SetupDiDestroyDeviceInfoList(dev_info);

    if(!found)
        throw usb_monitor_exception("Cannot find device");

    DEVINST dev_parent = 0;
    if(CR_SUCCESS != CM_Get_Parent(&dev_parent, spdd.DevInst, 0))
        throw usb_monitor_exception("Cannot get device parent");

    device_info info;
    info.dev_class = *guid;
    info.dev_inst = dev_parent;
    info.dev_number = DeviceNumber;

    return info;
}

void usb_monitor::safe_eject(char letter)
{
    DEVINST dev = get_device_info(letter).dev_inst;

    //Проверим, не подконтролен ли нам этот девайс, и если это так, освободим его
    unmount_device(static_cast<wchar_t>(letter), false);

    if(CR_SUCCESS != CM_Request_Device_EjectW(dev, NULL, NULL, 0, 0))
        throw usb_monitor_exception("Cannot safe-eject device");
}

void usb_monitor::set_safe_remove_on_timeout(bool remove)
{
    safe_remove_on_timeout_ = remove;
}

bool usb_monitor::is_set_safe_remove_on_timeout_on() const
{
    return safe_remove_on_timeout_;
}

bool usb_monitor::is_started() const
{
    return started_;
}

void usb_monitor::enable_usb_hard_drive_monitoring(bool enable)
{
    monitor_hard_drives_ = enable;
}

bool usb_monitor::is_usb_hard_drive_monitoring_enabled() const
{
    return monitor_hard_drives_;
}
