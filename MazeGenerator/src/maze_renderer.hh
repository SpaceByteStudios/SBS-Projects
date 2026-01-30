#pragma once

#include "maze.hh"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct MazeRenderer {
  MazeRenderer(sf::RenderWindow &window, Maze &maze);

  void draw_grid(Maze &maze);
  void draw_path(Maze &maze);

  void set_color(sf::Color &new_grid_color, sf::Color &new_path_color);

  sf::RenderWindow &window;

  sf::Color grid_color;
  sf::Color path_color;
};
