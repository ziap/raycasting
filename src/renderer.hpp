#pragma once

#include <SDL2/SDL.h>

namespace Renderer {
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern SDL_Texture *screen_texture;
extern SDL_Surface *screen_surface;

extern Uint32 *screen_buffer;

void InitWindow(void);
void DestroyWindow(void);

void SetColor(Uint8, Uint8, Uint8);

void Render(void);
}  // namespace Renderer
