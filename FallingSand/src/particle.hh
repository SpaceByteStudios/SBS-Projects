#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

#include "elementType.hh"

struct Particle {
  ElementType type;
  sf::Color color;

  Particle() : type(ElementType::Empty), color(0, 0, 0) {
  }

  Particle(ElementType elementType, sf::Color particleColor) {
    type = elementType;
    color = particleColor;
  }
};
