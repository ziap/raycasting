#include "renderer.hpp"

const int Renderer::WINDOW_WIDTH = 1020;
const int Renderer::WINDOW_HEIGHT = 720;

SDL_Renderer *Renderer::renderer = nullptr;
SDL_Window *Renderer::window = nullptr;

#define bg_color 255, 255, 255

void Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetRenderDrawColor(Renderer::renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Renderer::ClearScreen() {
  SetColor(bg_color);
  SDL_RenderClear(renderer);
}

void Renderer::InitWindow() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(
    WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer
  );
  ClearScreen();
  SDL_RenderPresent(renderer);
}

void Renderer::DestroyWindow() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::Render() {
  ClearScreen();
  SetColor(255, 0, 0);
  for (auto i = 0; i < WINDOW_WIDTH; i++) {
      SetColor(
        Uint8(255 - 255 * i / WINDOW_WIDTH), 0,
         Uint8(255 * i / WINDOW_WIDTH)
      );
      SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT - 1);
  }
  SDL_RenderPresent(renderer);
}
