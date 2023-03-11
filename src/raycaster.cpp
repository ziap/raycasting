#include "raycaster.hpp"

#include "color.h"
#include "config.h"
#include "gamestate.hpp"
#include "renderer.hpp"
#include "texture_ceiling.h"
#include "texture_floor.h"
#include "texture_wall.h"
#include "util.hpp"

#define buffer Renderer::screen_buffer

#define paint_texture(texture_type, off_x, off_y, tile_factor)                 \
  (texture_type::data                                                          \
     [int(off_x * texture_type::width * tile_factor) % texture_type::width +   \
      int(off_y * texture_type::height * tile_factor) % texture_type::height * \
        texture_type::width])

#define paint_texture_inv(texture_type, off_x, off_y, tile_factor)             \
  (texture_type::data                                                          \
     [texture_type::width -                                                    \
      int(off_x * texture_type::width * tile_factor) % texture_type::width -   \
      1 +                                                                      \
      int(off_y * texture_type::height * tile_factor) % texture_type::height * \
        texture_type::width])

#define DIFFUSE_STRENGTH 0.5
#define diffuse_shading(pixel, diffuse)                                       \
  (Color::Blend(                                                              \
    black, pixel, 256 * ((1 - DIFFUSE_STRENGTH) + diffuse * DIFFUSE_STRENGTH) \
  ))

// Use bitshift to avoid clang int division warning
constexpr float half_height = Config::Display::HEIGHT >> 1;

constexpr auto half_fov = Config::Display::FOV / 2;
constexpr auto half_vfov = Config::Display::VFOV / 2;

// The distance to the projection plane, or a wall that is as high as the screen
// Use trigonometry we get 0.5 / tan(fov / 2) or cot(fov / 2) * 0.5
constexpr auto proj_dist = Math::cot(half_vfov * Math::deg2rad) * 0.5f;

constexpr auto black = Color::FromHex(0x000000);

static void CastFloor(
  int x, int cam_off, float sin_x, float cos_x, float cos_fix, int wall_off
) {
  for (int y = -Math::abs(cam_off); y < half_height - wall_off; y++) {
    const auto ray_len = proj_dist * half_height / (half_height - y);

    const auto hit_x = GameState::player_x + sin_x * ray_len * cos_fix;
    const auto hit_y = GameState::player_y + cos_x * ray_len * cos_fix;
    const auto hit_tile_x = (int)hit_x - (hit_x < 0);
    const auto hit_tile_y = (int)hit_y - (hit_y < 0);

    const auto hit_off_x = hit_x - hit_tile_x;
    const auto hit_off_y = hit_y - hit_tile_y;

    const auto floor_y = Config::Display::HEIGHT - 1 - cam_off - y;
    const auto ceiling_y = y - cam_off;

    if (floor_y < Config::Display::HEIGHT) {
      const auto light_len =
        proj_dist * half_height / (half_height - y + cam_off);
      const auto diffuse = 0.5 / Math::sqrt(light_len * light_len + 0.25);

      const auto floor_offset = floor_y * Config::Display::WIDTH;
      buffer[x + floor_offset] = diffuse_shading(
        paint_texture(TEXTURE_FLOOR, hit_off_x, hit_off_y, 1), diffuse
      );
    }

    if (ceiling_y >= 0) {
      const auto light_len =
        proj_dist * half_height / (half_height - y - cam_off);
      const auto diffuse = 0.5 / Math::sqrt(light_len * light_len + 0.25);

      const auto ceiling_offset = ceiling_y * Config::Display::WIDTH;
      buffer[x + ceiling_offset] = diffuse_shading(
        paint_texture(TEXTURE_CEILING, hit_off_x, hit_off_y, 1), diffuse
      );
    }
  }
}

void Raycaster::Render() {
  for (auto x = 0; x < Config::Display::WIDTH; x++) {
    constexpr auto tan_hfov_x2 = Math::tan(half_fov * Math::deg2rad) * 2;

    const auto tan_cast_dir =
      ((float)x / Config::Display::WIDTH - 0.5) * tan_hfov_x2;

    // calculate sin(x) and cos(x) indirectly so we don't have to do
    // sin/cos of atan(x)

    // cos(x)^-2 = (tan(x)^2 + 1)
    const auto inv_sqr_cos_dir = (tan_cast_dir * tan_cast_dir + 1);

    // TODO: remove the tan and sqrt by precalculate the sine and cosine of the
    // angle between 2 screen pixels and incrementally increase the sine and
    // cosine of the cast angle with that using sin(x + y) and cos(x + y)
    const auto inv_cos = Math::sqrt(inv_sqr_cos_dir);

    const auto cos_dir = 1.0 / inv_cos;
    const auto sin_dir = tan_cast_dir * cos_dir;

    const auto sin_rot = GameState::player_rot_sin;
    const auto cos_rot = GameState::player_rot_cos;

    const auto sin_x = sin_dir * cos_rot + cos_dir * sin_rot;
    const auto cos_x = cos_dir * cos_rot - sin_dir * sin_rot;

    const auto cam_off = GameState::player_pitch / half_fov * half_height;

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
        if (dist > Config::Display::RENDER_DISTANCE) break;
      }
    }

    const auto ray_end_x = ray_start_x + ray_dir_x * dist;
    const auto ray_end_y = ray_start_y + ray_dir_y * dist;

    if (found) {
      const auto wall_height = half_height * proj_dist * 2 / dist * inv_cos;
      const auto offset =
        (Config::Display::HEIGHT - wall_height) * 0.5f - cam_off;

      const auto wall_start = (int)Math::max(offset, 0);
      const auto wall_end =
        (int)Math::min(offset + wall_height, Config::Display::HEIGHT);

      const auto wall_off_x = ray_end_x - (int)ray_end_x + (ray_end_x < 0);
      const auto wall_off_y = ray_end_y - (int)ray_end_y + (ray_end_y < 0);

      for (auto i = wall_start; i < wall_end; i++) {
        const auto pixel = x + i * Config::Display::WIDTH;
        const auto wall_off_h = (i - offset) / wall_height;

        constexpr auto wall_tile = 1;

        // Check which side of the wall are we on to flip the textures
        if (horizontal) {
          Color::color_t wall_color;
          if (step_x < 0) {
            wall_color =
              paint_texture(TEXTURE_WALL, wall_off_y, wall_off_h, wall_tile);
          } else {
            wall_color = paint_texture_inv(
              TEXTURE_WALL, wall_off_y, wall_off_h, wall_tile
            );
          }
          buffer[pixel] = diffuse_shading(wall_color, Math::abs(sin_x));
        } else {
          Color::color_t wall_color;
          if (step_y < 0) {
            wall_color = paint_texture_inv(
              TEXTURE_WALL, wall_off_x, wall_off_h, wall_tile
            );
          } else {
            wall_color =
              paint_texture(TEXTURE_WALL, wall_off_x, wall_off_h, wall_tile);
          }
          buffer[pixel] = diffuse_shading(wall_color, Math::abs(cos_x));
        }
      }

      CastFloor(x, cam_off, sin_x, cos_x, inv_cos, wall_height / 2);
    } else {
      CastFloor(x, cam_off, sin_x, cos_x, inv_cos, 0);
    }
  }
}
