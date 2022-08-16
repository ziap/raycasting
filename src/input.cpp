#include "input.hpp"

#include "util.hpp"

bool Input::mouse_down = false, Input::mouse_move = false;
int Input::mouse_x = 0, Input::mouse_y = 0;
int Input::input_x = 0, Input::input_y = 0;

void Input::MouseDown() { mouse_down = true; }
void Input::MouseUp() { mouse_down = false; }

void Input::KeyDown(Input::KeyType key_type) {
  switch (key_type) {
    case KEY_UP: Input::input_y++; break;
    case KEY_DOWN: Input::input_y--; break;
    case KEY_LEFT: Input::input_x--; break;
    case KEY_RIGHT: Input::input_x++; break;
    case KEY_OTHER: break;
  }
  input_x = Math::clamp(input_x, -1, 1);
  input_y = Math::clamp(input_y, -1, 1);
}

void Input::KeyUp(Input::KeyType key_type) {
  switch (key_type) {
    case KEY_UP: Input::input_y--; break;
    case KEY_DOWN: Input::input_y++; break;
    case KEY_LEFT: Input::input_x++; break;
    case KEY_RIGHT: Input::input_x--; break;
    case KEY_OTHER: break;
  }
}

void Input::MouseMove(int xrel, int yrel) {
  mouse_move = true;
  mouse_x = xrel;
  mouse_y = yrel;
}
