#include "game.hpp"

#include "cstdio"

const int Game::WINDOW_WIDTH = 1280;
const int Game::WINDOW_HEIGHT = 720;

Game::Game() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(
    Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0, &this->window, &this->renderer
  );
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);
  SDL_RenderClear(this->renderer);
  SDL_RenderPresent(this->renderer);
  printf("Sucessfully launched game\n");
}

Game::~Game() {
  printf("Exiting game...\n");
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Game::run() {
  SDL_Event event;
  auto last_frame = SDL_GetTicks64();
  auto last_fps_update = last_frame;
  auto frames = 0;
  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    frames++;
    auto now = SDL_GetTicks64();
    this->delta_time = now - last_frame;
    last_frame = now;
    if (now - last_fps_update > 1000) {
      printf("FPS: %d\n", frames);
      last_fps_update = now;
      frames = 0;
    }
  }
}
