#pragma once

#ifndef USE_WASM
#define USE_WASM false
#endif

#include "util.hpp"

namespace Config {

namespace Level {
constexpr int WIDTH = 10;
constexpr int HEIGHT = 10;

constexpr int TEST_MAP[WIDTH * HEIGHT] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
  1, 0, 0, 0, 1, 1, 0, 0, 0, 1,  //
  1, 0, 0, 0, 1, 0, 0, 1, 0, 1,  //
  1, 1, 0, 0, 1, 1, 1, 1, 0, 1,  //
  1, 1, 0, 0, 0, 0, 0, 0, 0, 1,  //
  1, 1, 0, 1, 0, 0, 1, 0, 0, 1,  //
  1, 0, 0, 0, 0, 0, 0, 1, 0, 1,  //
  1, 0, 0, 0, 0, 1, 0, 0, 0, 1,  //
  1, 0, 0, 0, 0, 1, 0, 0, 1, 1,  //
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
};
}  // namespace Level

namespace Display {
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 960;

constexpr float FOV = 90;

// vfov = 360 * atan(tan(hfov * pi / 360) * height / width) / pi
constexpr float VFOV =
  360 * Math::atan(Math::tan(FOV * Math::pi / 360.0) * HEIGHT / WIDTH) /
  Math::pi;
constexpr float RENDER_DISTANCE = 200;
}  // namespace Display

namespace Player {
// The speed of the player in tile/s
constexpr float SPEED = 2.0;
constexpr float RADIUS = 0.2;
}  // namespace Player

}  // namespace Config
