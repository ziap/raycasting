#include "raycaster.hpp"

#include "color.h"
#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "texture_floor.h"
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

template <int Y = 0>
struct FloorCaster {
  static constexpr float Shade(float intensity) {
    return (1 - intensity) + intensity * (1.0f - (float)(Y / floor_height));
  }
  static constexpr auto ray_len = floor_height / (floor_height - Y);
  static constexpr auto floor_offset =
    (Config::Display::HEIGHT - Y - 1) * Config::Display::WIDTH;
  static constexpr auto sky_offset = Y * Config::Display::WIDTH;
  static constexpr auto sky_shade = Shade(0.5);
  static constexpr auto sky_shaded = Color::Blend(sky1, sky2, sky_shade);

  inline static void CastFloor(
    int x, float sin_x, float cos_x, float cos_fix, int wall_offset
  ) {
    if (floor_height - Y > wall_offset) {
      const auto hit_x = GameState::player_x + sin_x * ray_len * cos_fix;
      const auto hit_y = GameState::player_y + cos_x * ray_len * cos_fix;
      const auto hit_tile_x = (int)hit_x - (hit_x < 0);
      const auto hit_tile_y = (int)hit_y - (hit_y < 0);

      const auto hit_off_x = hit_x - hit_tile_x;
      const auto hit_off_y = hit_y - hit_tile_y;

      paint_texture(x + floor_offset, TEXTURE_FLOOR, hit_off_x, hit_off_y);

      buffer[x + sky_offset] = sky_shaded;
    }
    if constexpr (Y < floor_height - 1) {
      FloorCaster<Y + 1>::CastFloor(x, sin_x, cos_x, cos_fix, wall_offset);
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

    // Raycasting using the DDA line generation algorithm
    const auto ray_start_x = GameState::player_x;
    const auto ray_start_y = GameState::player_y;

    auto tile_x = (int)ray_start_x - (ray_start_x < 0);
    auto tile_y = (int)ray_start_y - (ray_start_y < 0);

    // {Sin(x), Cos(x)} is the ray normalized direction vector
    // Player position is the ray starting position
    // Redefine for better readability
    const auto ray_dir_x = sin_x;
    const auto ray_dir_y = cos_x;

    const auto scaling_x = ray_dir_y / ray_dir_x;
    const auto scaling_y = ray_dir_x / ray_dir_y;

    // Calculate how much the ray change in magnitude by moving in each axis
    const auto ray_step_x = Math::sqrt(1 + scaling_x * scaling_x);
    const auto ray_step_y = Math::sqrt(1 + scaling_y * scaling_y);

    // We create 2 ray for each axis, set the distance to the smaller one and
    // increment it until that ray is bigger. That way we can move in integer
    // space without skipping a tile
    auto ray_len_x = 0.0;
    auto ray_len_y = 0.0;

    const auto step_x = ray_dir_x < 0 ? -1 : 1;
    const auto step_y = ray_dir_y < 0 ? -1 : 1;

    // Align the ray with the next tile
    if (ray_dir_x < 0) {
      ray_len_x = (ray_start_x - tile_x) * ray_step_x;
    } else {
      ray_len_x = (tile_x + 1.0f - ray_start_x) * ray_step_x;
    }
    if (ray_dir_y < 0) {
      ray_len_y = (ray_start_y - tile_y) * ray_step_y;
    } else {
      ray_len_y = (tile_y + 1.0f - ray_start_y) * ray_step_y;
    }

    auto dist = 0.0f;
    auto found = false;
    auto horizontal = false;
    for (;;) {
      // Increment the smaller ray
      if (ray_len_x < ray_len_y) {
        tile_x += step_x;
        dist = ray_len_x;
        ray_len_x += ray_step_x;
        horizontal = true;
      } else {
        tile_y += step_y;
        dist = ray_len_y;
        ray_len_y += ray_step_y;
        horizontal = false;
      }
      if (
        // Stop casting if the ray move out of boundary
        tile_x >= 0 && tile_x < Config::Level::WIDTH &&  // Horizontal boundary
        tile_y >= 0 && tile_y < Config::Level::HEIGHT    // Vertical boundary
      ) {
        found = GameState::level[tile_x + tile_y * Config::Level::WIDTH];

        // Or when it hit a wall
        if (found) break;
      } else {
        break;
      }
    }

    const auto ray_end_x = ray_start_x + ray_dir_x * dist;
    const auto ray_end_y = ray_start_y + ray_dir_y * dist;

    if (found) {
      const auto wall_height = (float)Config::Display::HEIGHT / dist * cos_fix;
      const auto offset = (Config::Display::HEIGHT - wall_height) / 2;

      const auto wall_start = (int)Math::max(offset, 0);
      const auto wall_end =
        (int)Math::min(offset + wall_height, Config::Display::HEIGHT);

      for (auto i = wall_start; i < wall_end; i++) {
        const auto pixel = x + i * Config::Display::WIDTH;
        if (horizontal) buffer[pixel] = Color::FromHex(0x0a0a0a);
        else
          buffer[pixel] = Color::FromHex(0x0d0d0d);
      }

      FloorCaster<0>::CastFloor(x, sin_x, cos_x, cos_fix, (int)wall_height / 2);
    } else {
      FloorCaster<0>::CastFloor(x, sin_x, cos_x, cos_fix, 0);
    }
  }
}
