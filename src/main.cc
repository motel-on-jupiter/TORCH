/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "auxiliary/csyntax_aux.h"

class TORCHFrame : public wxFrame {
 public:
  TORCHFrame::TORCHFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
      : wxFrame(NULL, wxID_ANY, title, pos, size) {
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    SetMenuBar(menu_bar);
    CreateStatusBar();
  }
  virtual ~TORCHFrame() {
  }

 private:
  void OnExit(wxCommandEvent &event) {
    UNUSED(event);
    Close(true);
  }

  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(TORCHFrame, wxFrame)
  EVT_MENU(wxID_EXIT, TORCHFrame::OnExit)
wxEND_EVENT_TABLE()

class TORCHApp : public wxApp {
 public:
  virtual bool OnInit() {
    TORCHFrame *frame = new TORCHFrame("TORCH v0.0.1", wxPoint(50, 50),
                                       wxSize(900, 680));
    frame->Show(true);
    return true;
  }
};

wxIMPLEMENT_APP(TORCHApp);

