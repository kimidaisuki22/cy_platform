#include <cassert>
#include <highlevelmonitorconfigurationapi.h>
#include <iostream>
#include <main/main.h>
#include <minwindef.h>
#include <physicalmonitorenumerationapi.h>
#include <string>
#include <synchapi.h>
#include <windows.h>
#include <winnt.h>

#pragma comment(lib, "Dxva2.lib")
std::string utf16_to_utf8(const std::wstring &utf16_string) {
  if (utf16_string.empty()) {
    return std::string();
  }

  // Calculate the size of the destination buffer
  int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string.data(), -1,
                                      nullptr, 0, nullptr, nullptr);
  if (utf8_size == 0) {
    throw std::runtime_error("Failed to compute UTF-8 string length.");
  }

  // Create a buffer to store UTF-8 string
  std::string utf8_string(utf8_size, 0);

  // Convert from UTF-16 to UTF-8
  int convert_result =
      WideCharToMultiByte(CP_UTF8, 0, utf16_string.data(), -1, &utf8_string[0],
                          utf8_size, nullptr, nullptr);
  if (convert_result == 0) {
    throw std::runtime_error("Failed to convert UTF-16 string to UTF-8.");
  }

  // Remove the null terminator
  utf8_string.resize(utf8_size - 1);

  return utf8_string;
}
class Physical_monitor {
public:
  Physical_monitor(HANDLE handle, std::wstring native_name)
      : monitor_(handle), name_(native_name) {}
  void *get_handle() { return monitor_; }
  std::string get_name() const { return utf16_to_utf8(name_); }
  struct Brightness {
    DWORD min_;
    DWORD current_;
    DWORD max_;

    bool ok_{};
  };
  Brightness get_current_brightness() {
    Brightness brightness{};
    auto ok = GetMonitorBrightness(monitor_, &brightness.min_,
                                   &brightness.current_, &brightness.max_);
    brightness.ok_ = ok;
    return brightness;
  }
  bool set_brightness(DWORD brightness) {
    return SetMonitorBrightness(monitor_, brightness);
  }
  HANDLE monitor_{};
  std::wstring name_;
};
void change_monitor_brightness(DWORD new_brightness) {
  // Get the handle to the monitor
  HMONITOR h_monitor =
      MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);
  // GetForegroundWindow
  // Get the number of physical monitors
  DWORD monitor_count = 0;
  if (!GetNumberOfPhysicalMonitorsFromHMONITOR(h_monitor, &monitor_count)) {
    std::cerr << "Error getting number of physical monitors.\n";
    return;
  }

  // Allocate array to hold physical monitor handles
  LPPHYSICAL_MONITOR physical_monitors =
      (LPPHYSICAL_MONITOR)malloc(monitor_count * sizeof(PHYSICAL_MONITOR));
  if (!physical_monitors) {
    std::cerr << "Error allocating memory for physical monitors.\n";
    return;
  }

  // Get the physical monitor handles
  if (!GetPhysicalMonitorsFromHMONITOR(h_monitor, monitor_count,
                                       physical_monitors)) {
    std::cerr << "Error getting physical monitors.\n";
    free(physical_monitors);
    return;
  }

  // Set the brightness for each monitor
  for (DWORD i = 0; i < monitor_count; i++) {
    Physical_monitor monitor{physical_monitors[i].hPhysicalMonitor,
                             physical_monitors[i].szPhysicalMonitorDescription};
    auto cu = monitor.get_current_brightness().current_;
    if (!monitor.set_brightness(new_brightness)) {
      std::cerr << "Error setting brightness for monitor " << i << ".\n";
    }
    std::cout << "Change monitor " << i << " " << monitor.get_name() << ": "
              << cu << " -> " << new_brightness << " -> " << cu << "\n";
    Sleep(3000);
    monitor.set_brightness(cu);
  }

  // Clean up
  if (!DestroyPhysicalMonitors(monitor_count, physical_monitors)) {
    std::cerr << "Error destroying physical monitors.\n";
  }

  free(physical_monitors);
}

int main() {
  DWORD brightness = 50; // Set desired brightness (0-100)
  change_monitor_brightness(brightness);
  return 0;
}
