/**
 * Copyright (C) 2015 The Motel on Jupiter
 */
#include "particle/loader/TorchSampleParticleLoader.h"
#include "auxiliary/csyntax_aux.h"
#include "includer/spark_include.h"
#include "logging/Logger.h"
#include "particle/loader/ParticleLoader.h"

bool TorchSampleParticleLoader::Load(SPK::Ref<SPK::System> spk_system) {
  SPK::Ref<SPK::Group> group = spk_system->createGroup(1);
  if (!group) {
    LOGGER.Error("Failed to create particle group");
    return false;
  }
  SPK::Ref<SPK::Emitter> emitter = SPK::NormalEmitter::create();
  if (!emitter) {
    LOGGER.Error("Failed to create particle emitter");
    return false;
  }
  group->addEmitter(emitter);
  SPK::Ref<SPK::Renderer> renderer = SPK::GL::GLQuadRenderer::create();
  if (!renderer) {
    LOGGER.Error("Failed to create particle renderer");
    return false;
  }
  group->setRenderer(renderer);
  return true;
}
