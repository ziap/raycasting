#include "renderer.hpp"

#include "config.h"
#include "floorcaster.hpp"
#include "input.hpp"
#include "util.h"

SDL_Renderer *renderer = nullptr;
SDL_Window *window = nullptr;

SDL_Texture *screen_texture = nullptr;
SDL_Surface *screen_surface = nullptr;
Uint32 *Renderer::screen_buffer = nullptr;

void Renderer::InitWindow() {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_version version;
  SDL_VERSION(&version);
  INFO("SDL: %d.%d.%d\n", version.major, version.minor, version.patch);

  window = SDL_CreateWindow(
    "Raycast engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    Config::WIDTH, Config::HEIGHT, SDL_WINDOW_SHOWN
  );
  auto pf = SDL_GetWindowPixelFormat(window);
  auto pf_name = SDL_GetPixelFormatName(pf);
  INFO("Pixel format: %s\n", pf_name);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RendererInfo renderer_info;
  SDL_GetRendererInfo(renderer, &renderer_info);
  INFO("Renderer: %s\n", renderer_info.name);

  screen_texture = SDL_CreateTexture(
    renderer, pf, SDL_TEXTUREACCESS_TARGET, Config::WIDTH, Config::HEIGHT
  );
  screen_surface = SDL_CreateRGBSurface(
    0, Config::WIDTH, Config::HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,
    0xff000000
  );
 
  screen_buffer = (Uint32 *)screen_surface->pixels;

  SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Renderer::DestroyWindow() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::Render() {
  FloorCaster::CastFloor(screen_buffer);

  SDL_UpdateTexture(
    screen_texture, nullptr, screen_surface->pixels, screen_surface->pitch
  );

  SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}
