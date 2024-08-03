#include "main/win32/send_keyboard.h"
#include "Windows.h"
namespace cy_platform {
bool send_keyboard_key(int vk) {
  INPUT input;
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = VK_MEDIA_STOP;
  input.ki.wScan = 0;
  input.ki.dwFlags = 0;
  input.ki.time = 0;
  input.ki.dwExtraInfo = 0;

  if (SendInput(1, &input, sizeof(INPUT)) != 1) {
    return false;
  }

  input.ki.dwFlags = KEYEVENTF_KEYUP;
  if (SendInput(1, &input, sizeof(INPUT)) != 1) {
    return false;
  }
  return true;
}
}; // namespace cy_platform