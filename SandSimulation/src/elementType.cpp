#include "elementType.hh"

#include <SFML/Graphics/Color.hpp>

ElementAttributes getAttributes(ElementType type) {
  switch (type) {
    case ElementType::Empty:
      return ElementAttributes{sf::Color(), false};
    case ElementType::Sand:
      return ElementAttributes{sf::Color(255, 255, 0), false};
    case ElementType::Water:
      return ElementAttributes{sf::Color(0, 0, 255), true};
    case ElementType::Stone:
      return ElementAttributes{sf::Color(128, 128, 128), false};
    case ElementType::Wood:
      return ElementAttributes{sf::Color(128, 64, 0), false, 10};
    case ElementType::Fire:
      return ElementAttributes{sf::Color(255, 128, 0), false, 0, 5};
  }
}
