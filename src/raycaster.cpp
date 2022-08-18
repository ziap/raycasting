#include "raycaster.hpp"

#include "color.h"
#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "texture_floor.h"
#include "texture_lava.h"
#include "util.hpp"

#define buffer Renderer::screen_buffer

#define paint_texture(pixel, texture_type, off_x, off_y)               \
  {                                                                    \
    const auto texture_x = int(off_x * texture_type::width);           \
    const auto texture_y = int(off_y * texture_type::height);          \
    buffer[pixel] =                                                    \
      texture_type::data[texture_x + texture_y * texture_type::width]; \
  }

constexpr float floor_height = Config::Display::HEIGHT >> 1;
constexpr float half_fov = Config::Display::FOV >> 1;

constexpr auto sky1 = Color::FromHex(0xffffff);
constexpr auto sky2 = Color::FromHex(0x7fbfff);

template <int y>
consteval float Shade(float intensity) {
  return (1 - intensity) + intensity * (1.0f - (float)(y / floor_height));
}

template <int Y = 0>
struct FloorCaster {
  static constexpr auto ray_len = floor_height / (floor_height - Y);
  static constexpr auto floor_offset =
    (Config::Display::HEIGHT - Y - 1) * Config::Display::WIDTH;
  static constexpr auto sky_offset = Y * Config::Display::WIDTH;
  static constexpr auto sky_shade = Shade<Y>(0.5);
  static constexpr auto sky_shaded = Color::Blend(sky1, sky2, sky_shade);

  inline static void CastFloor(int x, float sin_x, float cos_x, float cos_fix) {
    const auto hit_x = GameState::player_x + sin_x * ray_len * cos_fix;
    const auto hit_y = GameState::player_y + cos_x * ray_len * cos_fix;
    const auto hit_tile_x = (int)hit_x - (hit_x < 0);
    const auto hit_tile_y = (int)hit_y - (hit_y < 0);

    const auto hit_off_x = hit_x - hit_tile_x;
    const auto hit_off_y = hit_y - hit_tile_y;

    if (hit_tile_x >= 0 && hit_tile_y >= 0 && hit_tile_x < Config::Level::WIDTH && hit_tile_y < Config::Level::HEIGHT) {
      if (GameState::level[hit_tile_x + hit_tile_y * Config::Level::WIDTH]) {
        paint_texture(x + floor_offset, TEXTURE_LAVA, hit_off_x, hit_off_y);
      } else {
        paint_texture(x + floor_offset, TEXTURE_FLOOR, hit_off_x, hit_off_y);
      }
    } else {
      buffer[x + floor_offset] = sky_shaded;
    }

    buffer[x + sky_offset] = sky_shaded;
    if constexpr (Y < floor_height - 1) {
      FloorCaster<Y + 1>::CastFloor(x, sin_x, cos_x, cos_fix);
    }
  }
};

void Raycaster::Render() {
  for (auto x = 0; x < Config::Display::WIDTH; x++) {
    const auto cast_dir =
      ((float)x / Config::Display::WIDTH * Config::Display::FOV - half_fov) *
      Math::deg2rad;
    const auto angle_x = GameState::player_rot + cast_dir;
    const auto sin_x = Math::sin(angle_x);
    const auto cos_x = Math::cos(angle_x);
    const auto cos_fix = 1 / Math::cos(cast_dir);
    FloorCaster<0>::CastFloor(x, sin_x, cos_x, cos_fix);

    const auto ray_start_x = GameState::player_x;
    const auto ray_start_y = GameState::player_y;

    const auto tile_x = (int)ray_start_x - (ray_start_x < 0);
    const auto tile_y = (int)ray_start_y - (ray_start_y < 0);

    if (x != Config::Display::WIDTH / 2) continue;

    // {Sin(x), Cos(x)} is the ray normalized direction vector
    // Player position is the ray starting position
    // Redefine for better readability
    const auto ray_dir_x = sin_x;
    const auto ray_dir_y = cos_x;

    const auto scaling_x = sin_x / cos_x;
    const auto scaling_y = cos_x / sin_x;
    const auto step_size_x = Math::sqrt(1 + scaling_x * scaling_x);
    const auto step_size_y = Math::sqrt(1 + scaling_y * scaling_y);

    // Accumulated ray length for each axis
    auto ray_len_x = 0;
    auto ray_len_y = 0;

    const auto ray_step_x = ray_dir_x < 0 ? -1 : 1;
    const auto ray_step_y = ray_dir_y < 0 ? -1 : 1;

    if (ray_dir_x < 0) {
      ray_len_x = (ray_start_x - tile_x) * step_size_x;
    } else {
      ray_len_x = (tile_x + 1.0f - ray_start_x) * step_size_x;
    }

    if (ray_dir_y < 0) {
      ray_len_y = (ray_start_y - tile_y) * step_size_y;
    } else {
      ray_len_y = (tile_y + 1.0f - ray_start_y) * step_size_y;
    }

    const auto ray_end_x = ray_start_x + ray_len_x;
    const auto ray_end_y = ray_start_y + ray_len_y;
    INFO("Ray hit = [%f, %f]\n", ray_end_x, ray_end_y);
  }
}
