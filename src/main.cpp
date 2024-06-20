#include "main/monitor_brightness.h"
#include "main/turn_off_screen.h"
#include <cstdint>
#include <cstring>
#include <minwindef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <winbase.h>
void test_brightness() {
  DWORD brightness = 50; // Set desired brightness (0-100)
  change_monitor_brightness(brightness);
}

#include "main/network_speed.h"
#include "main/serial_port.h"
int main(int argc, char **argv) {
  turn_off_screen();
  // network_speed_monitor_test();
}
