#pragma once

#include <cstdio>

#define INFO(...)                                                    \
  {                                                                  \
    printf("[INFO][%s:%d][%s]: ", __FILE__, __LINE__, __FUNCTION__); \
    printf(__VA_ARGS__);                                             \
  }
