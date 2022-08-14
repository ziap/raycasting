#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

#include <unordered_set>

namespace Input {
extern std::unordered_set<SDL_Keycode> keys;

extern bool mouse_down;

extern int mouse_x, mouse_y;

extern bool HandleEvent(SDL_Event*);
}  // namespace Input
