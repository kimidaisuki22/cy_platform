#include "main/monitor_brightness.h"
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

#include "main/serial_port.h"
int main(int argc, char **argv) {
  do {
    try {
      test_serial_port(std::stoi(argv[1]));
    } catch (...) {
    }
  } while (argc > 2);
  return 0;
}
