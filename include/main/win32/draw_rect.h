#pragma once
#include <Windows.h>
#include <windef.h>

namespace cy_platform {
void draw_rect(RECT rect) {
  // Get the device context of the desktop
  HDC hdc = GetDC(NULL); // Passing NULL gets the DC for the entire screen

  // Create a red pen to draw the rectangle
  HPEN hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0)); // 5-pixel thick red pen
  HGDIOBJ hOldPen = SelectObject(hdc, hPen);

  // Set the brush to hollow to only draw the outline
  HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
  HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

  // Draw the rectangle
  Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

  // Restore the original pen and brush
  SelectObject(hdc, hOldPen);
  SelectObject(hdc, hOldBrush);

  // Clean up
  DeleteObject(hPen);
  ReleaseDC(NULL, hdc);
}
} // namespace cy_platform