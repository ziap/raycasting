#include "floorcaster.hpp"

#include <cmath>

#include "config.h"
#include "gamestate.hpp"
#include "input.hpp"
#include "renderer.hpp"

using namespace Config;
constexpr float floor_height = HEIGHT / 2;

void FloorCaster::CastFloor(Uint32 *buffer) {
  for (auto x = 0; x < WIDTH; x++) {
    auto cast_dir = ((float)x / WIDTH * FOV - FOV / 2) * DEG2RAD;
    auto angle_x = GameState::player_rot + cast_dir;
    auto sin_x = std::sin(angle_x);
    auto cos_x = std::cos(angle_x);
    for (auto y = 0; y < floor_height; y++) {
      auto n =
        floor_height / (floor_height - y) / std::cos(cast_dir);
      auto hit_x = (int)round(GameState::player_x + sin_x * n);
      auto hit_y = (int)round(GameState::player_y + cos_x * n);
      if ((hit_x % 2 != 0) == (hit_y % 2 != 0))
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0;
      else
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0xffffff;
      
      buffer[x + y * WIDTH] = 0x7fbfff
      + ((Uint8)(64 * y / floor_height) << 16)
      + ((Uint8)(32 * y / floor_height) << 8);
        //= 0x7FBFFF;      
    }
  }
}
