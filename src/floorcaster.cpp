#include "floorcaster.hpp"

#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "util.hpp"

using namespace Config;
constexpr float floor_height = HEIGHT >> 1;
constexpr float half_fov = FOV >> 1;

void FloorCaster::CastFloor(unsigned *buffer) {
  for (auto x = 0; x < WIDTH; x++) {
    auto cast_dir = ((float)x / WIDTH * FOV - half_fov) * DEG2RAD;
    auto angle_x = GameState::player_rot + cast_dir;
    auto sin_x = std::sin(angle_x);
    auto cos_x = std::cos(angle_x);
    for (auto y = 0; y < floor_height; y++) {
      auto n = floor_height / (floor_height - y) / std::cos(cast_dir);
      auto hit_x = GameState::player_x + sin_x * n;
      auto hit_y = GameState::player_y + cos_x * n;
      if (((int)std::round(hit_x) % 2 != 0) == ((int)std::round(hit_y) % 2 != 0))
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0;
      else
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0xffffff;

      buffer[x + y * WIDTH] = 0x7fbfff + ((char)(64 * y / floor_height) << 16) +
                              ((char)(32 * y / floor_height) << 8);
    }
  }
}
