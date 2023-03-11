#pragma once

namespace Color {
using color_t = unsigned;

constexpr color_t FromHex(unsigned hex) {
  return 0xff000000                   // alpha
         | ((hex & 0xff0000) >> 16)   // red
         | ((hex & 0x00ff00))         // green
         | ((hex & 0x0000ff) << 16);  // blue
};

extern constexpr color_t Blend(color_t c1, color_t c2, short f) {
  const auto r1 = ((c1 & 0x0000ff) >> 0);
  const auto r2 = ((c2 & 0x0000ff) >> 0);
  const auto g1 = ((c1 & 0x00ff00) >> 8);
  const auto g2 = ((c2 & 0x00ff00) >> 8);
  const auto b1 = ((c1 & 0xff0000) >> 16);
  const auto b2 = ((c2 & 0xff0000) >> 16);
  const int rf = r1 + (r2 - r1) * f / 256;
  const int gf = g1 + (g2 - g1) * f / 256;
  const int bf = b1 + (b2 - b1) * f / 256;
  return 0xff000000              // alpha
         | ((rf & 0xff) << 0)    // red
         | ((gf & 0xff) << 8)    // green
         | ((bf & 0xff) << 16);  // blue
};
}  // namespace Color
