#include "util.hpp"

#if USE_WASM
#include "wasm_import.hpp"

float Math::sqrt(float x) { return __builtin_sqrtf(x); }

float Math::atan(float x) { return Math_atan(x); }

#else
#include <cmath>

float Math::atan(float x) { return std::atan(x); }

float Math::sqrt(float x) { return std::sqrt(x); }

#endif
