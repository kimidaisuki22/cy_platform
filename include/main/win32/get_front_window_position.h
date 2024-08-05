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
Coord_2d_int get_front_window_size() {
  auto hwnd = GetForegroundWindow();
  if (!hwnd) {
    return {};
  }
  RECT rect{};
  if (!GetWindowRect(hwnd, &rect)) {
    return {};
  }
  return {rect.right - rect.left, rect.bottom - rect.top};
}
} // namespace cy_platform