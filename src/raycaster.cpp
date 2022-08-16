#include "raycaster.hpp"

#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "util.hpp"

using namespace Config;
constexpr float floor_height = HEIGHT >> 1;
constexpr float half_fov = FOV >> 1;

static void CastFloor(unsigned *buffer) {
  for (auto x = 0; x < WIDTH; x++) {
    auto cast_dir = ((float)x / WIDTH * FOV - half_fov) * Math::deg2rad;
    auto angle_x = GameState::player_rot + cast_dir;
    auto sin_x = Math::sin(angle_x);
    auto cos_x = Math::cos(angle_x);
    for (auto y = 0; y < floor_height; y++) {
      auto n = floor_height / (floor_height - y) / Math::cos(cast_dir);
      auto hit_x = GameState::player_x + sin_x * n;
      auto hit_y = GameState::player_y + cos_x * n;
      if ((Math::round(hit_x) % 2 != 0) == (Math::round(hit_y) % 2 != 0))
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0xff000000;
      else
        buffer[x + (HEIGHT - y - 1) * WIDTH] = 0xffffffff;

      buffer[x + y * WIDTH] = 0xffffbf7f +
                              ((char)(64 * y / floor_height) << 0) +
                              ((char)(32 * y / floor_height) << 8);
    }
  }
}

void Raycaster::Render(unsigned *buffer) { CastFloor(buffer); }
