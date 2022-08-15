#include "input.hpp"

bool Input::mouse_down = false, Input::mouse_move = false;
int Input::mouse_x = 0, Input::mouse_y = 0;
int Input::input_x = 0, Input::input_y = 0;

void Input::MouseDown() { mouse_down = true; }
void Input::MouseUp() { mouse_down = false; }

void Input::KeyDown(Input::KeyType key_type) {
  switch (key_type) {
    case KEY_UP:
      if (Input::input_y < 1) Input::input_y++;
      break;
    case KEY_DOWN:
      if (Input::input_y > -1) Input::input_y--;
      break;
    case KEY_LEFT:
      if (Input::input_x > -1) Input::input_x--;
      break;
    case KEY_RIGHT:
      if (Input::input_x < 1) Input::input_x++;
      break;
    case KEY_OTHER: break;
  }
}

void Input::KeyUp(Input::KeyType key_type) {
  switch (key_type) {
    case KEY_UP:
      if (Input::input_y > 0) Input::input_y--;
      break;
    case KEY_DOWN:
      if (Input::input_y < 0) Input::input_y++;
      break;
    case KEY_LEFT:
      if (Input::input_x < 0) Input::input_x++;
      break;
    case KEY_RIGHT:
      if (Input::input_x > 0) Input::input_x--;
      break;
    case KEY_OTHER: break;
  }
}

void Input::MouseMove(int xrel, int yrel) {
  mouse_move = true;
  mouse_x = xrel;
  mouse_y = yrel;
}
