/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "auxiliary/assert_aux.h"

inline void ASSERT(bool expr) {
  if (!expr) {
    __debugbreak();
  }
}

inline void assert_not_reached() {
  ASSERT(false);
}
