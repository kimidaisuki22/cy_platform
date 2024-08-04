#pragma once
#include <optional>
#include <string>
#include <windows.h>
#include <winuser.h>
namespace cy_platform {
std::optional<std::string> get_front_window_title() {
  auto hwnd = GetForegroundWindow();
  if (!hwnd) {
    return {};
  }
  std::string title{};
  title.resize(256);
  int n = GetWindowTextA(hwnd, title.data(), title.size());
  if (n <= 0) {
    return {};
  }
  title.resize(n);
  return title;
}
} // namespace cy_platform