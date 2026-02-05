#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "maze3d.hh"

struct Maze3DRenderer {
  Maze3DRenderer(sf::RenderWindow& window, Maze3D& maze, sf::Vector3f& cells_size);

  void draw_grid(Maze3D& maze);
  void draw_path(Maze3D& maze);

  void set_color(sf::Color& new_grid_color, sf::Color& new_path_color);

  sf::RenderWindow& window;

  sf::Color grid_color;
  sf::Color path_color;
};
