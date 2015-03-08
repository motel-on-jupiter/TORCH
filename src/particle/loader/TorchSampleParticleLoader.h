/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef PARTICLE_LOADER_TORCHSAMPLEPARTICLELOADER_H_
#define PARTICLE_LOADER_TORCHSAMPLEPARTICLELOADER_H_

#include "includer/spark_include.h"
#include "particle/loader/ParticleLoader.h"

class TorchSampleParticleLoader : public ParticleLoader {
 public:
  TorchSampleParticleLoader() : ParticleLoader() {}
  virtual ~TorchSampleParticleLoader() {}

  virtual bool Load(SPK::Ref<SPK::System> spk_system);
};

#endif /* PARTICLE_LOADER_TORCHSAMPLEPARTICLELOADER_H_ */
