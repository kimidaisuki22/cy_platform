cmake_minimum_required(VERSION 3.23.0)

option(WITH_BENCHMARK "Build benchmarks" OFF)
if(WITH_BENCHMARK)
  list(APPEND VCPKG_MANIFEST_FEATURES "benchmark")
endif()


option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "test")
endif()



project(main VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

add_library(cy_platform)

target_sources(cy_platform PRIVATE
src/win32/utf8_to_wstring.cpp
src/win32/media_control.cpp
src/win32/send_keyboard.cpp
)
add_executable(main src/main.cpp)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(main PRIVATE spdlog::spdlog cy_platform)

target_include_directories(cy_platform PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
if(WITH_BENCHMARK)
  add_subdirectory(benchmark)
endif()

install(TARGETS main DESTINATION bin)
# install(TARGETS $<TARGET_RUNTIME_DLLS:main> DESTINATION bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME update_server_package)
include(CPack)

include(GNUInstallDirs)
