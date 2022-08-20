#if USE_WASM
#include "config.h"
#include "gamestate.hpp"
#include "input.hpp"
#include "raycaster.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "wasm_import.hpp"

#define export extern "C" __attribute__((visibility("default")))

export void GameLoop(float time) {
  GameState::Update(time / 1000);
  Raycaster::Render();
}

export void MouseDown() { return Input::MouseDown(); }
export void MouseUp() { return Input::MouseUp(); }
export void MouseMove(int x, int y) { return Input::MouseMove(x, y); }

export void KeyUp(const int type_id) {
  const auto key_type = static_cast<Input::KeyType>(type_id);
  Input::KeyUp(key_type);
}

export void KeyDown(const int type_id) {
  const auto key_type = static_cast<Input::KeyType>(type_id);
  Input::KeyDown(key_type);
}

export void* InitAndGetPointer() {
  Resize(Config::Display::WIDTH, Config::Display::HEIGHT);
  GameState::Init();
  return (void*)&Renderer::screen_buffer;
}
#endif
