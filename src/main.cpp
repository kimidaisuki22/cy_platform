#include "main/monitor_brightness.h"
#include <fileapi.h>
#include <minwindef.h>
#include <string>
#include <string_view>
void test_brightness() {
  DWORD brightness = 50; // Set desired brightness (0-100)
  change_monitor_brightness(brightness);
}
#include <iostream>
void test_serial_port(int n) {
  HANDLE file_handle;
  bool is_success;
  std::string com_path =
      "COM" + std::to_string(n); //  Most systems have a COM1 port

  //  Open a handle to the specified com port.
  file_handle = CreateFileA(com_path.c_str(), GENERIC_READ | GENERIC_WRITE,
                            0,    //  must be opened with exclusive-access
                            NULL, //  default security attributes
                            OPEN_EXISTING, //  must use OPEN_EXISTING
                            0,             //  not overlapped I/O
                            NULL); //  hTemplate must be NULL for comm devices

  if (file_handle == INVALID_HANDLE_VALUE) {
    //  Handle the error.
    printf("CreateFile failed with error %d.\n", GetLastError());
    throw;
  }

  DCB dcb{};
  dcb.DCBlength = sizeof(DCB);

  //  Build on the current configuration by first retrieving all current
  //  settings.
  is_success = GetCommState(file_handle, &dcb);

  if (!is_success) {
    //  Handle the error.
    printf("GetCommState failed with error %d.\n", GetLastError());
    throw;
  }

  //  Fill in some DCB values and set the com state:
  //  57,600 bps, 8 data bits, no parity, and 1 stop bit.
  dcb.BaudRate = CBR_57600;  //  baud rate
  dcb.ByteSize = 8;          //  data size, xmit and rcv
  dcb.Parity = NOPARITY;     //  parity bit
  dcb.StopBits = ONESTOPBIT; //  stop bit

  is_success = SetCommState(file_handle, &dcb);

  if (!is_success) {
    //  Handle the error.
    printf("SetCommState failed with error %d.\n", GetLastError());
    throw;
  }

  //  Get the comm config again.
  is_success = GetCommState(file_handle, &dcb);

  if (!is_success) {
    //  Handle the error.
    printf("GetCommState failed with error %d.\n", GetLastError());
    throw;
  }
  char buffer[1024];
  DWORD size{};
  bool ok{};
  do {
    ok = ReadFile(file_handle, buffer, sizeof buffer, &size, nullptr);
    std::cout << std::string_view{buffer, size};
    std::cout << "AAA\n";
  } while (ok);
}
int main() {
  test_serial_port(5);
  return 0;
}
