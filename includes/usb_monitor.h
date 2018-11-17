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
	//Ñîçäàåò åäèíñòâåííûé ýêçåìïëÿð êëàññà usb_monitor
	//monitor_hard_drives - îïöèÿ îòñëåæèâàíèÿ âíåøíèõ æåñòêèõ äèñêîâ
	static usb_monitor* create(bool monitor_hard_drives = false);

	//Óäàëèòü ýêçåìïëÿð êëàññà èç ïàìÿòè
	static void remove();

	//Äîáàâëÿåò êîëëáåê, âûçûâàåìûé ïðè äîáàâëåíèè íîâîãî USB flash-äèñêà
	//Ôîðìàò êîëëáåêà: void(char letter), ãäå letter - áóêâà äîáàâèâøåãîñÿ äèñêà
	template<typename Handler>
	void on_device_add(Handler handler)
	{
		on_device_added_ = handler;
	}

	//Ôóíêöèè äëÿ óäàëåíèÿ ñóùåñòâóþùèõ êîëëáåêîâ
	void on_device_add();

	//Ñòàðòóåò îòñëåæèâàíèå USB
	void start();
	//Îñòàíàâëèâàåò îòñëåæèâàíèå USB
	void stop();
	//Çàïóùåíî ëè îòñëåæèâàíèå USB
	bool is_started() const;
	
	//Ïîëó÷èòü áóêâû âñåõ USB flash-äèñêîâ, èìåþùèõñÿ â ñèñòåìå â äàííûé ìîìåíò âðåìåíè
	//Åñëè include_usb_hard_drives == true, òî â ñïèñîê ïîïàäóò áóêâû âíåøíèõ æåñòêèõ äèñêîâ,
	//â ïðîòèâíîì ñëó÷àå - òîëüêî ôëåøêè
	static std::set<wchar_t> get_flash_disks(bool include_usb_hard_drives);
	
	//Âçÿòü ïîä êîíòðîëü ñóùåñòâóþùèå USB-ôëåøêè
	//Åñëè óñòðîéñòâî óæå áûëî çàìîíòèðîâàíî, íè÷åãî íå ïðîèçîéäåò
	//Äëÿ êàæäîãî çàìîíòèðîâàííîãî óñòðîéñòâà áóäåò âûçâàí êîëëáåê on_device_add
	void mount_existing_devices();

	//Âñïîìîãàòåëüíàÿ ôóíêöèÿ äëÿ êîíñîëüíûõ ïðèëîæåíèé
	static void message_pump();

	//Äåñòðóêòîð
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
