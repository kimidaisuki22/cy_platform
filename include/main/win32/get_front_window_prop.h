#pragma once
#include <Windows.h>
#include <minwindef.h>
#include <optional>
#include <string>
namespace cy_platform {
inline std::optional<DWORD> get_pid_by_hwnd(HWND hwnd) {
  DWORD processId;
  auto thread_id = GetWindowThreadProcessId(hwnd, &processId);
  if (!thread_id) {
    return {};
  }
  return processId;
}

inline std::optional<std::string> get_exe_path_by_pid(DWORD processId) {
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                FALSE, processId);
  if (hProcess == NULL) {
    return {};
  }
  std::string path;
  path.resize(65535);
  DWORD size = path.size();
  BOOL result = QueryFullProcessImageName(hProcess, 0, path.data(), &size);
  CloseHandle(hProcess);
  if (!result) {
    return {};
  }
  path.resize(size);
  return path;
}
} // namespace cy_platform