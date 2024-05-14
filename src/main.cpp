#include <highlevelmonitorconfigurationapi.h>
#include <iostream>
#include <main/main.h>
#include <minwindef.h>
#include <physicalmonitorenumerationapi.h>
#include <synchapi.h>
#include <windows.h>

#pragma comment(lib, "Dxva2.lib")
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
    DWORD mi, cu, ma;
    GetMonitorBrightness(physical_monitors[i].hPhysicalMonitor, &mi, &cu, &ma);
    if (!SetMonitorBrightness(physical_monitors[i].hPhysicalMonitor,
                              new_brightness)) {
      std::cerr << "Error setting brightness for monitor " << i << ".\n";
    }
    std::cout << "Change monitor " << i << ": " << cu << " -> "
              << new_brightness << " -> " << cu << "\n";
    Sleep(3000);
    SetMonitorBrightness(physical_monitors[i].hPhysicalMonitor, cu);
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
