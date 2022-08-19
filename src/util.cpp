#include "util.hpp"

int Math::clamp(int x, int l, int h) { return x < l ? l : (x > h ? h : x); }

#if USE_WASM
#include "wasm_import.hpp"

float Math::abs(float x) { return x < 0 ? -x : x; }
float Math::min(float x, float y) { return x < y ? x : y; }
float Math::max(float x, float y) { return x > y ? x : y; }
float Math::cos(float x) { return Math_cos(x); }
float Math::sin(float x) { return Math_sin(x); }
float Math::tan(float x) { return Math_tan(x); }
float Math::atan(float x) { return Math_atan(x); }
float Math::sqrt(float x) { return Math_sqrt(x); }

#else
#include <cmath>

float Math::abs(float x) { return std::abs(x); }
float Math::min(float x, float y) { return std::min(x, y); }
float Math::max(float x, float y) { return std::max(x, y); }
float Math::cos(float x) { return std::cos(x); }
float Math::sin(float x) { return std::sin(x); }
float Math::tan(float x) { return std::tan(x); }
float Math::atan(float x) { return std::tan(x); }
float Math::sqrt(float x) { return std::sqrt(x); }

#endif
