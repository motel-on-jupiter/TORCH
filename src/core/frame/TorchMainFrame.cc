/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#include "core/frame/TorchMainFrame.h"
#include "core/TorchApp.h"
#include "core/window/TorchPreviewPane.h"
#include "includer/wx_include.h"
#include "logging/Logger.h"
#include "particle/loader/FileParticleLoader.h"

TorchMainFrame::TorchMainFrame(const wxString &title, const wxPoint &pos,
                               const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size,
              wxDEFAULT_FRAME_STYLE |wxSUNKEN_BORDER),
      aui_manager_(),
      canvas_(nullptr) {
  aui_manager_.SetManagedWindow(this);
}

TorchMainFrame::~TorchMainFrame() {
  aui_manager_.UnInit();
}

bool TorchMainFrame::Initialize() {
  // Set up menu bar
  wxMenu *file_menu = new wxMenu;
  if (file_menu == nullptr) {
    LOGGER.Error("Failed to allocate for file menu object");
    return false;
  }
  if (file_menu->Append(wxID_IMPORT, "Import",
                        "Import the setting of partcle system from exported file") == nullptr) {
    LOGGER.Error("Failed to append import item to file menu");
    return false;
  }
  if (file_menu->Append(wxID_EXIT) == nullptr) {
    LOGGER.Error("Failed to append exit item to file menu");
    return false;
  }
  wxMenu *help_menu = new wxMenu;
  if (help_menu == nullptr) {
    LOGGER.Error("Failed to allocate for help menu object");
    return false;
  }
  if (help_menu->Append(wxID_ABOUT) == nullptr) {
    LOGGER.Error("Failed to append exit item to file menu");
    return false;
  }
  wxMenuBar *menu_bar = new wxMenuBar;
  if (menu_bar == nullptr) {
    LOGGER.Error("Failed to allocate for menu bar object");
    return false;
  }
  if (!menu_bar->Append(file_menu, "&File")) {
    LOGGER.Error("Failed to append file menu to menu bar");
    return false;
  }
  if (!menu_bar->Append(help_menu, "&Help")) {
    LOGGER.Error("Failed to append help menu to menu bar");
    return false;
  }
  SetMenuBar(menu_bar);

  // Set up status bar
  if (CreateStatusBar() == nullptr) {
    LOGGER.Error("Failed to create status bar");
    return false;
  }

  // Set up preview canvas
  int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
  canvas_ = new TorchPreviewCanvas(this, args);
  if (canvas_ == nullptr) {
    LOGGER.Error("Failed to allocate for preview canvas object");
    return false;
  }
  if (!canvas_->Initialize()) {
    return false;
  }

  // Set up preview pain
  wxAuiPaneInfo info;
  info.DefaultPane();
  info.Name(wxT("preview"));
  info.Caption(wxT("Preview"));
  info.PaneBorder(true);
  info.MaximizeButton(true);
  info.MinimizeButton(true);
  info.PinButton(true);
  aui_manager_.AddPane(canvas_, info);
  aui_manager_.Update();
  return true;
}

void TorchMainFrame::OnImport(wxCommandEvent& WXUNUSED(event)) {
  wxFileDialog dialog(this);
  if (dialog.ShowModal() == wxID_OK) {
    FileParticleLoader loader(dialog.GetPath());
    loader.Load(canvas_->particle());
  }
}

void TorchMainFrame::OnExit(wxCommandEvent& WXUNUSED(event)) {
  Close(true);
}

void TorchMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
  wxAboutDialogInfo info;
  info.SetName(TorchApp::kName);
  info.SetDescription(_("A particle designer for SPARK engine"));
  info.SetCopyright(wxT("Copyright (C) 2014 The Motel on Jupiter"));
  info.AddDeveloper(wxT("Bobo Zeta"));
  wxAboutBox(info);
}

wxBEGIN_EVENT_TABLE(TorchMainFrame, wxFrame)
  EVT_MENU(wxID_IMPORT, TorchMainFrame::OnImport)
  EVT_MENU(wxID_EXIT, TorchMainFrame::OnExit)
  EVT_MENU(wxID_ABOUT, TorchMainFrame::OnAbout)
wxEND_EVENT_TABLE()
