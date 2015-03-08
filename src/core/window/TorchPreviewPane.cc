/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#include "core/window/TorchPreviewPane.h"
#include "includer/gl_include.h"
#include "includer/glm_include.h"
#include "includer/spark_include.h"
#include "includer/wx_include.h"
#include "logging/Logger.h"
#include "particle/loader/TorchSampleParticleLoader.h"

TorchPreviewCanvas::TorchPreviewCanvas(wxFrame *parent, int *args)
    : wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize,
                 wxFULL_REPAINT_ON_RESIZE),
      context_(nullptr),
      timer_(nullptr),
      playing_time_(0),
      particle_loader_(),
      particle_() {
}

TorchPreviewCanvas::~TorchPreviewCanvas() {
  if (timer_ != nullptr) {
    timer_->Stop();
    delete timer_;
  }
  delete context_;
}

bool TorchPreviewCanvas::Initialize() {
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
  particle_ = SPK::System::create(true);
  if (!particle_) {
    LOGGER.Error("Failed to create particle system");
    CleanUp();
    return false;
  }
  SPK::System::useConstantStep(1.0f / 30.0f);
  if (!particle_loader_.Load(particle_)) {
    CleanUp();
    return false;
  }
  return true;
}

void TorchPreviewCanvas::OnResize(wxSizeEvent& WXUNUSED(event)) {
  Refresh();
}

void TorchPreviewCanvas::OnPaint(wxPaintEvent& WXUNUSED(event)) {
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

  if (particle_) {
    particle_->renderParticles();
  }

  glFlush();
  SwapBuffers();
}

void TorchPreviewCanvas::OnTimer(wxTimerEvent &event) {
  if (event.GetTimer().GetId() == timer_->GetId()) {
    playing_time_ += timer_->GetInterval();
    if (particle_) {
      particle_->updateParticles(
          static_cast<float>(timer_->GetInterval()) * 0.001f);
    }
    Refresh();
  }
}

BEGIN_EVENT_TABLE(TorchPreviewCanvas, wxGLCanvas)
  EVT_SIZE(TorchPreviewCanvas::OnResize)
  EVT_PAINT(TorchPreviewCanvas::OnPaint)
  EVT_TIMER(0, TorchPreviewCanvas::OnTimer)
END_EVENT_TABLE()

void TorchPreviewCanvas::CleanUp() {
  particle_ = SPK_NULL_REF;
  if (timer_ != nullptr) {
    timer_->Stop();
    delete timer_;
    timer_ = nullptr;
  }
  delete context_;
  context_ = nullptr;
}
