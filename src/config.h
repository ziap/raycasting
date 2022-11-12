#pragma once

#ifndef USE_WASM
#define USE_WASM false
#endif

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
constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

// TODO: Implement constexpr atan and stop hardcoding this
// vfov = 360 * atan(tan(hfov * pi / 360) * height / width) / pi
constexpr float VFOV = 73.73979529168804;
constexpr float FOV = 90;
constexpr float RENDER_DISTANCE = 200;
}  // namespace Display

namespace Player {
// The speed of the player in tile/s
constexpr float SPEED = 2.0;
constexpr float RADIUS = 0.2;
}  // namespace Player

}  // namespace Config
