/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "auxiliary/csyntax_aux.h"
#include "includer/gl_include.h"
#include "includer/glm_include.h"
#include "includer/spark_include.h"
#include "includer/wx_include.h"
#include "logging/emitter/DebuggerConsoleLogEmitter.h"
#include "logging/Logger.h"

namespace TORCH {

class PreviewCanvas : public wxGLCanvas {
 public:
  PreviewCanvas(wxFrame *parent, int *args);
  virtual ~PreviewCanvas();

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

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
  EVT_SIZE(PreviewCanvas::OnResize)
  EVT_PAINT(PreviewCanvas::OnPaint)
  EVT_TIMER(0, PreviewCanvas::OnTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(wxFrame *parent, int *args)
    : wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE),
      context_(nullptr),
      timer_(nullptr),
      playing_time_(0),
      particle_system_() {
}

PreviewCanvas::~PreviewCanvas() {
  if (timer_ != nullptr) {
    timer_->Stop();
    delete timer_;
  }
  delete context_;
}

bool PreviewCanvas::Initialize() {
  // Create GL context
  context_ = new wxGLContext(this);
  if (context_ == nullptr) {
    LOGGER.Error("Failed to allocate for GL context object");
    return false;
  }

  // Set up timer
  timer_ = new wxTimer(this, 0);
  if (timer_ == nullptr) {
    LOGGER.Error("Failed to allocate for timer object");
    CleanUp();
    return false;
  }
  if (!(timer_->Start(kPreviewUpdateFrequency))) {
    LOGGER.Error("Failed to start timer");
    CleanUp();
    return false;
  }

  // To avoid flashing on MSW
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);

  // Set up particle system
  SPK::System::useConstantStep(1.0f / 30.0f);
  particle_system_ = SPK::System::create(true);
  if (!particle_system_) {
    LOGGER.Error("Failed to create particle system");
    CleanUp();
    return false;
  }
  SPK::Ref<SPK::Group> group = particle_system_->createGroup(1);
  if (!group) {
    LOGGER.Error("Failed to create particle group");
    CleanUp();
    return false;
  }
  SPK::Ref<SPK::Emitter> emitter = SPK::NormalEmitter::create();
  if (!emitter) {
    LOGGER.Error("Failed to create particle emitter");
    CleanUp();
    return false;
  }
  group->addEmitter(emitter);
  SPK::Ref<SPK::Renderer> renderer = SPK::GL::GLQuadRenderer::create();
  if (!renderer) {
    LOGGER.Error("Failed to create particle renderer");
    CleanUp();
    return false;
  }
  group->setRenderer(renderer);
  return true;
}

void PreviewCanvas::OnResize(wxSizeEvent& WXUNUSED(event)) {
  Refresh();
}

void PreviewCanvas::OnPaint(wxPaintEvent& WXUNUSED(event)) {
  if (!IsShown()) {
    return;
  }

  wxGLCanvas::SetCurrent(*context_);
  wxPaintDC(this);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glViewport(0, 0, GetSize().x, GetSize().y);

  glMatrixMode(GL_PROJECTION);
  float aspect = static_cast<float>(GetSize().x) / static_cast<float>(GetSize().y);
  glLoadMatrixf(glm::value_ptr(glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f)));
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(glm::translate(glm::vec3(0.0f, 0.0f, -5.0f))));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor4f(0.2f, 0.2f, 0.5f, 1.0f);
  glBegin(GL_LINES);
  for (float x = -500.0f; x < 500.0f; x += 1.0f) {
    glVertex3f(x, 0.0f, -500.0f);
    glVertex3f(x, 0.0f, 500.0f);
  }
  for (float z = -500.0f; z < 500.0f; z += 1.0f) {
    glVertex3f(-500.0f, 0.0f, z);
    glVertex3f(500.0f, 0.0f, z);
  }
  glEnd();

  if (particle_system_) {
    particle_system_->renderParticles();
  }

  glFlush();
  SwapBuffers();
}

void PreviewCanvas::OnTimer(wxTimerEvent &event) {
  if (event.GetTimer().GetId() == timer_->GetId()) {
    playing_time_ += timer_->GetInterval();
    if (particle_system_) {
      particle_system_->updateParticles(
          static_cast<float>(timer_->GetInterval()) * 0.001f);
    }
    Refresh();
  }
}

void PreviewCanvas::CleanUp() {
  particle_system_ = SPK_NULL_REF;
  if (timer_ != nullptr) {
    timer_->Stop();
    delete timer_;
    timer_ = nullptr;
  }
  delete context_;
  context_ = nullptr;
}

class MainFrame : public wxFrame {
 public:
  MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                       const wxSize &size)
      : wxFrame(NULL, wxID_ANY, title, pos, size) {
  }
  virtual ~MainFrame() {
  }

  bool Initialize() {
    // Set up menu bar
    wxMenu *file_menu = new wxMenu;
    if (file_menu == nullptr) {
      LOGGER.Error("Failed to allocate for file menu object");
      return false;
    }
    wxMenuItem *menu_item = file_menu->Append(wxID_EXIT);
    if (menu_item == nullptr) {
      LOGGER.Error("Failed to append exit item to file menu");
      return false;
    }
    wxMenu *help_menu = new wxMenu;
    if (help_menu == nullptr) {
      LOGGER.Error("Failed to allocate for help menu object");
      return false;
    }
    menu_item = help_menu->Append(wxID_ABOUT);
    if (menu_item == nullptr) {
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
    wxStatusBar *status_bar = CreateStatusBar();
    if (status_bar == nullptr) {
      LOGGER.Error("Failed to create status bar");
      return false;
    }

    // Set up preview canvas
    int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
    PreviewCanvas *canvas = new PreviewCanvas(this, args);
    if (canvas == nullptr) {
      LOGGER.Error("Failed to allocate for preview canvas object");
      return false;
    }
    if (!canvas->Initialize()) {
      return false;
    }
    return true;
  }

 private:
  void OnExit(wxCommandEvent& WXUNUSED(event)) {
    Close(true);
  }

  void OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxAboutDialogInfo info;
    info.SetName(_("TORCH v0.0.2"));
    info.SetDescription(_("A particle designer for SPARK engine"));
    info.SetCopyright(wxT("Copyright (C) 2014 The Motel on Jupiter"));
    info.AddDeveloper(wxT("Bobo Zeta"));
    wxAboutBox(info);
  }

  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MainFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

class App : public wxApp {
 public:
  virtual bool OnInit() {
#if defined(_MSC_VER) && defined(_DEBUG)
    // Validate the detection for memory leak
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Set up logger
    LOGGER.PushEmitter(DEBUGGER_CONSOLE_LOG_EMITTER);
    LOGGER.Info("Initialize application");

    // Set up window
    MainFrame *frame = new MainFrame("TORCH v0.0.2", wxPoint(50, 50),
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

  virtual int OnExit() {
    LOGGER.Info("Finalize application");
    return 0;
  }
};

wxIMPLEMENT_APP(App);

}
