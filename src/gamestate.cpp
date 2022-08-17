#include "gamestate.hpp"

#include "config.h"
#include "input.hpp"
#include "util.hpp"

float GameState::player_x = 0, GameState::player_y = 0,
      GameState::player_rot = 0;

void GameState::Update(float delta_time) {
  if (Input::mouse_move) {
    Input::mouse_move = false;
    player_rot += (float)Input::mouse_x / Config::WIDTH * 1000 * delta_time;
  }
  constexpr auto rad_90 = 90.0f * Math::deg2rad;
  const auto speed =
    5.0f *
    ((Input::input_x != 0 && Input::input_y != 0) ? Math::insqrt2 : 1.0f);
  const auto speed_x = speed * Input::input_x * delta_time;
  const auto speed_y = speed * Input::input_y * delta_time;

  player_x += speed_x * Math::sin(player_rot + rad_90);
  player_y += speed_x * Math::cos(player_rot + rad_90);
  player_x += speed_y * Math::sin(player_rot);
  player_y += speed_y * Math::cos(player_rot);
}
