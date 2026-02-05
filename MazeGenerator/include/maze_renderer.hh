#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "maze.hh"

struct MazeRenderer {
  MazeRenderer(sf::RenderWindow& window, Maze& maze);

  void draw_grid(const Maze& maze);
  void draw_path(const Maze& maze);

  void set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color);

  sf::RenderWindow& window;

  sf::Color grid_color;
  sf::Color path_color;
};
