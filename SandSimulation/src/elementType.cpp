#include "elementType.hh"

#include <SFML/Graphics/Color.hpp>
#include <random>

sf::Color mapColor(const sf::Color& a, const sf::Color& b, float t) {
  return sf::Color(static_cast<std::uint8_t>(a.r + (b.r - a.r) * t),
                   static_cast<std::uint8_t>(a.g + (b.g - a.g) * t),
                   static_cast<std::uint8_t>(a.b + (b.b - a.b) * t));
}

ElementAttributes getAttributes(ElementType type) {
  switch (type) {
    case ElementType::Empty: {
      return ElementAttributes{sf::Color(), false};
    }

    case ElementType::Sand: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(255, 192, 0), sf::Color(255, 255, 0), t);
      return ElementAttributes{color, false};
    }

    case ElementType::Water: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(0, 32, 255), sf::Color(0, 0, 255), t);
      return ElementAttributes{color, true};
    }

    case ElementType::Steam: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(128, 160, 255), sf::Color(160, 176, 255), t);
      return ElementAttributes{color, false};
    }

    case ElementType::Stone: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(100, 100, 100), sf::Color(156, 156, 156), t);
      return ElementAttributes{color, false};
    }

    case ElementType::Wood: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(128, 48, 0), sf::Color(128, 80, 0), t);
      return ElementAttributes{color, false, 7};
    }

    case ElementType::Fire: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(255, 64, 0), sf::Color(255, 192, 0), t);
      return ElementAttributes{color, false, 0, 5};
    }

    case ElementType::Oil: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(160, 128, 64), sf::Color(192, 144, 64), t);
      return ElementAttributes{color, true, 50};
    }

    case ElementType::Acid: {
      float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      sf::Color color = mapColor(sf::Color(32, 192, 32), sf::Color(80, 255, 80), t);
      return ElementAttributes{color, true, 15};
    }
  }
}
