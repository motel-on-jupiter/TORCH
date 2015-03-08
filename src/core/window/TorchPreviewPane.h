/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef CORE_WINDOW_TORCHPREVIEWPANE_H_
#define CORE_WINDOW_TORCHPREVIEWPANE_H_

#include "includer/spark_include.h"
#include "includer/wx_include.h"
#include "logging/Logger.h"
#include "particle/loader/TorchSampleParticleLoader.h"

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
  TorchSampleParticleLoader particle_loader_;
  SPK::Ref<SPK::System> particle_;
};

#endif /* CORE_WINDOW_TORCHPREVIEWPANE_H_ */
