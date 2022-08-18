#include "gamestate.hpp"

#include "config.h"
#include "input.hpp"
#include "util.hpp"

#define LEVEL_SIZE (Config::Level::WIDTH * Config::Level::HEIGHT)
bool GameState::level[LEVEL_SIZE] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,  //
  1, 0, 0, 0, 1, 0, 0, 0, 0, 1,  //
  1, 1, 1, 0, 0, 0, 1, 0, 0, 1,  //
  1, 0, 1, 0, 0, 0, 1, 0, 0, 1,  //
  1, 0, 1, 0, 0, 0, 1, 1, 1, 1,  //
  1, 0, 0, 0, 0, 0, 0, 0, 0, 1,  //
  1, 0, 0, 1, 1, 1, 0, 0, 0, 1,  //
  1, 0, 0, 0, 0, 1, 0, 0, 0, 1,  //
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  //
};

float GameState::player_x = 1.5, GameState::player_y = 1.5,
      GameState::player_rot = 0;

void GameState::Update(float delta_time) {
  if (Input::mouse_move) {
    Input::mouse_move = false;
    player_rot +=
      (float)Input::mouse_x / Config::Display::WIDTH * 1000 * delta_time;
  }
  const auto speed =
    Config::Player::SPEED * delta_time *
    ((Input::input_x != 0 && Input::input_y != 0) ? Math::insqrt2 : 1.0f);
  const auto speed_x = speed * Input::input_x;
  const auto speed_y = speed * Input::input_y;

  constexpr auto rad_90 = 90.0f * Math::deg2rad;
  const auto sin_x = Math::sin(player_rot + rad_90);
  const auto cos_x = Math::cos(player_rot + rad_90);
  const auto sin_y = Math::sin(player_rot);
  const auto cos_y = Math::cos(player_rot);

  // The difference between the currently position and the next position
  const auto next_diff_x = speed_x * sin_x + speed_y * sin_y;
  const auto next_diff_y = speed_x * cos_x + speed_y * cos_y;

  const auto radius_shift_x =
    ((next_diff_x < 0) ? -Config::Player::RADIUS : Config::Player::RADIUS);
  const auto radius_shift_y =
    ((next_diff_y < 0) ? -Config::Player::RADIUS : Config::Player::RADIUS);

  const auto next_pos_x = player_x + next_diff_x + radius_shift_x;
  const auto next_pos_y = player_y + next_diff_y + radius_shift_y;

  const auto next_tile_x = (int)next_pos_x - (next_pos_x < 0);
  const auto next_tile_y = (int)next_pos_y - (next_pos_y < 0);

  if (!level[next_tile_x + next_tile_y * Config::Level::WIDTH]) {
    player_x += next_diff_x;
    player_y += next_diff_y;
  }
}
