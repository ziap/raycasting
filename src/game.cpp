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
  auto frames = 0;
  SDL_Event event;
  while (Input::HandleEvent(&event)) {
    frames++;
    auto now = SDL_GetTicks64();
    GameState::Update(now - last_frame);
    Renderer::Render();
    last_frame = now;
    if (now - last_fps_update > 1000) {
      INFO("FPS: %d\n", frames);
      last_fps_update = now;
      frames = 0;
    }
  }
}
