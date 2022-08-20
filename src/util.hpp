#pragma once

namespace Math {
constexpr float pi = 3.141592653589793;
constexpr float pix2 = pi * 2.0;
constexpr float pidiv2 = pi / 2.0;
constexpr float sqrpi = pi * pi;
constexpr float deg2rad = pi / 180.0;
constexpr float insqrt2 = 1.0 / 1.4142135623730951;

constexpr int clamp(int x, int l, int h) { return x < l ? l : (x > h ? h : x); }
constexpr float abs(float x) { return x < 0 ? -x : x; }
constexpr float min(float x, float y) { return x < y ? x : y; }
constexpr float max(float x, float y) { return x > y ? x : y; }

float sqrt(float x);

#if USE_WASM

constexpr float cos(float x) {
  // cos(x) = sin((x + pi/2) % 2pi)
  x += pi + pidiv2;
  x = x - ((int)(x / pix2) + (x < 0)) * pix2 - pi;

  // fast sine approximation
  const auto cv = 1.27323954 * x + 0.405284735 * x * -Math::abs(x);
  return .225 * (cv * Math::abs(cv) - cv) + cv;
}

constexpr float sin(float x) {
  // sin(x) = sin(x % 2pi)
  x += pi;
  x = x - ((int)(x / pix2) + (x < 0)) * pix2 - pi;

  // Same as above
  const auto cv = 1.27323954 * x + 0.405284735 * x * -Math::abs(x);
  return .225 * (cv * Math::abs(cv) - cv) + cv;
}

#else
// Use the standard library trig functions
float sin(float);
float cos(float);
#endif
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
