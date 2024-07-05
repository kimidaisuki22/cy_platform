#include "main/monitor_brightness.h"
#include "main/turn_off_screen.h"
#include <chrono>
#include <cstdint>
#include <cstring>
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
#include <main/win32/get_mouse_position.h>
int main(int argc, char **argv) {
  // turn_off_screen();
  // network_speed_monitor_test();

  while (true) {
    auto [x, y] = cy_platform::get_mouse_position();
    auto [w_x, w_y] = cy_platform::get_front_window_position();
    std::cout << x << ", " << y << " - " << w_x << ", " << w_y << "     \r";
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }
}
