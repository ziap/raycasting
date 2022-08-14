#pragma once

#include <SDL2/SDL.h>

namespace Renderer {

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

void InitWindow(void);
void DestroyWindow(void);

void SetColor(Uint8, Uint8, Uint8);
void ClearScreen(void);
void Render(void);
}  // namespace Renderer
