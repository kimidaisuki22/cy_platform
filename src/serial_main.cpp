#include "main/monitor_brightness.h"
#include "main/turn_off_screen.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <minwindef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <winbase.h>

#include "main/serial_port.h"
#include <main/win32/get_front_window_position.h>
#include <main/win32/get_mouse_position.h>
int main(int argc, char **argv) {
  Serial_device device{std::stoi(argv[1])};
  auto move = [&device](int dx, int dy) {
    device.write(std::format("mouse(0,{},{},0,0)\r\n", dx, dy));
  };
  // 0 left, 1 right, 2 middle
  auto click = [&device](int button) {
    device.write(std::format("mouse({},0,0,0,0)\r\n", (1 << button)));
  };
  // for (int i = 0; i < 10; i++) {
  //   move(10, 10);
  //   std::this_thread::sleep_for(std::chrono::milliseconds{10});
  // }
  device.write("help()\r\n");
  std::cout << device.read(8) << "\n";
  std::cout << "Auto clicker\n";
  for (;;) {
    click(0);
    int t = 80 + (rand() * 1.f / RAND_MAX) * 30;
    std::this_thread::sleep_for(std::chrono::seconds{t});
  }
}
