#if USE_WASM
#include "config.h"
#include "input.hpp"
#include "raycaster.hpp"
#include "util.hpp"
#include "wasm_import.hpp"

unsigned image_buffer[Config::WIDTH * Config::HEIGHT] = {0};

extern "C" {
void GameState_Update(float time) { GameState::Update(time); }
void Raycaster_Render() { return Raycaster::Render(image_buffer); };
void Input_MouseDown() { return Input::MouseDown(); }
void Input_MouseUp() { return Input::MouseUp(); }
void Input_MouseMove(int x, int y) { return Input::MouseMove(x, y); }
void Input_KeyDown(int type) {
  Input::KeyDown(static_cast<Input::KeyType>(type));
}
void Input_KeyUp(int type) { Input::KeyUp(static_cast<Input::KeyType>(type)); }
void* BufferPointer() { return (void*)&image_buffer; }
}
#endif
