#include "simulation.hh"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "elementType.hh"
#include "particle.hh"

Simulation::Simulation() {
  isRunning = true;

  size = sf::Vector2u(10, 10);

  front_grid.resize(size.x * size.y);
  back_grid.resize(size.x * size.y);

  window = sf::RenderWindow(sf::VideoMode({500u, 500u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  window.setPosition({1320, 100});
}

Simulation::Simulation(int width, int height) {
  isRunning = true;

  size = sf::Vector2u(width, height);

  front_grid.resize(size.x * size.y);
  back_grid.resize(size.x * size.y);

  window = sf::RenderWindow(sf::VideoMode({500u, 500u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  window.setPosition({1320, 100});
}

void Simulation::run() {
  for (int y = size.y - 1; y >= 0; y--) {
    for (int x = size.x - 1; x >= 0; x--) {
      int index = y * size.x + x;

      switch (front_grid[index].type) {
        case ElementType::Empty:
          back_grid[index] = front_grid[index];
          break;

        case ElementType::Sand:
          updateSand(x, y);
          break;

        default:
          back_grid[index] = front_grid[index];
          break;
      }
    }
  }

  std::swap(front_grid, back_grid);
}

void Simulation::draw() {
  window.clear();

  sf::Vector2f pixel_size((float)window.getSize().x / size.x, (float)window.getSize().y / size.y);

  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      int index = y * size.x + x;

      sf::RectangleShape rect({pixel_size});
      rect.setPosition({x * pixel_size.x, y * pixel_size.y});
      rect.setFillColor(front_grid[index].color);

      window.draw(rect);
    }
  }

  window.display();
}

void Simulation::processInput() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }

    if (event->is<sf::Event::KeyPressed>()) {
      if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
        window.close();
      }
    }
  }
}

void Simulation::addParticle(const Particle& particle, const sf::Vector2u& pos) {
  int index = pos.y * size.y + pos.x;

  if (index < front_grid.size()) {
    front_grid[index] = particle;
  }
}

void Simulation::updateSand(int x, int y) {
  int index = y * size.x + x;
  if (y + 1 >= size.y) {
    back_grid[index] = front_grid[index];
    return;
  }

  int below_index = (y + 1) * size.x + x;
  if (front_grid[below_index].type == ElementType::Empty) {
    back_grid[below_index] = front_grid[index];
    return;
  }

  int below_left_index = (y + 1) * size.x + (x - 1);
  if (x - 1 >= 0 && front_grid[below_left_index].type == ElementType::Empty) {
    back_grid[below_left_index] = front_grid[index];
    return;
  }

  int below_right_index = (y + 1) * size.x + (x + 1);
  if (x + 1 < size.x && front_grid[below_right_index].type == ElementType::Empty) {
    back_grid[below_right_index] = front_grid[index];
    return;
  }

  back_grid[index] = front_grid[index];
}
