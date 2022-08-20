#include "util.hpp"

#if USE_WASM

float Math::sqrt(float x) { return __builtin_sqrtf(x); }

#else
#include <cmath>

float Math::cos(float x) { return std::cos(x); }
float Math::sin(float x) { return std::sin(x); }
float Math::sqrt(float x) { return std::sqrt(x); }

#endif
