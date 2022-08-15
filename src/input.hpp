#pragma once

namespace Input {

enum KeyType { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_OTHER };

extern bool mouse_down, mouse_move;

extern int mouse_x, mouse_y;
extern int input_x, input_y;

void MouseDown(void);
void MouseUp(void);

void KeyDown(KeyType);
void KeyUp(KeyType);

void MouseMove(int, int);
}  // namespace Input
