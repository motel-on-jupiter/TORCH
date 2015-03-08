/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef CORE_FRAME_TORCHMAINFRAME_H_
#define CORE_FRAME_TORCHMAINFRAME_H_

#include "includer/wx_include.h"

class TorchMainFrame : public wxFrame {
 public:
  TorchMainFrame::TorchMainFrame(const wxString &title, const wxPoint &pos,
                                 const wxSize &size);
  virtual ~TorchMainFrame();

  bool Initialize();

 private:
  void OnExit(wxCommandEvent& WXUNUSED(event));
  void OnAbout(wxCommandEvent& WXUNUSED(event));
  wxAuiManager aui_manager_;

  wxDECLARE_EVENT_TABLE();
};

#endif /* CORE_FRAME_TORCHMAINFRAME_H_ */
