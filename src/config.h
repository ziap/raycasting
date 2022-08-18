#pragma once

#ifndef USE_WASM
#define USE_WASM false
#endif

namespace Config {

namespace Level {
constexpr int WIDTH = 10;
constexpr int HEIGHT = 10;
}  // namespace Level

namespace Display {
constexpr int WIDTH = 1080;
constexpr int HEIGHT = 720;
constexpr int FOV = 60;
}  // namespace Display

namespace Player {
// The speed of the player in tile/s
constexpr float SPEED = 2.0;
constexpr float RADIUS = 0.2;
constexpr float HEIGHT = 0.5;
}  // namespace Player

}  // namespace Config
