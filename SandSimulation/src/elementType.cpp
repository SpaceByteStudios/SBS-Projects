#include "elementType.hh"

#include <SFML/Graphics/Color.hpp>

ElementAttributes getAttributes(ElementType type) {
  switch (type) {
    case ElementType::Empty:
      return ElementAttributes{sf::Color()};
    case ElementType::Sand:
      return ElementAttributes{sf::Color(255, 255, 0)};
    case ElementType::Water:
      return ElementAttributes{sf::Color(0, 0, 255)};
  }
}
