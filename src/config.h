#pragma once

#ifndef USE_WASM
#define USE_WASM false
#endif

namespace Config {

namespace Level {
constexpr int WIDTH = 50;
constexpr int HEIGHT = 50;
}  // namespace Level

namespace Display {
constexpr int WIDTH = 960;
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
