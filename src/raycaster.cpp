#include "raycaster.hpp"

#include "color.h"
#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "util.hpp"

#define buffer Renderer::screen_buffer

using namespace Config;
constexpr float floor_height = HEIGHT >> 1;
constexpr float half_fov = FOV >> 1;

constexpr auto black = Color::FromHex(0x000000);

constexpr auto floor1 = Color::FromHex(0xeeeeee);
constexpr auto floor2 = Color::FromHex(0xffffff);

constexpr auto sky1 = Color::FromHex(0xffffff);
constexpr auto sky2 = Color::FromHex(0x7fbfff);

template <int y>
consteval float Shade(float intensity) {
  return (1 - intensity) + intensity * (1.0f - (float)(y / floor_height));
}

template <int Y = 0>
struct FloorCaster {
  static constexpr auto ray_len = floor_height / (floor_height - Y);
  static constexpr auto floor_offset = (HEIGHT - Y - 1) * WIDTH;
  static constexpr auto sky_offset = Y * WIDTH;
  static constexpr auto sky_shade = Shade<Y>(0.5);
  static constexpr auto floor_shade = Shade<Y>(0.2);
  static constexpr auto sky_shaded = Color::Blend(sky1, sky2, sky_shade);
  static constexpr auto floor1_shaded =
    Color::Blend(black, floor1, floor_shade);
  static constexpr auto floor2_shaded =
    Color::Blend(black, floor2, floor_shade);

  inline static void CastFloor(int x, float sin_x, float cos_x, float cos_dir) {
    const auto normalized_len = ray_len / cos_dir;
    const auto hit_x = GameState::player_x + sin_x * normalized_len;
    const auto hit_y = GameState::player_y + cos_x * normalized_len;
    const auto round_hit_x = Math::round(hit_x);
    const auto round_hit_y = Math::round(hit_y);
    // const auto abs_hit_x = Math::abs(hit_x);
    // const auto abs_hit_y = Math::abs(hit_y);
    // const auto hit_off_x = abs_hit_x - (int)abs_hit_x;
    // const auto hit_off_y = abs_hit_y - (int)abs_hit_y;

    if ((round_hit_x % 2 != 0) == (round_hit_y % 2 != 0))
      buffer[x + floor_offset] = floor1_shaded;
    else
      buffer[x + floor_offset] = floor2_shaded;

    buffer[x + sky_offset] = sky_shaded;

    if constexpr (Y < floor_height - 1) {
      FloorCaster<Y + 1>::CastFloor(x, sin_x, cos_x, cos_dir);
    }
  }
};

void Raycaster::Render() {
  for (auto x = 0; x < WIDTH; x++) {
    const auto cast_dir = ((float)x / WIDTH * FOV - half_fov) * Math::deg2rad;
    const auto angle_x = GameState::player_rot + cast_dir;
    const auto sin_x = Math::sin(angle_x);
    const auto cos_x = Math::cos(angle_x);
    const auto cos_dir = Math::cos(cast_dir);
    FloorCaster<0>::CastFloor(x, sin_x, cos_x, cos_dir);
  }
}
