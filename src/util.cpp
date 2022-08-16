#include "util.hpp"

int Math::clamp(int x, int l, int h) { return x < l ? l : (x > h ? h : x); }
int Math::round(float x) { return (int)(x + 0.5f - (x < 0)); }

#if USE_WASM
#include "wasm_import.hpp"

float Math::cos(float x) { return Math_cos(x); }
float Math::sin(float x) { return Math_sin(x); }

#else
#include <algorithm>
#include <cmath>

float Math::cos(float x) { return std::cos(x); }
float Math::sin(float x) { return std::sin(x); }

#endif
