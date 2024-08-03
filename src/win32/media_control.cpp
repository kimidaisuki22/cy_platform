#include "main/win32/send_keyboard.h"
#include <Windows.h>
#include <winuser.h>
namespace cy_platform {
bool stop_media() { return send_keyboard_key(VK_MEDIA_STOP); }
bool play_or_pause_media() { return send_keyboard_key(VK_MEDIA_PLAY_PAUSE); }
bool play_media() { return send_keyboard_key(VK_PLAY); }
bool next_media() { return send_keyboard_key(VK_MEDIA_NEXT_TRACK); }
bool prev_media() { return send_keyboard_key(VK_MEDIA_PREV_TRACK); }
bool volume_up() { return send_keyboard_key(VK_VOLUME_UP); }
bool volume_down() { return send_keyboard_key(VK_VOLUME_DOWN); }
bool volume_mute() { return send_keyboard_key(VK_VOLUME_MUTE); }
} // namespace cy_platform