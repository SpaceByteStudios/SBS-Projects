#pragma once

#include "SFML/Graphics/Color.hpp"

enum class ElementType { Empty, Sand, Water };

struct ElementAttributes {
  sf::Color color;
};

ElementAttributes getAttributes(ElementType type);
