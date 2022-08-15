#include "game.hpp"

#include "gamestate.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "util.h"

void Game::Start() {
  Renderer::InitWindow();
  INFO("Sucessfully launched game\n");
}

void Game::Stop() {
  Renderer::DestroyWindow();
  INFO("Sucessfully exited game\n");
}

void Game::Run() {
  auto last_frame = SDL_GetTicks64();
  auto last_fps_update = last_frame;
  auto frames = 0.0;
  SDL_Event event;
  while (Input::HandleEvent(&event)) {
    frames++;
    auto now = SDL_GetTicks64();
    GameState::Update((float)(now - last_frame) / 1000.0f);
    Renderer::Render();
    last_frame = now;
    auto delta_time = now - last_fps_update;
    if (delta_time >= 1000) {
      INFO("FPS: %f\n", frames * 1000 / delta_time);
      last_fps_update = now;
      frames = 1;
    }
  }
}
