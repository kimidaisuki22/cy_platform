#pragma once
#include <Windows.h>
#include <iostream>
#include <pdh.h>

#pragma comment(lib, "pdh.lib")

int network_speed_monitor_test() {
  PDH_HQUERY query;
  PDH_HCOUNTER counter;
  PDH_STATUS status;
  DWORD dwType;
  PDH_FMT_COUNTERVALUE counterValue;

  // Initialize the PDH query
  status = PdhOpenQuery(NULL, 0, &query);
  if (status != ERROR_SUCCESS) {
    std::cerr << "PdhOpenQuery failed: " << status << std::endl;
    return 1;
  }

  // Add the network interface counter to the query
  status = PdhAddCounterA(query, "\\Network Interface(*)\\Bytes Total/sec", 0,
                          &counter);
  if (status != ERROR_SUCCESS) {
    std::cerr << "PdhAddCounter failed: " << status << std::endl;
    PdhCloseQuery(query);
    return 1;
  }

  // Collect the data
  status = PdhCollectQueryData(query);
  if (status != ERROR_SUCCESS) {
    std::cerr << "PdhCollectQueryData failed: " << status << std::endl;
    PdhCloseQuery(query);
    return 1;
  }

  // Wait for a second to get a valid speed reading
  Sleep(1000);

  while (1) {
    // Collect the data again
    status = PdhCollectQueryData(query);
    if (status != ERROR_SUCCESS) {
      std::cerr << "PdhCollectQueryData failed: " << status << std::endl;
      PdhCloseQuery(query);
      return 1;
    }

    // Get the counter value
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_LONG, &dwType,
                                         &counterValue);
    if (status != ERROR_SUCCESS) {
      std::cerr << "PdhGetFormattedCounterValue failed: " << status
                << std::endl;
      PdhCloseQuery(query);
      return 1;
    }

    // Print the network speed in bytes per second
    std::cout << "Network speed: " << counterValue.longValue << " bytes/sec"
              << std::endl;
    Sleep(1000);
  }
  // Close the query
  PdhCloseQuery(query);

  return 0;
}