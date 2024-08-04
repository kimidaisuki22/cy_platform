#include "main/monitor_brightness.h"
#include "main/turn_off_screen.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <minwindef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <winbase.h>
void test_brightness() {
  DWORD brightness = 50; // Set desired brightness (0-100)
  change_monitor_brightness(brightness);
}

#include "main/network_speed.h"
#include "main/serial_port.h"
#include <main/win32/get_front_window_position.h>
#include <main/win32/get_front_window_title.h>
#include <main/win32/get_mouse_position.h>
int main(int argc, char **argv) {
  // turn_off_screen();
  // network_speed_monitor_test();
  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
  // Get the height of the title bar
  int titleBarHeight =
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYEDGE) * 2;

  // Get the width of the border
  int borderWidth = GetSystemMetrics(SM_CXFRAME);

  // Get the height of the border
  int borderHeight = GetSystemMetrics(SM_CYFRAME);

  printf("Title Bar Height: %d pixels\n", titleBarHeight);
  printf("Border Width: %d pixels\n", borderWidth);
  printf("Border Height: %d pixels\n", borderHeight);

  while (true) {
    auto [x, y] = cy_platform::get_mouse_position();
    auto [w_x, w_y] = cy_platform::get_front_window_position();
    auto title_text =
        std::format("{}, {} - {}, {} [{} {}] {}", x, y, w_x, w_y, x - w_x,
                    y - w_y - titleBarHeight,
                    cy_platform::get_front_window_title().value_or("[FAILED]"));
    std::cout << std::format("{:120}\r", title_text);
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }
}
