#pragma once
#include <SDL2/SDL.h>

class Game {
 public:
  Game();
  ~Game();

  void run();

  Uint64 delta_time;

 private:
  const static int WINDOW_WIDTH;
  const static int WINDOW_HEIGHT;

  SDL_Window *window;
  SDL_Renderer *renderer;
};
