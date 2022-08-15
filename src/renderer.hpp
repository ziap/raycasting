#pragma once

#include <SDL2/SDL.h>

namespace Renderer {
extern Uint32 *screen_buffer;

void InitWindow(void);
void DestroyWindow(void);

void Render(void);

void ResetMouse(void);
}  // namespace Renderer
