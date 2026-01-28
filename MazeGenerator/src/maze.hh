#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

struct Cell {
  unsigned char walls_bitmap;
};

struct Maze {
  Maze();
  Maze(sf::Vector2u new_grid_size);

  bool is_inside(sf::Vector2u pos);
  int index_at_pos(sf::Vector2u pos);
  std::vector<sf::Vector2u> get_neighbors(sf::Vector2u pos);
  bool are_neighbors(sf::Vector2u pos1, sf::Vector2u pos2);
  bool is_path_free(sf::Vector2u pos1, sf::Vector2u pos2);

  const sf::Vector2u grid_size;
  sf::Vector2f cell_size;

  sf::Vector2u start_cell;
  sf::Vector2u end_cell;

  std::vector<Cell> grid;
  std::vector<sf::Vector2u> path;
};
