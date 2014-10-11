/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef LOGGING_EMITTER_DEBUGGERCONSOLELOGEMITTER_H_
#define LOGGING_EMITTER_DEBUGGERCONSOLELOGEMITTER_H_

#ifdef _MSC_VER

#include "auxiliary/csyntax_aux.h"
#include "logging/emitter/LogEmitter.h"

#define DEBUGGER_CONSOLE_LOG_EMITTER (DebuggerConsoleLogEmitter::GetMutableInstance())

class DebuggerConsoleLogEmitter : public LogEmitter,
    public Singleton<DebuggerConsoleLogEmitter> {
 public:
  enum {
    kMaxEmittableSize = 1024
  };

  DebuggerConsoleLogEmitter();
  virtual ~DebuggerConsoleLogEmitter();

  virtual int Initialize();
  virtual int Finalize();

  virtual void Emitv(const char *prefix, const char *format, va_list args);

 private:
  virtual int EmitMultibyteChars(const char *mbchars);
  virtual int EmitPrefix(const char *prefix);
  virtual int EmitBody(const char *format, va_list args);
};

#endif /* _MSC_VER */

#endif /* LOGGING_EMITTER_DEBUGGERCONSOLELOGEMITTER_H_ */
