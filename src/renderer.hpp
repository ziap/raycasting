#pragma once

namespace Renderer {
extern unsigned *screen_buffer;

void InitWindow(void);
void DestroyWindow(void);
void GameLoop(void);
}  // namespace Renderer
