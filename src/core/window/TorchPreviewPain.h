/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef CORE_WINDOW_TORCHPREVIEWPAIN_H_
#define CORE_WINDOW_TORCHPREVIEWPAIN_H_

#include "includer/spark_include.h"
#include "includer/wx_include.h"
#include "logging/Logger.h"

class TorchPreviewCanvas : public wxGLCanvas {
 public:
  TorchPreviewCanvas(wxFrame *parent, int *args);
  virtual ~TorchPreviewCanvas();

  bool Initialize();

  void OnResize(wxSizeEvent &event);
  void OnPaint(wxPaintEvent &event);
  void OnTimer(wxTimerEvent &event);

  DECLARE_EVENT_TABLE()

 private:
  static const int kPreviewUpdateFrequency = 1000 / 30;

  void CleanUp();

  wxGLContext *context_;
  wxTimer *timer_;
  uint64_t playing_time_;
  SPK::Ref<SPK::System> particle_system_;
};

#endif /* CORE_WINDOW_TORCHPREVIEWPAIN_H_ */
