#if USE_WASM
#include "config.h"
#include "gamestate.hpp"
#include "input.hpp"
#include "raycaster.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "wasm_import.hpp"

extern "C" {
void GameLoop(float time) {
  GameState::Update(time / 1000);
  Raycaster::Render();
}

void Input_MouseDown() { return Input::MouseDown(); }
void Input_MouseUp() { return Input::MouseUp(); }
void Input_MouseMove(int x, int y) { return Input::MouseMove(x, y); }

void Input_KeyUp(const int type_id) {
  const auto key_type = static_cast<Input::KeyType>(type_id);
  Input::KeyUp(key_type);
}

void Input_KeyDown(const int type_id) {
  const auto key_type = static_cast<Input::KeyType>(type_id);
  Input::KeyDown(key_type);
}

void* BufferPointer() {
  Resize(Config::Display::WIDTH, Config::Display::HEIGHT);
  return (void*)&Renderer::screen_buffer;
}
}
#endif
