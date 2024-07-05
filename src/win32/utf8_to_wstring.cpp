#include "main/win32/utf8_to_wstring.h"
#include <Windows.h>

namespace cy_platform::win32 {
std::wstring utf8_to_wstring(std::string_view str) {
  int len = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), NULL, 0);
  wchar_t *buffer = new wchar_t[len];
  MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), buffer, len);
  std::wstring wstr(buffer);
  delete[] buffer;
  return wstr;
}
} // namespace cy_platform::win32