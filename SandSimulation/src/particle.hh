#pragma once

#include "elementType.hh"

struct Particle {
  bool hasUpdated;
  ElementType type;

  Particle() : type(ElementType::Empty) {
  }

  Particle(ElementType elementType) {
    type = elementType;
  }
};
