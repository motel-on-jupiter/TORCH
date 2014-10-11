/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef INCLUDER_CRTDBGINCLUDE_H_
#define INCLUDER_CRTDBGINCLUDE_H_

#if defined(_MSC_VER) && defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#endif /* defined(_MSC_VER) && defined(_DEBUG) */

#endif /* INCLUDER_CRTDBGINCLUDE_H_ */
