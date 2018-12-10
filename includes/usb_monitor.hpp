#ifndef USB_MONITOR_HPP
#define USB_MONITOR_HPP
#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <map>
#include <stdexcept>
#include <set>
#include <string>
#include <Windows.h>
#include <cfgmgr32.h>
#include <boost/function.hpp>

class usb_monitor_exception : public std::runtime_error {
 public:
  explicit usb_monitor_exception(const std::string& message);
};

class usb_monitor {
 public:
  // Creates a single instance of the class usb_monitor
  // monitor_hard_drives - option to monitor external hard drives
  static usb_monitor* create(bool monitor_hard_drives = false);

  // Remove class instance from memory
  static void remove();

  // Adds a callback called when adding a new USB flash disk
  // Callback format: void (char letter), where letter is the letter of the
  // added disk
  template <typename Handler>
  void on_device_add(Handler handler) {
    on_device_added_ = handler;
  }

  // Options for deleting existing callbacks
  void on_device_add();

  // USB tracking starts
  void start();
  // Stops USB tracking
  void stop();
  // USB tracking started
  bool is_started() const;

  // Get the letters of all USB flash drives currently on the system
  // If include_usb_hard_drives == true,
  // then the list will contain the letters of external hard drives,
  // otherwise, only flash drives
  static std::set<wchar_t> get_flash_disks(bool include_usb_hard_drives);

  // Take control of existing USB flash drives
  // If the device has already been mounted, nothing will happen
  // For each mounted device a callback will be called on_device_add
  void mount_existing_devices();

  // Helper function for console applications
  static void message_pump();

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

  static INT_PTR WINAPI WinProcCallback(HWND hWnd,
                                        UINT message,
                                        WPARAM wParam,
                                        LPARAM lParam);
  static std::auto_ptr<usb_monitor> instanse_;

  void detect_changed_devices();
  void mount_device(wchar_t letter);
  void unmount_device(wchar_t letter, bool call_unsafe_callback = true);
  BOOL devices_changed(WPARAM wParam, LPARAM lParam);

  struct device_info {
    DEVINST dev_inst;
    GUID dev_class;
    long dev_number;
  };

  static const device_info get_device_info(char letter);

  boost::function<void(char)> on_device_added_;

  static const std::wstring class_name;
};

#endif  // USB_MONITOR_HPP
