#include "renderer.hpp"

#include "input.hpp"
#include "util.h"

const int Renderer::WINDOW_WIDTH = 1080;
const int Renderer::WINDOW_HEIGHT = 720;

SDL_Renderer *Renderer::renderer = nullptr;
SDL_Window *Renderer::window = nullptr;

SDL_Texture *Renderer::screen_texture = nullptr;
SDL_Surface *Renderer::screen_surface = nullptr;
Uint32 *Renderer::screen_buffer = nullptr;

void Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b) {
  SDL_SetRenderDrawColor(Renderer::renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

void Renderer::InitWindow() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_version version;
  SDL_VERSION(&version);
  INFO("SDL: %d.%d.%d\n", version.major, version.minor, version.patch);

  window = SDL_CreateWindow(
    "Raycast engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
  );
  auto pf = SDL_GetWindowPixelFormat(window);
  auto pf_name = SDL_GetPixelFormatName(pf);
  INFO("Pixel format: %s\n", pf_name);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RendererInfo renderer_info;
  SDL_GetRendererInfo(renderer, &renderer_info);
  INFO("Renderer: %s\n", renderer_info.name);

  screen_texture = SDL_CreateTexture(
    renderer, pf, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT
  );
  screen_surface = SDL_CreateRGBSurface(
    0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,
    0xff000000
  );

  screen_buffer = (Uint32 *)screen_surface->pixels;
}

void Renderer::DestroyWindow() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::Render() {
  SetColor(0, 0, 0);
  SDL_RenderClear(renderer);

  for (auto i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    auto x = i % WINDOW_WIDTH;
    auto y = i / WINDOW_WIDTH;

    auto diff_x = x - Input::mouse_x;
    auto diff_y = y - Input::mouse_y;

    auto diff = diff_x * diff_x + diff_y * diff_y;
    auto constexpr max_dist =
      WINDOW_WIDTH * WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT;

    auto inter = (Uint8)(255.0f * float(diff) / float(max_dist));

    screen_buffer[i] = 0x00ffff + (inter << 16) - inter;
  }

  SDL_UpdateTexture(
    screen_texture, nullptr, screen_surface->pixels, screen_surface->pitch
  );

  SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
