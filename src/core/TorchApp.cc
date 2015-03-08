/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "core/TorchApp.h"
#include "core/frame/TorchMainFrame.h"
#include "includer/wx_include.h"
#include "logging/emitter/DebuggerConsoleLogEmitter.h"
#include "logging/Logger.h"

const char TorchApp::kName[] = "TORCH v0.1.0";

bool TorchApp::OnInit() {
#if defined(_MSC_VER) && defined(_DEBUG)
  // Validate the detection for memory leak
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // Set up logger
  LOGGER.PushEmitter(DEBUGGER_CONSOLE_LOG_EMITTER);
  LOGGER.Info("Initialize application");

  // Set up window
  TorchMainFrame *frame = new TorchMainFrame(kName, wxPoint(50, 50),
                                             wxSize(900, 680));
  if (frame == nullptr) {
    LOGGER.Error("Failed to allocate for frame object");
    return false;
  }
  if (!frame->Initialize()) {
    return false;
  }

  // Show window
  LOGGER.Info("Show window");
  if (!frame->Show(true)) {
    LOGGER.Error("Failed to show frame");
    return false;
  }
  return true;
}

int TorchApp::OnExit() {
  LOGGER.Info("Finalize application");
  return 0;
}
