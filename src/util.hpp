#pragma once

namespace Math {
constexpr float pi = 3.141592653589793;
constexpr float deg2rad = pi / 180.0;
constexpr float insqrt2 = 1.0 / 1.4142135623730951;

int clamp(int, int, int);

float abs(float);
float max(float, float);
float min(float, float);

float sin(float);
float cos(float);
float tan(float);
float atan(float);

float sqrt(float);
}  // namespace Math

#if USE_WASM

#define INFO(...)
#define WARN(...)

#else
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
