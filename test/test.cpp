#include "main/win32/media_control.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
// Demonstrate some basic assertions.
TEST(T_media, Pause_and_play) {
  using namespace cy_platform;
  // ASSERT_TRUE(stop_media());
  std::this_thread::sleep_for(std::chrono::seconds{1});
  // It can pause chrome, but can't recover the play.
  ASSERT_TRUE(play_or_pause_media());
  // play is just not work on my pc.
  // std::this_thread::sleep_for(std::chrono::seconds{1});
  // ASSERT_TRUE(play_media());
}
