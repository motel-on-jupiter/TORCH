/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "logging/emitter/LogEmitter.h"
#include <cstdarg>

LogEmitter::LogEmitter() {
}

LogEmitter::~LogEmitter() {
}

void LogEmitter::Emit(const char* prefix, const char* format, ...) {
  va_list args;
  va_start(format, args);
  Emitv(prefix, format, args);
  va_end(args);
}
