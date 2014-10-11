/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/glcanvas.h>
#include "auxiliary/csyntax_aux.h"
#include "wrapper/glgraphics_wrap.h"

class PreviewCanvas : public wxGLCanvas {
 public:
  PreviewCanvas(wxFrame *parent, int *args);
  virtual ~PreviewCanvas();

  void OnResize(wxSizeEvent &evt);
  void OnPaint(wxPaintEvent &evt);

  DECLARE_EVENT_TABLE()

 private:
  wxGLContext *context_;
  GLfloat vertices_[8][3];
};

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
  EVT_SIZE(PreviewCanvas::OnResize)
  EVT_PAINT(PreviewCanvas::OnPaint)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(wxFrame *parent, int *args)
    : wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE),
      context_(nullptr) {
  context_ = new wxGLContext(this);
  vertices_[0][0] = vertices_[1][0] = vertices_[2][0] = vertices_[3][0] = -1.0f;
  vertices_[4][0] = vertices_[5][0] = vertices_[6][0] = vertices_[7][0] = 1.0f;
  vertices_[0][1] = vertices_[1][1] = vertices_[4][1] = vertices_[5][1] = -1.0f;
  vertices_[2][1] = vertices_[3][1] = vertices_[6][1] = vertices_[7][1] = 1.0f;
  vertices_[0][2] = vertices_[3][2] = vertices_[4][2] = vertices_[7][2] = 1.0f;
  vertices_[1][2] = vertices_[2][2] = vertices_[5][2] = vertices_[6][2] = -1.0f;

  // To avoid flashing on MSW
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

PreviewCanvas::~PreviewCanvas() {
  delete context_;
}

void PreviewCanvas::OnResize(wxSizeEvent& event) {
  UNUSED(event);
  Refresh();
}

void PreviewCanvas::OnPaint(wxPaintEvent& event) {
  UNUSED(event);

  if (!IsShown()) {
    return;
  }

  wxGLCanvas::SetCurrent(*context_);
  wxPaintDC(this);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);

  glViewport(0.0f, 0.0f, GetSize().x, GetSize().y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, GetSize().x / GetSize().y, 0.1f, 200.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(50.0f, 0.0f, 1.0f, 0.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  static const GLint faces[6][4] = { { 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5,
      4 }, { 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 } };
  for (int i = 0; i < 6; i++) {
    glBegin(GL_LINE_STRIP);
    glVertex3fv(&vertices_[faces[i][0]][0]);
    glVertex3fv(&vertices_[faces[i][1]][0]);
    glVertex3fv(&vertices_[faces[i][2]][0]);
    glVertex3fv(&vertices_[faces[i][3]][0]);
    glVertex3fv(&vertices_[faces[i][0]][0]);
    glEnd();
  }
  glFlush();
  SwapBuffers();
}

class TORCHFrame : public wxFrame {
 public:
  TORCHFrame::TORCHFrame(const wxString &title, const wxPoint &pos,
                         const wxSize &size)
      : wxFrame(NULL, wxID_ANY, title, pos, size),
        preview_canvas_(nullptr) {
    // Set up menu bar
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(wxID_EXIT);
    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    SetMenuBar(menu_bar);

    // Set up status bar
    CreateStatusBar();

    // Set up preview canvas
    int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
    preview_canvas_ = new PreviewCanvas(this, args);
  }
  virtual ~TORCHFrame() {
    delete preview_canvas_;
  }

 private:
  void OnExit(wxCommandEvent &event) {
    UNUSED(event);
    Close(true);
  }

  wxDECLARE_EVENT_TABLE();

  PreviewCanvas *preview_canvas_;
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

