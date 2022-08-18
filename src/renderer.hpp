#pragma once

namespace Renderer {

#if USE_WASM
extern unsigned screen_buffer[];
#else
extern unsigned *screen_buffer;
void InitWindow(void);
void DestroyWindow(void);
void GameLoop(void);
#endif
}  // namespace Renderer
