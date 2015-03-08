/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#ifndef PARTICLE_LOADER_FILEPARTICLELOADER_H_
#define PARTICLE_LOADER_FILEPARTICLELOADER_H_

#include <string>
#include "includer/spark_include.h"
#include "particle/loader/ParticleLoader.h"

class FileParticleLoader : public ParticleLoader {
 public:
  FileParticleLoader(const char *path);
  virtual ~FileParticleLoader();

  virtual bool Load(SPK::Ref<SPK::System> &system);

 private:
  std::string path_;
};

#endif /* PARTICLE_LOADER_FILEPARTICLELOADER_H_ */
