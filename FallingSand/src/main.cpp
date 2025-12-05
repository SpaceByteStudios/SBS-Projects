#include <iostream>

#include <SFML/Graphics.hpp>

#include "particle.hh"

int main()
{
  sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({500u, 500u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  
  window.setPosition(sf::Vector2i(1320, 100));
  
  int const grid_size = 20;
  sf::Vector2i const grid_dim = {(int)window.getSize().x / grid_size, (int)window.getSize().y / grid_size};

  std::vector<Particle> grid;
  grid.resize(grid_dim.x * grid_dim.y);

  while (window.isOpen())
  {
    while (const std::optional event = window.pollEvent())
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
    }

    window.clear();
    window.display();
  }
}
