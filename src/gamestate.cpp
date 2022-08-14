#include "gamestate.hpp"

#include "input.hpp"
#include "util.h"

void GameState::Update(Uint64 delta_time) {
  if (Input::mouse_down)
    INFO("Dragging: (%d, %d)\n", Input::mouse_x, Input::mouse_y);
}
