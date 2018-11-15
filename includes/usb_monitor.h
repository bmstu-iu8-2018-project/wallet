#ifndef USB_MONITOR_H
#define USB_MONITOR_H

#pragma once
#include <memory>
#include <stdexcept>
#include <set>
#include <map>
#include <string>
#include <Windows.h>
#include <cfgmgr32.h>
#include <boost/function.hpp>

class usb_monitor_exception : public std::runtime_error
{
public:
	explicit usb_monitor_exception(const std::string& message);
};

class usb_monitor
{
public:
	//Создает единственный экземпляр класса usb_monitor
	//monitor_hard_drives - опция отслеживания внешних жестких дисков
	static usb_monitor* create(bool monitor_hard_drives = false);

	//Удалить экземпляр класса из памяти
	static void remove();

	//Добавляет коллбек, вызываемый при добавлении нового USB flash-диска
	//Формат коллбека: void(char letter), где letter - буква добавившегося диска
	template<typename Handler>
	void on_device_add(Handler handler)
	{
		on_device_added_ = handler;
	}

	//Функции для удаления существующих коллбеков
	void on_device_add();

	//Стартует отслеживание USB
	void start();
	//Останавливает отслеживание USB
	void stop();
	//Запущено ли отслеживание USB
	bool is_started() const;
	
	//Получить буквы всех USB flash-дисков, имеющихся в системе в данный момент времени
	//Если include_usb_hard_drives == true, то в список попадут буквы внешних жестких дисков,
	//в противном случае - только флешки
	static std::set<wchar_t> get_flash_disks(bool include_usb_hard_drives);
	
	//Взять под контроль существующие USB-флешки
	//Если устройство уже было замонтировано, ничего не произойдет
	//Для каждого замонтированного устройства будет вызван коллбек on_device_add
	void mount_existing_devices();

	//Вспомогательная функция для консольных приложений
	static void message_pump();

	//Деструктор
	~usb_monitor();

private:
	explicit usb_monitor(bool monitor_hard_drives);
	usb_monitor(const usb_monitor&);
	usb_monitor& operator=(const usb_monitor&);

	bool started_, safe_remove_on_timeout_, monitor_hard_drives_;
	HWND mon_hwnd_;
	
	std::set<wchar_t> existing_usb_devices_;
	typedef std::map<size_t, std::pair<HDEVNOTIFY, wchar_t> > notifications;
	notifications existing_notifications_;

	static INT_PTR WINAPI WinProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static std::auto_ptr<usb_monitor> instanse_;

	void detect_changed_devices();
	void mount_device(wchar_t letter);
	void unmount_device(wchar_t letter, bool call_unsafe_callback = true);
	BOOL devices_changed(WPARAM wParam, LPARAM lParam);

	struct device_info
	{
		DEVINST dev_inst;
		GUID dev_class;
		long dev_number;
	};

	static const device_info get_device_info(char letter);

	boost::function<void(char)> on_device_added_;

	static const std::wstring class_name;
};

#endif // USB_MONITOR_H
