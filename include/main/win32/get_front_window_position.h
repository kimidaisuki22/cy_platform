#pragma once
#include <main/coord_2d_int.h>
#include <windows.h>
#include <winuser.h>
namespace cy_platform {
Coord_2d_int get_front_window_position() {
  auto hwnd = GetForegroundWindow();
  if (!hwnd) {
    return {};
  }
  RECT rect{};
  if (!GetWindowRect(hwnd, &rect)) {
    return {};
  }
  return {rect.left, rect.top};
}
} // namespace cy_platform