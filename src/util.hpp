#pragma once

constexpr float DEG2RAD = 3.1415926535897 / 180.0;

#if RENDERER == RENDERER_WASM
extern float cos(float);
extern float sin(float);
extern float round(float);

#define INFO(...)
#define WARN(...)

#else
#include <cmath>
#include <cstdio>

#define INFO(...)                                                    \
  {                                                                  \
    printf("[INFO][%s:%d][%s]: ", __FILE__, __LINE__, __FUNCTION__); \
    printf(__VA_ARGS__);                                             \
  }

#define WARN(...)                                                    \
  {                                                                  \
    printf("[WARN][%s:%d][%s]: ", __FILE__, __LINE__, __FUNCTION__); \
    printf(__VA_ARGS__);                                             \
  }
#endif
