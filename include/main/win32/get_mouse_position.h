#pragma once
#include "draw_rect.h"
#include <main/coord_2d_int.h>
#include <windows.h>
#include <winuser.h>
namespace cy_platform {
Coord_2d_int get_mouse_position() {
  POINT point{};
  GetCursorPos(&point);
  return {point.x, point.y};
}
int get_bar_height() {
  auto window_handle = GetForegroundWindow();
  // RECT window_rect;
  // RECT window_rectangle, client_rectangle;
  // int height, width;
  // GetWindowRect(window_handle, &window_rectangle);
  // GetClientRect(window_handle, &client_rectangle);
  // height = (window_rectangle.bottom - window_rectangle.top) -
  //          (client_rectangle.bottom - client_rectangle.top);
  // width = (window_rectangle.right - window_rectangle.left) -
  //         (client_rectangle.right - client_rectangle.left);
  // return height - (width / 2);
  RECT wrect;
  auto hwnd = window_handle;
  GetWindowRect(hwnd, &wrect);
  RECT crect;
  GetClientRect(hwnd, &crect);
  POINT lefttop = {crect.left, crect.top}; // Practicaly both are 0
  ClientToScreen(hwnd, &lefttop);
  POINT rightbottom = {crect.right, crect.bottom};
  ClientToScreen(hwnd, &rightbottom);

  int left_border =
      lefttop.x - wrect.left; // Windows 10: includes transparent part
  int right_border = wrect.right - rightbottom.x;   // As above
  int bottom_border = wrect.bottom - rightbottom.y; // As above
  int top_border_with_title_bar =
      lefttop.y - wrect.top; // There is no transparent part
  return top_border_with_title_bar;
}
Coord_2d_int get_mouse_position_in_front_window() {
  auto abs = get_mouse_position();
  auto hwnd = GetForegroundWindow();
  RECT window_rect;
  GetWindowRect(hwnd, &window_rect);
  auto bar_height = get_bar_height();
  return {abs.x - window_rect.left, abs.y - bar_height - window_rect.top};
}
} // namespace cy_platform