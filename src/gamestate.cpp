#include "gamestate.hpp"

#include "input.hpp"
#include "util.h"
#include "config.h"

float GameState::player_x = 0, GameState::player_y = 0, GameState::player_rot = 0;

void GameState::Update(float delta_time) {
  if (Input::mouse_move) {
    Input::mouse_move = false;
    player_rot += (float)Input::mouse_x / Config::WIDTH * 1000 * delta_time;
  }
  constexpr auto rad_90 = 90.0f * Config::DEG2RAD;
  player_x += 5 * Input::input_x * sin(player_rot + rad_90) * delta_time;
  player_y += 5 * Input::input_x * cos(player_rot + rad_90) * delta_time;
  player_x += 5 * Input::input_y * sin(player_rot) * delta_time;
  player_y += 5 * Input::input_y * cos(player_rot) * delta_time;
}
