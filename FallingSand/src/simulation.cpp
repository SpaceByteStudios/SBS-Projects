#include "simulation.hh"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <iostream>

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

  cell_size = sf::Vector2f((float)window.getSize().x / size.x, (float)window.getSize().y / size.y);
}

Simulation::Simulation(int width, int height) {
  isRunning = true;

  size = sf::Vector2u(width, height);

  front_grid.resize(size.x * size.y);
  back_grid.resize(size.x * size.y);

  window = sf::RenderWindow(sf::VideoMode({500u, 500u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  window.setPosition({1320, 100});

  cell_size = sf::Vector2f((float)window.getSize().x / size.x, (float)window.getSize().y / size.y);
}

void Simulation::run() {
  updateParticles();

  calculatePlacePositions();
}

void Simulation::draw() {
  window.clear();

  drawParticles();

  showPlaceRadius();

  window.display();
}

void Simulation::processInput() {
  while (const std::optional event = window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window.close();
    }

    if (event->is<sf::Event::KeyPressed>()) {
      auto key = event->getIf<sf::Event::KeyPressed>()->code;
      if (key == sf::Keyboard::Key::Escape) {
        window.close();
      }

      if (key == sf::Keyboard::Key::Add || key == sf::Keyboard::Key::Equal) {
        place_radius += 0.5;
      }

      if (key == sf::Keyboard::Key::Subtract || key == sf::Keyboard::Key::Hyphen) {
        place_radius -= 0.5;

        if (place_radius < 0) {
          place_radius = 0;
        }
      }

      if (key == sf::Keyboard::Key::S) {
        place_type = ElementType::Sand;
      }

      if (key == sf::Keyboard::Key::W) {
        place_type = ElementType::Water;
      }
    }

    if (event->is<sf::Event::MouseWheelScrolled>()) {
      auto scroll = event->getIf<sf::Event::MouseWheelScrolled>()->delta;

      if (scroll > 0) {
        place_radius += 0.5;
      } else if (scroll < 0) {
        place_radius -= 0.5;
      }
    }
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2u cell_pos(mouse_pos.x / cell_size.x, mouse_pos.y / cell_size.y);

    for (sf::Vector2i& pos : place_positions) {
      float dist = std::sqrt((pos.x - cell_pos.x) * (pos.x - cell_pos.x) +
                             (pos.y - cell_pos.y) * (pos.y - cell_pos.y));

      if (dist > place_radius) {
        continue;
      }

      ElementAttributes particle_attr = getAttributes(place_type);
      addParticle(Particle(place_type, particle_attr.color), sf::Vector2u(pos.x, pos.y));
    }
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2u cell_pos(mouse_pos.x / cell_size.x, mouse_pos.y / cell_size.y);

    for (sf::Vector2i& pos : place_positions) {
      float dist = std::sqrt((pos.x - cell_pos.x) * (pos.x - cell_pos.x) +
                             (pos.y - cell_pos.y) * (pos.y - cell_pos.y));

      if (dist > place_radius) {
        continue;
      }

      setParticle(Particle(ElementType::Empty, sf::Color(0, 0, 0)), sf::Vector2u(pos.x, pos.y));
    }
  }
}

void Simulation::addParticle(const Particle& particle, const sf::Vector2u& pos) {
  int index = pos.y * size.x + pos.x;

  if (index < front_grid.size() && front_grid[index].type == ElementType::Empty) {
    front_grid[index] = particle;
  }
}

void Simulation::setParticle(const Particle& particle, const sf::Vector2u& pos) {
  int index = pos.y * size.x + pos.x;

  if (index < front_grid.size()) {
    front_grid[index] = particle;
  }
}

void Simulation::updateParticles() {
  std::fill(back_grid.begin(), back_grid.end(), Particle());

  for (int y = size.y - 1; y >= 0; y--) {
    for (int x = 0; x < size.x; x++) {
      int index = y * size.x + x;

      switch (front_grid[index].type) {
        case ElementType::Empty:
          break;

        case ElementType::Sand:
          updateSand(x, y);
          break;

        case ElementType::Water:
          updateWater(x, y);
          break;

        default:
          break;
      }
    }
  }

  std::swap(front_grid, back_grid);
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

void Simulation::updateWater(int x, int y) {
  int index = y * size.x + x;

  if (y + 1 >= size.y) {
    back_grid[index] = front_grid[index];
    return;
  }

  int below_index = (y + 1) * size.x + x;
  if (back_grid[below_index].type == ElementType::Empty) {
    back_grid[below_index] = front_grid[index];
    return;
  }

  int below_left_index = (y + 1) * size.x + (x - 1);
  if (x - 1 >= 0 && back_grid[below_left_index].type == ElementType::Empty) {
    back_grid[below_left_index] = front_grid[index];
    return;
  }

  int below_right_index = (y + 1) * size.x + (x + 1);
  if (x + 1 < size.x && back_grid[below_right_index].type == ElementType::Empty) {
    back_grid[below_right_index] = front_grid[index];
    return;
  }

  int left_index = y * size.x + (x - 1);
  if (back_grid[left_index].type == ElementType::Empty) {
    back_grid[left_index] = front_grid[index];
    return;
  }

  int right_index = y * size.x + (x + 1);
  if (back_grid[right_index].type == ElementType::Empty) {
    back_grid[right_index] = front_grid[index];
    return;
  }

  back_grid[index] = front_grid[index];
}

void Simulation::drawParticles() {
  sf::VertexArray vertices(sf::PrimitiveType::Triangles, size.x * size.y * 6);

  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      int index = y * size.x + x;
      sf::Color& color = front_grid[index].color;

      int vertexIndex = index * 6;

      sf::Vector2f pos(x * cell_size.x, y * cell_size.y);

      vertices[vertexIndex + 0].position = pos;
      vertices[vertexIndex + 1].position = pos + sf::Vector2f(cell_size.x, 0);
      vertices[vertexIndex + 2].position = pos + sf::Vector2f(cell_size.x, cell_size.y);

      vertices[vertexIndex + 3].position = pos;
      vertices[vertexIndex + 4].position = pos + sf::Vector2f(cell_size.x, cell_size.y);
      vertices[vertexIndex + 5].position = pos + sf::Vector2f(0, cell_size.y);

      for (int i = 0; i < 6; i++) {
        vertices[vertexIndex + i].color = color;
      }
    }
  }

  window.draw(vertices);
}

void Simulation::calculatePlacePositions() {
  place_positions.clear();

  sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
  sf::Vector2u cell_pos(mouse_pos.x / cell_size.x, mouse_pos.y / cell_size.y);

  for (int x = -place_radius; x <= place_radius; x++) {
    for (int y = -place_radius; y <= place_radius; y++) {
      sf::Vector2i place_pos(cell_pos.x + x, cell_pos.y + y);

      if (place_pos.x < 0 || place_pos.x >= size.x || place_pos.y < 0 || place_pos.y >= size.y) {
        continue;
      }

      place_positions.push_back(place_pos);
    }
  }
}

void Simulation::showPlaceRadius() {
  int max_cells = (2 * place_radius + 1) * (2 * place_radius + 1);
  sf::VertexArray vertices(sf::PrimitiveType::Triangles, max_cells * 6);

  sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
  sf::Vector2u cell_pos(mouse_pos.x / cell_size.x, mouse_pos.y / cell_size.y);

  sf::Color color(128, 128, 128, 32);

  int currentVertex = 0;

  for (sf::Vector2i& pos : place_positions) {
    float dist = std::sqrt((pos.x - cell_pos.x) * (pos.x - cell_pos.x) +
                           (pos.y - cell_pos.y) * (pos.y - cell_pos.y));

    if (dist > place_radius) {
      continue;
    }

    sf::Vector2f pos_f(pos.x * cell_size.x, pos.y * cell_size.y);
    vertices[currentVertex + 0].position = pos_f;
    vertices[currentVertex + 1].position = pos_f + sf::Vector2f(cell_size.x, 0);
    vertices[currentVertex + 2].position = pos_f + sf::Vector2f(cell_size.x, cell_size.y);

    vertices[currentVertex + 3].position = pos_f;
    vertices[currentVertex + 4].position = pos_f + sf::Vector2f(cell_size.x, cell_size.y);
    vertices[currentVertex + 5].position = pos_f + sf::Vector2f(0, cell_size.y);

    for (int i = 0; i < 6; i++) {
      vertices[currentVertex + i].color = color;
    }

    currentVertex += 6;
  }

  window.draw(vertices);
}
