#include "renderer.hpp"

int main() {
  Renderer::InitWindow();
  Renderer::GameLoop();
  Renderer::DestroyWindow();
  return 0;
}
