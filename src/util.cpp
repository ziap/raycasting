#include "util.hpp"

#if !USE_WASM

#include <cmath>

float Math::cos(float x) { return std::cos(x); }

float Math::sin(float x) { return std::sin(x); }

float Math::round(float x) { return std::round(x); }

#endif
