#pragma once

namespace Color {
using color_t = unsigned;

constexpr color_t FromHex(unsigned hex) {
  return 0xff000000                   // alpha
         | ((hex & 0xff0000) >> 16)   // red
         | ((hex & 0x00ff00))         // green
         | ((hex & 0x0000ff) << 16);  // blue
};

extern constexpr color_t Blend(color_t c1, color_t c2, float f) {
  auto r1 = (int)((c1 & 0x0000ff) >> 0);
  auto r2 = (int)((c2 & 0x0000ff) >> 0);
  auto g1 = (int)((c1 & 0x00ff00) >> 8);
  auto g2 = (int)((c2 & 0x00ff00) >> 8);
  auto b1 = (int)((c1 & 0xff0000) >> 16);
  auto b2 = (int)((c2 & 0xff0000) >> 16);
  r1 += (float)(r2 - r1) * f;
  g1 += (float)(g2 - g1) * f;
  b1 += (float)(b2 - b1) * f;
  return 0xff000000              // alpha
         | ((r1 & 0xff) << 0)    // red
         | ((g1 & 0xff) << 8)    // green
         | ((b1 & 0xff) << 16);  // blue
};
}  // namespace Color
