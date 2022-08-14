#include "input.hpp"

std::unordered_set<SDL_Keycode> Input::keys;

bool Input::mouse_down = false;

int Input::mouse_x, Input::mouse_y;

bool Input::HandleEvent(SDL_Event *e) {
  SDL_PollEvent(e);
  SDL_GetMouseState(&mouse_x, &mouse_y);
  switch (e->type) {
    case SDL_QUIT: return false;
    case SDL_MOUSEBUTTONDOWN: mouse_down = true; break;
    case SDL_MOUSEBUTTONUP: mouse_down = false; break;
    case SDL_KEYDOWN: keys.insert(e->key.keysym.sym); break;
    case SDL_KEYUP: keys.erase(e->key.keysym.sym); break;
  }
  return true;
}
