/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifdef _MSC_VER

#include "logging/emitter/DebuggerConsoleLogEmitter.h"
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>

DebuggerConsoleLogEmitter::DebuggerConsoleLogEmitter() {
}

DebuggerConsoleLogEmitter::~DebuggerConsoleLogEmitter() {
}

int DebuggerConsoleLogEmitter::Initialize() {
  return 0;
}

int DebuggerConsoleLogEmitter::Finalize() {
  return 0;
}

int DebuggerConsoleLogEmitter::EmitMultibyteChars(const char *mbchars) {
  // Convert to wide characters
  TCHAR buf[kMaxEmittableSize] = { 0 };
  size_t prefixlen = 0;
  errno_t error = mbstowcs_s(&prefixlen, buf, kMaxEmittableSize, mbchars,
                             kMaxEmittableSize);
  if (ERANGE == error) {
    OutputDebugString(
        TEXT(
            "\n[LOG_EMITTER ERROR]: Message is too long to copy to the buffer\n"));
    return -1;
  } else if (EINVAL == error) {
    OutputDebugString(
        TEXT(
            "\n[LOG_EMITTER ERROR]: mbstowcs_s() called with invald arguments to copy the message to the buffer\n"));
    return -1;
  }

  // Output to console
  OutputDebugString(buf);
  return 0;
}

int DebuggerConsoleLogEmitter::EmitPrefix(const char *prefix) {
  return EmitMultibyteChars(prefix);
}

int DebuggerConsoleLogEmitter::EmitBody(const char *format, va_list args) {
  // Format the message
  char mbsbuf[kMaxEmittableSize] = { 0 };
  size_t mbslen = vsnprintf_s(mbsbuf, kMaxEmittableSize, _TRUNCATE, format,
                              args);
  if (mbslen >= kMaxEmittableSize) {
    OutputDebugString(
        TEXT(
            "\n[LOG_EMITTER ERROR]: Message body is too long to copy the buffer\n"));
    return -1;
  }

  // Emit the message
  return EmitMultibyteChars(mbsbuf);
}

void DebuggerConsoleLogEmitter::Emitv(const char *prefix, const char *format,
                                      va_list args) {
  if (0 > EmitPrefix(prefix)) {
    OutputDebugString(
        TEXT("[LOG_EMITTER ERROR]: Failed to emit the message prefix\n"));
    return;
  }
  if (0 > EmitBody(format, args)) {
    OutputDebugString(
        TEXT("\n[LOG_EMITTER ERROR]: Failed to emit the message body\n"));
    return;
  }
  OutputDebugString(TEXT("\n"));
}

#endif /* _MSC_VER */
