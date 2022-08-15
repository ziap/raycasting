#pragma once

#include <SDL2/SDL_events.h>
namespace Input {

extern bool mouse_down, mouse_move;

extern int mouse_x, mouse_y;
extern int input_x, input_y;

extern bool HandleEvent(SDL_Event*);
}  // namespace Input
