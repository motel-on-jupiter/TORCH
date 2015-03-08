/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#include "particle/loader/FileParticleLoader.h"
#include "auxiliary/csyntax_aux.h"
#include "includer/spark_include.h"
#include "logging/Logger.h"
#include "particle/loader/ParticleLoader.h"

FileParticleLoader::FileParticleLoader(const char *path)
: ParticleLoader(),
  path_(path) {
}

FileParticleLoader::~FileParticleLoader() {
}

bool FileParticleLoader::Load(SPK::Ref<SPK::System> &system) {
  system = SPK::IO::Manager::get().load(path_);
  // Renderer settings is not supported for importing
  SPK::Ref<SPK::GL::GLQuadRenderer> renderer = SPK::GL::GLQuadRenderer::create();
  if (!renderer) {
    LOGGER.Error("Failed to create particle renderer");
    return false;
  }
  for (size_t i=0; i<system->getNbGroups(); ++i) {
    system->getGroup(i)->setRenderer(renderer);
  }
  return true;
}
