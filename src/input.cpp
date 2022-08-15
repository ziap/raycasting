#include "input.hpp"

#include "config.h"
#include "renderer.hpp"

bool Input::mouse_down = false, Input::mouse_move = false;

int Input::mouse_x = 0, Input::mouse_y = 0;

int Input::input_x = 0, Input::input_y = 0;

enum KeyType { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_OTHER };

static KeyType GetKeyType(SDL_Keycode key) {
  switch (key) {
    case SDLK_UP: return KEY_UP;
    case SDLK_DOWN: return KEY_DOWN;
    case SDLK_LEFT: return KEY_LEFT;
    case SDLK_RIGHT: return KEY_RIGHT;

    case SDLK_w: return KEY_UP;
    case SDLK_s: return KEY_DOWN;
    case SDLK_a: return KEY_LEFT;
    case SDLK_d: return KEY_RIGHT;
    default: return KEY_OTHER;
  }
}

bool Input::HandleEvent(SDL_Event *e) {
  SDL_PollEvent(e);
  switch (e->type) {
    case SDL_QUIT: return false;
    case SDL_MOUSEBUTTONDOWN: mouse_down = true; break;
    case SDL_MOUSEBUTTONUP: mouse_down = false; break;
    case SDL_KEYDOWN: {
      auto key_type = GetKeyType(e->key.keysym.sym);
      switch (key_type) {
        case KEY_UP: input_y++; break;
        case KEY_DOWN: input_y--; break;
        case KEY_LEFT: input_x--; break;
        case KEY_RIGHT: input_x++; break;
        case KEY_OTHER: break;
      }
      break;
    }
    case SDL_KEYUP: {
      auto key_type = GetKeyType(e->key.keysym.sym);
      switch (key_type) {
        case KEY_UP: input_y--; break;
        case KEY_DOWN: input_y++; break;
        case KEY_LEFT: input_x++; break;
        case KEY_RIGHT: input_x--; break;
        case KEY_OTHER: break;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      mouse_move = true;
      mouse_x = e->motion.xrel;
      mouse_y = e->motion.yrel;
      break;
    }
  }

  input_x = std::max(input_x, -1);
  input_x = std::min(input_x, 1);
  input_y = std::max(input_y, -1);
  input_y = std::min(input_y, 1);
  return true;
}
