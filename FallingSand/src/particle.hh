#pragma once

#include <SFML/Graphics.hpp>

class Particle{
  private:
    sf::Vector2i pos;
    sf::Color color;
  public:
    Particle();
    Particle(sf::Vector2i& position);
};
