#include "renderer.hpp"

#include "config.h"
#include "input.hpp"
#include "raycaster.hpp"
#include "util.hpp"

#if !USE_WASM

#include <SDL2/SDL.h>

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
    renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, Config::WIDTH,
    Config::HEIGHT
  );
  screen_surface = SDL_CreateRGBSurface(
    0, Config::WIDTH, Config::HEIGHT, 32, 0x000000ff, 0x0000ff00, 0x00ff0000,
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

static Input::KeyType GetKeyType(SDL_Keycode key) {
  switch (key) {
      // case SDLK_UP: return Input::KEY_UP;
      // case SDLK_DOWN: return Input::KEY_DOWN;
      // case SDLK_LEFT: return Input::KEY_LEFT;
      // case SDLK_RIGHT: return Input::KEY_RIGHT;

    case SDLK_w: return Input::KEY_UP;
    case SDLK_s: return Input::KEY_DOWN;
    case SDLK_a: return Input::KEY_LEFT;
    case SDLK_d: return Input::KEY_RIGHT;
    default: return Input::KEY_OTHER;
  }
}

static bool HandleEvent() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) switch (e.type) {
      case SDL_QUIT: return false;
      case SDL_MOUSEBUTTONDOWN: Input::MouseDown(); break;
      case SDL_MOUSEBUTTONUP: Input::MouseUp(); break;
      case SDL_KEYDOWN: {
        auto key_type = GetKeyType(e.key.keysym.sym);
        Input::KeyDown(key_type);
        break;
      }
      case SDL_KEYUP: {
        auto key_type = GetKeyType(e.key.keysym.sym);
        Input::KeyUp(key_type);
        break;
      }
      case SDL_MOUSEMOTION: {
        Input::MouseMove(e.motion.xrel, e.motion.yrel);
        break;
      }
    }
  return true;
}

static void Render() {
  Raycaster::Render(Renderer::screen_buffer);

  SDL_UpdateTexture(
    screen_texture, nullptr, screen_surface->pixels, screen_surface->pitch
  );

  SDL_RenderCopy(renderer, screen_texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void Renderer::GameLoop() {
  auto last_frame = SDL_GetTicks64();
  auto last_fps_update = last_frame;
  auto frames = 0.0;
  while (HandleEvent()) {
    frames++;
    auto now = SDL_GetTicks64();
    GameState::Update((float)(now - last_frame) / 1000.0f);
    Render();
    last_frame = now;
    auto delta_time = now - last_fps_update;
    if (delta_time >= 1000) {
      INFO("FPS: %f\n", frames * 1000 / delta_time);
      last_fps_update = now;
      frames = 1;
    }
  }
}

#endif
