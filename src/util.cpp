#include "util.hpp"

float Math::sqrt(float x) { return __builtin_sqrtf(x); }

float Math::atan(float x) { return __builtin_atanf(x); }

float Math::cpsign(float x, float y) { return __builtin_copysignf(x, y); }
