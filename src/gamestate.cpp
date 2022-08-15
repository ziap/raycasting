#include "gamestate.hpp"

#include "input.hpp"
#include "util.h"

void GameState::Update(Uint64 delta_time) {
  constexpr auto min_fps = 60;
  constexpr auto max_delta_time = 1000 / min_fps;

  if (delta_time > max_delta_time) WARN("High delta time: %lu\n", delta_time);

  if (Input::mouse_down)
    INFO("Dragging: (%d, %d)\n", Input::mouse_x, Input::mouse_y);
}
