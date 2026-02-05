#pragma once

#include <SFML/System/Vector3.hpp>
#include <vector>

struct Cell3D {
  // 6bit: 1: +y, 2: +x, 3: +z, 4: -x, 5: -z, 6: -y
  unsigned char walls_bitmap;
};

struct Wall3D {
  sf::Vector3i cell1;
  sf::Vector3i cell2;
};

struct Maze3D {
  Maze3D();
  Maze3D(sf::Vector3i new_grid_size);

  bool is_inside(sf::Vector3i pos);
  int index_at_pos(sf::Vector3i pos);

  // Needs Implementation
  // sf::Vector3i pos_at_index(int index);

  std::vector<sf::Vector3i> get_neighbors(sf::Vector3i pos);
  bool are_neighbors(sf::Vector3i pos1, sf::Vector3i pos2);
  bool is_path_free(sf::Vector3i pos1, sf::Vector3i pos2);

  void set_wall(sf::Vector3i pos1, sf::Vector3i pos2);
  void remove_wall(sf::Vector3i pos1, sf::Vector3i pos2);

  void remove_random_walls(int amount);

  const sf::Vector3i grid_size;
  sf::Vector3f cell_size;

  sf::Vector3i start_cell;
  sf::Vector3i end_cell;

  // index: x + z * grid_size.x + y * grid_size.x * grid_size.z
  std::vector<Cell3D> grid;
  std::vector<sf::Vector3i> path;
};
