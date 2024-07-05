#pragma once

#include <string_view>
namespace cy_platform::win32 {
std::wstring utf8_to_wstring(std::string_view str);
} // namespace cy_platform::win32