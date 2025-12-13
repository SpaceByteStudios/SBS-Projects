#pragma once

#include "SFML/Graphics/Color.hpp"

enum class ElementType { Empty, Sand, Water, Steam, Stone, Wood, Fire, Oil };

struct ElementAttributes {
  sf::Color color;
  bool is_fluid;
  int flammabilaty;
  int decay;
};

ElementAttributes getAttributes(ElementType type);

sf::Color mapColor(const sf::Color& a, const sf::Color& b, float t);
