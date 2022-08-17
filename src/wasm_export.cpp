#if USE_WASM
#include "config.h"
#include "input.hpp"
#include "raycaster.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "wasm_import.hpp"

extern "C" {
void Init() { Resize(Config::WIDTH, Config::HEIGHT); }
void GameState_Update(float time) { GameState::Update(time); }
void Raycaster_Render() { return Raycaster::Render(); };
void Input_MouseDown() { return Input::MouseDown(); }
void Input_MouseUp() { return Input::MouseUp(); }
void Input_MouseMove(int x, int y) { return Input::MouseMove(x, y); }
void Input_KeyDown(int type) {
  Input::KeyDown(static_cast<Input::KeyType>(type));
}
void Input_KeyUp(int type) { Input::KeyUp(static_cast<Input::KeyType>(type)); }
void* BufferPointer() { return (void*)&Renderer::screen_buffer; }
}
#endif
