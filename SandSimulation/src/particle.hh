#pragma once

#include <SFML/Graphics/Color.hpp>

#include "elementType.hh"

struct Particle {
  bool hasUpdated;
  ElementType type;
  sf::Color color;

  Particle() : type(ElementType::Empty), color(sf::Color()) {
  }

  Particle(ElementType elementType) {
    type = elementType;
    color = getAttributes(type).color;
  }
};
