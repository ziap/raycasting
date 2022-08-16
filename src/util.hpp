#pragma once

namespace Math {
constexpr float deg2rad = 3.1415926535897 / 180.0;

float sin(float);
float cos(float);
float round(float);
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
