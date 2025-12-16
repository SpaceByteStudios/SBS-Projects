#include "simulation.hh"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <utility>
#include <vector>

#include "elementType.hh"
#include "particle.hh"

Simulation::Simulation() : rng(std::random_device{}()) {
  isRunning = true;
  is_paused = false;

  size = sf::Vector2u(10, 10);

  grid.resize(size.x * size.y);

  window = sf::RenderWindow(sf::VideoMode({640u, 360u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  sf::Vector2i window_pos = {1920 - (int)window.getSize().x - 50, 50};
  window.setPosition(window_pos);

  cell_size = sf::Vector2f(static_cast<float>(window.getSize().x) / size.x,
                           static_cast<float>(window.getSize().y) / size.y);

  std::srand(std::time(nullptr));
}

Simulation::Simulation(int width, int height) : rng(std::random_device{}()) {
  isRunning = true;
  is_paused = false;

  size = sf::Vector2u(width, height);

  grid.resize(size.x * size.y);

  window = sf::RenderWindow(sf::VideoMode({640u, 360u}), "Falling Sand Simulation");
  window.setFramerateLimit(60);
  sf::Vector2i window_pos = {1920 - (int)window.getSize().x - 50, 50};
  window.setPosition(window_pos);

  cell_size = sf::Vector2f(static_cast<float>(window.getSize().x) / size.x,
                           static_cast<float>(window.getSize().y) / size.y);
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

      if (key == sf::Keyboard::Key::Space) {
        is_paused = !is_paused;
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

      if (key == sf::Keyboard::Key::A) {
        place_type = ElementType::Water;
      }

      if (key == sf::Keyboard::Key::W) {
        place_type = ElementType::Wood;
      }

      if (key == sf::Keyboard::Key::Q) {
        place_type = ElementType::Stone;
      }

      if (key == sf::Keyboard::Key::F) {
        place_type = ElementType::Fire;
      }

      if (key == sf::Keyboard::Key::O) {
        place_type = ElementType::Oil;
      }

      if (key == sf::Keyboard::Key::I) {
        place_type = ElementType::Steam;
      }

      if (key == sf::Keyboard::Key::K) {
        place_type = ElementType::Acid;
      }

      if (key == sf::Keyboard::Key::L) {
        place_type = ElementType::Lava;
      }
    }

    if (event->is<sf::Event::MouseWheelScrolled>()) {
      auto scroll = event->getIf<sf::Event::MouseWheelScrolled>()->delta;

      if (scroll > 0) {
        place_radius += 0.5 + 0.05 * place_radius;
      } else if (scroll < 0) {
        place_radius -= 0.5 + 0.05 * place_radius;

        if (place_radius < 0) {
          place_radius = 0;
        }
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

      addParticle(Particle(place_type), sf::Vector2u(pos.x, pos.y));
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

      setParticle(Particle(ElementType::Empty), sf::Vector2u(pos.x, pos.y));
    }
  }
}

void Simulation::addParticle(const Particle& particle, const sf::Vector2u& pos) {
  int index = pos_index(pos.x, pos.y);

  if (index < grid.size() && grid[index].type == ElementType::Empty) {
    grid[index] = particle;
  }
}

void Simulation::setParticle(const Particle& particle, const sf::Vector2u& pos) {
  int index = pos_index(pos.x, pos.y);

  if (index < grid.size()) {
    grid[index] = particle;
  }
}

void Simulation::updateParticles() {
  if (is_paused) {
    return;
  }

  for (int i = 0; i < grid.size(); i++) {
    grid[i].hasUpdated = false;
  }

  for (int y = size.y - 1; y >= 0; y--) {
    int reverse = rand() % 2;

    std::vector<int> xOrder(size.x);
    for (int x = 0; x < size.x; x++) {
      xOrder[x] = x;
    }

    std::shuffle(xOrder.begin(), xOrder.end(), rng);

    for (int i = 0; i < size.x; i++) {
      int x = xOrder[i];

      int index = pos_index(x, y);

      if (grid[index].hasUpdated) {
        continue;
      }

      switch (grid[index].type) {
        case ElementType::Empty:
          grid[index].hasUpdated = true;
          break;

        case ElementType::Sand:
          updateSand(x, y);
          break;

        case ElementType::Water:
          updateWater(x, y);
          break;

        case ElementType::Steam:
          updateSteam(x, y);
          break;

        case ElementType::Stone:
          updateStone(x, y);
          break;

        case ElementType::Wood:
          updateWood(x, y);
          break;

        case ElementType::Fire:
          updateFire(x, y);
          break;

        case ElementType::Oil:
          updateOil(x, y);
          break;

        case ElementType::Acid:
          updateAcid(x, y);
          break;

        case ElementType::Lava:
          updateLava(x, y);
          break;
      }
    }
  }
}

void Simulation::updateSand(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> positions = {{x, y + 1}, {x - 1 * dir, y + 1}, {x + 1 * dir, y + 1}};

  for (sf::Vector2i pos : positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (getAttributes(grid[pos_i].type).is_fluid) {
        int flip_rate = rand() % 100;

        if (flip_rate < 75) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }
}

void Simulation::updateWater(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> positions = {
      {x, y + 1}, {x - 1 * dir, y + 1}, {x + 1 * dir, y + 1}, {x - 1 * dir, y}, {x + 1 * dir, y}};

  for (sf::Vector2i pos : positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (grid[pos_i].type == ElementType::Water) {
        if (rand() % 100 < 1) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }

      if (grid[pos_i].type == ElementType::Oil) {
        int flip_rate = 75;

        if (rand() % 100 < flip_rate) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }
}

void Simulation::updateStone(int x, int y) {
  int index = pos_index(x, y);
  grid[index].hasUpdated = true;
}

void Simulation::updateWood(int x, int y) {
  int index = pos_index(x, y);
  grid[index].hasUpdated = true;
}

void Simulation::updateFire(int x, int y) {
  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> positions = {{x, y + 1},     {x - 1, y + 1}, {x + 1, y + 1},
                                         {x - 1, y},     {x + 1, y},     {x, y - 1},
                                         {x - 1, y - 1}, {x + 1, y - 1}};

  for (sf::Vector2i pos : positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        int move_spread = rand() % 100;

        if (move_spread < 5) {
          std::swap(grid[index], grid[pos_i]);
        }
      }

      if (grid[pos_i].type == ElementType::Water) {
        int boil_rate = rand() % 100;

        if (boil_rate < 20) {
          grid[pos_i] = Particle(ElementType::Steam);
          grid[index] = Particle(ElementType::Empty);
        }
      }

      if (getAttributes(grid[pos_i].type).flammabilaty) {
        int fire_spread = rand() % 100;

        if (fire_spread < getAttributes(grid[pos_i].type).flammabilaty) {
          grid[pos_i] = Particle(ElementType::Fire);
        }
      }
    }
  }

  int life_rate = getAttributes(grid[index].type).decay;

  if (rand() % 100 < life_rate) {
    grid[index] = Particle(ElementType::Empty);
  }
}

void Simulation::updateOil(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> positions = {
      {x, y + 1}, {x - 1 * dir, y + 1}, {x + 1 * dir, y + 1}, {x - 1 * dir, y}, {x + 1 * dir, y}};

  for (sf::Vector2i pos : positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (grid[pos_i].type == ElementType::Oil) {
        if (rand() % 100 < 1) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }
}

void Simulation::updateSteam(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  int condense_rate = 1;

  if (rand() % 1000 < condense_rate) {
    grid[index] = Particle(ElementType::Water);
    return;
  }

  std::vector<sf::Vector2i> positions = {
      {x, y - 1}, {x - 1 * dir, y - 1}, {x + 1 * dir, y - 1}, {x - 1 * dir, y}, {x + 1 * dir, y}};

  for (sf::Vector2i pos : positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (getAttributes(grid[pos_i].type).is_fluid) {
        int flip_rate = 75;

        if (rand() % 100 < flip_rate) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }
}

void Simulation::updateAcid(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> move_positions = {
      {x, y + 1}, {x - 1 * dir, y + 1}, {x + 1 * dir, y + 1}, {x - 1 * dir, y}, {x + 1 * dir, y}};

  std::vector<sf::Vector2i> acid_positions = {{x, y + 1},     {x - 1, y + 1}, {x + 1, y + 1},
                                              {x - 1, y},     {x + 1, y},     {x, y - 1},
                                              {x - 1, y - 1}, {x + 1, y - 1}};

  for (sf::Vector2i& pos : move_positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (grid[pos_i].type == ElementType::Acid) {
        if (rand() % 100 < 1) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }

  for (sf::Vector2i& pos : acid_positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      ElementType grid_type = grid[pos_i].type;
      if (grid_type == ElementType::Acid || grid_type == ElementType::Empty) {
        continue;
      } else if (grid_type == ElementType::Stone) {
        int corrode_rate = 1;

        if (rand() % 1000 < corrode_rate) {
          grid[index] = Particle(ElementType::Empty);
          grid[pos_i] = Particle(ElementType::Empty);
        }
      } else {
        int corrode_rate = 1;

        if (rand() % 100 < corrode_rate) {
          grid[index] = Particle(ElementType::Empty);
          grid[pos_i] = Particle(ElementType::Empty);
        }
      }
    }
  }
}

void Simulation::updateLava(int x, int y) {
  int dir = rand() % 2 ? 1 : -1;

  int index = pos_index(x, y);
  grid[index].hasUpdated = true;

  std::vector<sf::Vector2i> move_positions = {
      {x, y + 1}, {x - 1 * dir, y + 1}, {x + 1 * dir, y + 1}, {x - 1 * dir, y}, {x + 1 * dir, y}};

  std::vector<sf::Vector2i> lava_positions = {{x, y + 1},     {x - 1, y + 1}, {x + 1, y + 1},
                                              {x - 1, y},     {x + 1, y},     {x, y - 1},
                                              {x - 1, y - 1}, {x + 1, y - 1}};

  for (sf::Vector2i pos : move_positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Empty) {
        std::swap(grid[index], grid[pos_i]);
        return;
      }

      if (grid[pos_i].type == ElementType::Lava) {
        if (rand() % 100 < 1) {
          std::swap(grid[index], grid[pos_i]);
          return;
        }
      }
    }
  }

  for (sf::Vector2i& pos : lava_positions) {
    int pos_i = pos_index(pos.x, pos.y);

    if (is_inside(pos.x, pos.y)) {
      if (grid[pos_i].type == ElementType::Water) {
        grid[pos_i] = Particle(ElementType::Steam);
        grid[index] = Particle(ElementType::Stone);
      }

      if (getAttributes(grid[pos_i].type).flammabilaty) {
        int fire_spread = rand() % 100;

        if (fire_spread < getAttributes(grid[pos_i].type).flammabilaty) {
          grid[pos_i] = Particle(ElementType::Fire);
        }
      }
    }
  }
}

void Simulation::drawParticles() {
  sf::VertexArray vertices(sf::PrimitiveType::Triangles, size.x * size.y * 6);

  for (int y = 0; y < size.y; y++) {
    for (int x = 0; x < size.x; x++) {
      int index = pos_index(x, y);

      sf::Color color = grid[index].color;

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

      if (!is_inside(place_pos.x, place_pos.y)) {
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

bool Simulation::is_inside(int x, int y) {
  if (x < 0 || x >= size.x || y < 0 || y >= size.y) {
    return false;
  }

  return true;
}

int Simulation::pos_index(int x, int y) {
  return y * size.x + x;
}
