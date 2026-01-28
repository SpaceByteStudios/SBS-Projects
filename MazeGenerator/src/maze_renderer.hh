#pragma once

#include "maze.hh"
#include <SFML/Graphics/RenderWindow.hpp>

struct MazeRenderer {
  MazeRenderer(sf::RenderWindow &window, Maze &maze);

  void draw_grid(Maze &maze);
  void draw_path(Maze &maze);

  sf::RenderWindow &window;
};
