#pragma once

#ifndef USE_WASM
#define USE_WASM false
#endif

namespace Config {

namespace Level {
constexpr int WIDTH = 15;
constexpr int HEIGHT = 15;
}  // namespace Level

namespace Display {
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 960;

// TODO: Implement constexpr atan and stop hardcoding this
constexpr float VFOV = 73.73979529168803;
constexpr float FOV = 90;
}  // namespace Display

namespace Player {
// The speed of the player in tile/s
constexpr float SPEED = 2.0;
constexpr float RADIUS = 0.2;
}  // namespace Player

}  // namespace Config
