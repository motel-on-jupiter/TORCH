/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef GLGRAPHICSWRAP_H_
#define GLGRAPHICSWRAP_H_

#include <GL/freeglut.h>

/**
 * To vanish the output of method-unused warning
 */
namespace freeglut_avoid_warning {
class _dummy {
  _dummy() {
    glutInit(nullptr, nullptr);
    glutCreateWindow(nullptr);
    glutCreateMenu(nullptr);
  }
};
}

#endif /* GLGRAPHICSWRAP_H_ */
