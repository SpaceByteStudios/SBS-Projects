#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "elementType.hh"
#include "particle.hh"

class Simulation {
 public:
  Simulation();
  Simulation(int width, int height);

  void run();
  void draw();
  void processInput();

  void addParticle(const Particle& particle, const sf::Vector2u& pos);
  void setParticle(const Particle& particle, const sf::Vector2u& pos);

  bool isRunning;

 private:
  sf::Vector2u size;
  sf::Vector2f cell_size;

  std::vector<Particle> front_grid;
  std::vector<Particle> back_grid;

  sf::RenderWindow window;

  ElementType place_type = ElementType::Sand;
  int place_radius = 3;

  void updateSand(int x, int y);

  void showPlaceRadius();
};
