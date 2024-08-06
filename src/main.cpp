#include "main/monitor_brightness.h"
#include "main/turn_off_screen.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <minwindef.h>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
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
class Term {
public:
  void hide_cursor() { os << "\033[?25l"; }
  void clean_screen() { os << "\033[2J"; }
  void move_cursor(int x, int y) { os << std::format("\033[{};{}H", x, y); }

private:
  std::ostream &os = std::cout;
};
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
  auto line_up = [](int n) { std::cout << std::format("\033[{}A", n); };
  Term term;
  term.clean_screen();
  term.hide_cursor();
  term.move_cursor(0, 0);
  while (true) {
    auto [x, y] = cy_platform::get_mouse_position();
    auto [w_x, w_y] = cy_platform::get_front_window_position();
    auto [c_x, c_y] = cy_platform::get_mouse_position_in_front_window();
    auto [size_x, size_y] = cy_platform::get_front_window_size();
    std::vector<std::string> lines;
    lines.push_back(std::format("{}, {} - {}, {}", x, y, w_x, w_y));
    lines.push_back(std::format("[{} {}]", x - w_x, y - w_y - titleBarHeight));
    lines.push_back(std::format("client: [{} {}]", c_x, c_y));
    lines.push_back(std::format("client %: [{:.2} {:.2}]", c_x * 1.f / size_x,
                                c_y * 1.f / size_y));
    lines.push_back(std::format("size: [{} {}]", size_x, size_y));
    lines.push_back(std::format(
        "Title: '{}'",
        cy_platform::get_front_window_title().value_or("[FAILED]")));
    lines.push_back(
        std::format("bar height: {}", cy_platform::get_bar_height()));

    for (auto line : lines) {
      std::cout << std::format("{}\n\033[0K", line);
    }
    term.move_cursor(0, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }
}
