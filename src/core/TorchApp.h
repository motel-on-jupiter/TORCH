/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef CORE_TORCHAPP_H_
#define CORE_TORCHAPP_H_

#include <string>
#include "includer/wx_include.h"

class TorchApp : public wxApp {
 public:
  static const std::string kName;

  virtual bool OnInit();
  virtual int OnExit();
};

#endif /* CORE_TORCHAPP_H_ */
