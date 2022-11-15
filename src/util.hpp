#pragma once

namespace Math {
constexpr float pi = 3.141592653589793;
constexpr float pi_x2 = pi * 2.0;
constexpr float deg2rad = pi / 180.0;
constexpr float insqrt2 = 1.0 / 1.4142135623730951;

constexpr int clamp(int x, int l, int h) { return x < l ? l : (x > h ? h : x); }
constexpr float abs(float x) { return x < 0 ? -x : x; }
constexpr float min(float x, float y) { return x < y ? x : y; }
constexpr float max(float x, float y) { return x > y ? x : y; }

float sqrt(float x);

float cpsign(float x, float y);

// TODO: Use python to generate lookup tables after using integer rotation
constexpr float sin(float x) {
  // sin(x) = sin(x % 2pi)
  x += pi;
  x = x - ((int)(x / pi_x2) + (x < 0)) * pi_x2 - pi;

  // fast sine approximation
  constexpr auto u = 4 / pi;
  constexpr auto v = 4 / pi / pi;
  const auto cv = u * x - v * x * abs(x);
  return 0.225 * (cv * abs(cv) - cv) + cv;
}

constexpr float cos(float x) {
  // cos(x) = sin((x + pi/2))
  constexpr auto half_pi = pi * 0.5;
  return sin(x + half_pi);
}

constexpr float tan(float x) { return sin(x) / cos(x); }
constexpr float cot(float x) { return cos(x) / sin(x); }

constexpr float atan(float x) {
  constexpr auto a = 0.0776509570923569;
  constexpr auto b = -0.287434475393028;
  constexpr auto c = pi * 0.25 - a - b;

  const auto xx = x * x;
  return ((a * xx + b) * xx + c) * x;
}
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
