/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "logging/emitter/LogEmitter.h"

inline void Logger::Emit(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kNoneLevel, format, args);
  va_end(args);
}

inline void Logger::Emitv(const char *format, va_list args) {
  Emitv(kNoneLevel, format, args);
}

inline void Logger::Emit(Level level, const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(level, format, args);
  va_end(args);
}

inline void Logger::Emitv(Level level, const char *format, va_list args) {
  if (level >= kNumLogLevels) {
    fprintf(stdout,
            "Failed for logger to emit the message due to invalid level: %d",
            level);
    return;
  }

  auto it = emitters_.begin();
  for (; emitters_.end() != it; ++it) {
    (*it)->Emitv(kLogPrefixes[level], format, args);
  }
}

inline void Logger::Trace(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kTrace, format, args);
  va_end(args);
}

inline void Logger::Tracev(const char *format, va_list args) {
  Emitv(kTrace, format, args);
}

inline void Logger::Debug(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kDebug, format, args);
  va_end(args);
}

inline void Logger::Debugv(const char *format, va_list args) {
  Emitv(kDebug, format, args);
}

inline void Logger::Info(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kInfo, format, args);
  va_end(args);
}

inline void Logger::Infov(const char *format, va_list args) {
  Emitv(kInfo, format, args);
}

inline void Logger::Notice(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kNotice, format, args);
  va_end(args);
}

inline void Logger::Noticev(const char *format, va_list args) {
  Emitv(kNotice, format, args);
}

inline void Logger::Warn(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kWarn, format, args);
  va_end(args);
}

inline void Logger::Warnv(const char *format, va_list args) {
  Emitv(kWarn, format, args);
}

inline void Logger::Error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kError, format, args);
  va_end(args);
}

inline void Logger::Errorv(const char *format, va_list args) {
  Emitv(kError, format, args);
}

inline void Logger::Crit(const char *format, ...) {
  va_list args;
  va_start(args, format);
  Emitv(kCrit, format, args);
  va_end(args);
}

inline void Logger::Critv(const char *format, va_list args) {
  Emitv(kCrit, format, args);
}

inline void Logger::PushEmitter(LogEmitter &emitter) {
  emitters_.push_back(&emitter);
}

inline void Logger::ClearEmitters() {
  emitters_.clear();
}
