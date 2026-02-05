#pragma once

#include <SFML/System/Vector3.hpp>
#include <vector>

struct Cell3D {
  //       Top,   Front, Right, Back,  Left,  Bottom
  // 6bit: 0: +y, 1: +x, 2: +z, 3: -x, 4: -z, 5: -y
  unsigned char walls_bitmap;
};

struct Wall3D {
  sf::Vector3i cell1;
  sf::Vector3i cell2;
};

struct Maze3D {
  Maze3D();
  Maze3D(sf::Vector3i new_grid_size);

  bool is_inside(const sf::Vector3i& pos);
  int index_at_pos(const sf::Vector3i& pos);

  sf::Vector3i pos_at_index(int index);

  std::vector<sf::Vector3i> get_neighbors(const sf::Vector3i& pos);
  bool are_neighbors(const sf::Vector3i& pos1, const sf::Vector3i& pos2);
  bool is_path_free(const sf::Vector3i& pos1, const sf::Vector3i& pos2);

  void set_wall(const sf::Vector3i& pos1, const sf::Vector3i& pos2);
  void remove_wall(const sf::Vector3i& pos1, const sf::Vector3i& pos2);

  void remove_random_walls(int amount);

  const sf::Vector3i grid_size;
  sf::Vector3f cell_size;

  sf::Vector3i start_cell;
  sf::Vector3i end_cell;

  // index: x + z * grid_size.x + y * grid_size.x * grid_size.z
  std::vector<Cell3D> grid;
  std::vector<sf::Vector3i> path;
};
