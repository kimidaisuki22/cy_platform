#include "main/monitor_brightness.h"
#include <cstdint>
#include <cstring>
#include <fileapi.h>
#include <minwindef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <winbase.h>
void test_brightness() {
  DWORD brightness = 50; // Set desired brightness (0-100)
  change_monitor_brightness(brightness);
}
DCB init_dcb{};
#include <iostream>
void PrintCommState(DCB dcb) {
  printf("BaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d ev=%d "
         "on_ch=%d\n",
         dcb.BaudRate, dcb.ByteSize, dcb.Parity, dcb.StopBits, (int)dcb.EvtChar,
         (int)dcb.XonChar);
  // for (int i = 0; i < sizeof dcb; i++) {
  //   auto v = ((uint8_t *)&dcb)[i];
  //   printf("%d, ", static_cast<uint32_t>(v));
  // }
}
void fix_dcb(DCB &dcb) {
  uint8_t buffer[]{28, 0, 0, 0, 0, 194, 1, 0,   17,  19, 0, 0, 0, 0,
                   0,  0, 0, 0, 8, 0,   0, 204, 204, 0,  0, 0, 0, 0};
  memcpy(&dcb, buffer, sizeof(dcb));
}
void test_serial_port(int n) {
  HANDLE file_handle;
  bool is_success;
  std::string com_path =
      "\\\\.\\COM" + std::to_string(n); //  Most systems have a COM1 port

  //  Open a handle to the specified com port.
  file_handle = CreateFileA(com_path.c_str(), GENERIC_READ | GENERIC_WRITE,
                            0,    //  must be opened with exclusive-access
                            NULL, //  default security attributes
                            OPEN_EXISTING,        //  must use OPEN_EXISTING
                            FILE_FLAG_OVERLAPPED, //  not overlapped I/O
                            NULL); //  hTemplate must be NULL for comm devices

  if (file_handle == INVALID_HANDLE_VALUE) {
    static int count{};
    if (count++ == 0) {
      //  Handle the error.
      printf("CreateFile failed with error %d.\n", GetLastError());
    }
    throw std::runtime_error{"failed to connect"};
  }
  std::cout << "connected to " << com_path << "\n";
  { SetCommMask(file_handle, EV_BREAK); }

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
  PrintCommState(dcb);
  // static bool first_dcb{true};
  // if (first_dcb) {
  //   init_dcb = dcb;
  //   first_dcb = false;
  // } else {
  //   dcb = init_dcb;
  // }
  fix_dcb(dcb);
  PrintCommState(dcb);

  //  Fill in some DCB values and set the com state:
  //  bps, 8 data bits, no parity, and 1 stop bit.
  // dcb.BaudRate = CBR_9600; //  baud rate CBR_9600 CBR_115200
  // dcb.ByteSize = 8;          //  data size, xmit and rcv
  // dcb.Parity = NOPARITY;     //  parity bit
  // dcb.StopBits = ONESTOPBIT; //  stop bit
  // dcb.fBinary = true;
  // dcb.fParity = true;
  // dcb.fInX = true;
  // dcb.fOutX = true;
  // dcb.XonChar = (int)(-52);
  is_success = SetCommState(file_handle, &dcb);

  if (!is_success) {
    //  Handle the error.
    printf("SetCommState failed with error %d.\n", GetLastError());
    throw;
  }
  // // Set timeouts
  COMMTIMEOUTS timeout = {0};
  timeout.ReadIntervalTimeout = 1;
  // timeout.ReadTotalTimeoutConstant = 1;
  // timeout.ReadTotalTimeoutMultiplier = 1;
  // timeout.WriteTotalTimeoutConstant = 1;
  // timeout.WriteTotalTimeoutMultiplier = 1;

  SetCommTimeouts(file_handle, &timeout);
  // COMMTIMEOUTS timeout = {0};
  timeout = {};
  GetCommTimeouts(file_handle, &timeout);
  std::cout << timeout.ReadIntervalTimeout << "\n";
  std::cout << timeout.ReadTotalTimeoutConstant << "\n";
  std::cout << timeout.ReadTotalTimeoutMultiplier << "\n";
  std::cout << timeout.WriteTotalTimeoutConstant << "\n";
  std::cout << timeout.WriteTotalTimeoutMultiplier << "\n";
  //  Get the comm config again.
  is_success = GetCommState(file_handle, &dcb);

  if (!is_success) {
    //  Handle the error.
    printf("GetCommState failed with error %d.\n", GetLastError());
    throw;
  }
  PrintCommState(dcb);

  ClearCommError(file_handle, nullptr, nullptr);
  char buffer[1024];
  DWORD size{};
  bool ok{};
  do {
    int32_t r = 0;

    OVERLAPPED ovr = {0};
    DWORD recvd_ = 0;

    /* create event for the read completion */
    ovr.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (ovr.hEvent == NULL) {
    }

    std::cout << "start reading\n";
    ok = ReadFile(file_handle, buffer, sizeof buffer, &size, &ovr);
    if (!ok) {
      DWORD wr;

      wr = GetLastError();

      /* read pending, wait until completion */
      if (wr == ERROR_IO_PENDING) {
        wr = WaitForSingleObject(ovr.hEvent, 1000);
        switch (wr) {
        case WAIT_OBJECT_0:
          break;
        default:
          break;
        }
        ok = true;
      } else {
        ok = false;
      }
    }
    /* obtain results */
    if (GetOverlappedResult(file_handle, &ovr, &recvd_, FALSE) == FALSE) {
    }

    /* no more bytes available */
    if (recvd_ == 0) {
    }

    /* optionally store received bytes */
    if (recvd_ != NULL) {
      size = recvd_;
    }
    CloseHandle(ovr.hEvent);

    std::cout << std::string_view{buffer, size};
    // std::cout << "AAA\n";
  } while (ok);
}
int main(int argc, char **argv) {
  do {
    try {
      test_serial_port(std::stoi(argv[1]));
    } catch (...) {
    }
  } while (argc > 2);
  return 0;
}
