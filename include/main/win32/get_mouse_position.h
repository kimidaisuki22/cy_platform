#pragma once
#include <main/coord_2d_int.h>
#include <windows.h>
namespace cy_platform {
Coord_2d_int get_mouse_position() {
  POINT point{};
  GetCursorPos(&point);
  return {point.x, point.y};
}
} // namespace cy_platform