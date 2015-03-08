/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef PARTICLE_LOADER_PARTICLELOADER_H_
#define PARTICLE_LOADER_PARTICLELOADER_H_

#include "includer/spark_include.h"

class ParticleLoader {
 public:
  ParticleLoader() {}
  virtual ~ParticleLoader() {}

  virtual bool Load(SPK::Ref<SPK::System> spk_system) = 0;
};

#endif /* PARTICLE_LOADER_PARTICLELOADER_H_ */
