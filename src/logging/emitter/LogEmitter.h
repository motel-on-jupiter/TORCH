/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef LOGGING_EMITTER_LOGEMITTER_H_
#define LOGGING_EMITTER_LOGEMITTER_H_

class LogEmitter {
 public:
  LogEmitter();
  virtual ~LogEmitter();

  virtual void Emit(const char *prefix, const char *format, ...);
  virtual void Emitv(const char *prefix, const char *format, va_list args) = 0;
};

#endif /* LOGGING_EMITTER_LOGEMITTER_H_ */
