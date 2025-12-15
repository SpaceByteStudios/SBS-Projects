#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
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

  std::vector<Particle> grid;

  sf::RenderWindow window;

  std::vector<sf::Vector2i> place_positions;
  ElementType place_type = ElementType::Sand;
  float place_radius = 2.0;

  std::mt19937 rng;

  void updateParticles();
  void drawParticles();
  void updateSand(int x, int y);
  void updateWater(int x, int y);
  void updateStone(int x, int y);
  void updateWood(int x, int y);
  void updateFire(int x, int y);
  void updateOil(int x, int y);
  void updateSteam(int x, int y);
  void updateAcid(int x, int y);
  void updateLava(int x, int y);

  void calculatePlacePositions();
  void showPlaceRadius();

  bool is_inside(int x, int y);
  int pos_index(int x, int y);
};
