#include "maze3d.hh"

#include <stdlib.h>

#include <SFML/System/Vector3.hpp>
#include <vector>

Maze3D::Maze3D() : grid_size(sf::Vector3i(3, 3, 3)) {
  grid.resize(grid_size.x * grid_size.y * grid_size.z);

  for (int y = 0; y < grid_size.y; y++) {
    for (int z = 0; z < grid_size.z; z++) {
      for (int x = 0; x < grid_size.x; x++) {
        int index = index_at_pos(sf::Vector3i(x, y, z));

        grid[index].walls_bitmap = 15;
      }
    }
  }
}

Maze3D::Maze3D(sf::Vector3i new_grid_size) : grid_size(new_grid_size) {
  grid.resize(grid_size.x * grid_size.y * grid_size.z);

  for (int y = 0; y < grid_size.y; y++) {
    for (int z = 0; z < grid_size.z; z++) {
      for (int x = 0; x < grid_size.x; x++) {
        int index = index_at_pos(sf::Vector3i(x, y, z));

        grid[index].walls_bitmap = 15;
      }
    }
  }
}

bool Maze3D::is_inside(const sf::Vector3i& pos) const {
  if (pos.x < 0 || pos.x >= grid_size.x || pos.y < 0 || pos.y >= grid_size.y || pos.z < 0 || pos.z >= grid_size.z) {
    return false;
  }

  return true;
}

int Maze3D::index_at_pos(const sf::Vector3i& pos) const {
  if (is_inside(pos)) {
    return pos.x + pos.z * grid_size.x + pos.y * grid_size.x * grid_size.z;
  } else {
    return -1;
  }
}

sf::Vector3i Maze3D::pos_at_index(int index) {
  if (index > grid.size() - 1) {
    return sf::Vector3i(0, 0, 0);
  } else {
    int pos_y = index / (grid_size.x * grid_size.z);
    int remain = index % (grid_size.x * grid_size.z);

    int pos_x = remain % grid_size.x;
    int pos_z = remain / grid_size.x;
    return sf::Vector3i(pos_x, pos_y, pos_z);
  }
}

std::vector<sf::Vector3i> Maze3D::get_neighbors(const sf::Vector3i& pos) {
  std::vector<sf::Vector3i> cells_pos = {{pos.x, pos.y - 1, pos.z}, {pos.x, pos.y + 1, pos.z},
                                         {pos.x - 1, pos.y, pos.z}, {pos.x + 1, pos.y, pos.z},
                                         {pos.x, pos.y, pos.z - 1}, {pos.x, pos.y, pos.z + 1}};

  std::vector<sf::Vector3i> neigbor_cells;

  for (int i = 0; i < cells_pos.size(); i++) {
    if (is_inside(cells_pos[i])) {
      neigbor_cells.push_back({cells_pos[i].x, cells_pos[i].y, cells_pos[i].z});
    }
  }

  return neigbor_cells;
}

bool Maze3D::are_neighbors(const sf::Vector3i& pos1, const sf::Vector3i& pos2) {
  int index_cell1 = index_at_pos(pos1);
  int index_cell2 = index_at_pos(pos2);

  if (index_cell2 - 1 == index_cell1 || index_cell2 + 1 == index_cell1 || index_cell2 - grid_size.x == index_cell1 ||
      index_cell2 + grid_size.x == index_cell1 || index_cell2 + grid_size.x * grid_size.z == index_cell1 ||
      index_cell2 - grid_size.x * grid_size.z == index_cell1) {
    return true;
  } else {
    return false;
  }
}

bool Maze3D::is_path_free(const sf::Vector3i& pos1, const sf::Vector3i& pos2) {
  if (!is_inside(pos1)) {
    return false;
  }

  if (!is_inside(pos2)) {
    return false;
  }

  if (!are_neighbors(pos1, pos2)) {
    return false;
  }

  int index_cell1 = index_at_pos(pos1);
  int index_cell2 = index_at_pos(pos2);

  sf::Vector3i dir = static_cast<sf::Vector3i>(pos2 - pos1);

  unsigned char& cell1_bitmap = grid[index_cell1].walls_bitmap;
  unsigned char& cell2_bitmap = grid[index_cell2].walls_bitmap;

  if (dir == sf::Vector3i(-1, 0, 0)) {
    if (!(cell1_bitmap & (1 << 4)) && !(cell2_bitmap & (1 << 2))) {
      return true;
    }
  }

  if (dir == sf::Vector3i(1, 0, 0)) {
    if (!(cell1_bitmap & (1 << 2)) && !(cell2_bitmap & (1 << 4))) {
      return true;
    }
  }

  if (dir == sf::Vector3i(0, -1, 0)) {
    if (!(cell1_bitmap & (1 << 0)) && !(cell2_bitmap & (1 << 5))) {
      return true;
    }
  }

  if (dir == sf::Vector3i(0, 1, 0)) {
    if (!(cell1_bitmap & (1 << 5)) && !(cell2_bitmap & (1 << 0))) {
      return true;
    }
  }

  if (dir == sf::Vector3i(0, 0, -1)) {
    if (!(cell1_bitmap & (1 << 1)) && !(cell2_bitmap & (1 << 3))) {
      return true;
    }
  }

  if (dir == sf::Vector3i(0, 0, 1)) {
    if (!(cell1_bitmap & (1 << 3)) && !(cell2_bitmap & (1 << 1))) {
      return true;
    }
  }

  return false;
}

void Maze3D::set_wall(const sf::Vector3i& pos1, const sf::Vector3i& pos2) {
  //       Top,   Front, Right, Back,  Left,  Bottom
  // 6bit: 0: +y, 1: +x, 2: +z, 3: -x, 4: -z, 5: -y

  int cell1_index = index_at_pos(pos1);
  int cell2_index = index_at_pos(pos2);

  sf::Vector3i dir = static_cast<sf::Vector3i>(pos2 - pos1);

  unsigned char& cell1_bitmap = grid[cell1_index].walls_bitmap;
  unsigned char& cell2_bitmap = grid[cell2_index].walls_bitmap;

  if (dir == sf::Vector3i(-1, 0, 0)) {
    cell1_bitmap |= (1 << 4);
    cell2_bitmap |= (1 << 2);
  }

  if (dir == sf::Vector3i(1, 0, 0)) {
    cell1_bitmap |= (1 << 2);
    cell2_bitmap |= (1 << 4);
  }

  if (dir == sf::Vector3i(0, -1, 0)) {
    cell1_bitmap |= (1 << 0);
    cell2_bitmap |= (1 << 5);
  }

  if (dir == sf::Vector3i(0, 1, 0)) {
    cell1_bitmap |= (1 << 5);
    cell2_bitmap |= (1 << 0);
  }

  if (dir == sf::Vector3i(0, 0, -1)) {
    cell1_bitmap |= (1 << 1);
    cell2_bitmap |= (1 << 3);
  }

  if (dir == sf::Vector3i(0, 0, 1)) {
    cell1_bitmap |= (1 << 3);
    cell2_bitmap |= (1 << 1);
  }
}

void Maze3D::remove_wall(const sf::Vector3i& pos1, const sf::Vector3i& pos2) {
  // Top, Front, Right, Back, Left, Bottom
  // 6bit: 1: +y, 2: +x, 3: +z, 4: -x, 5: -z, 6: -y

  int cell1_index = index_at_pos(pos1);
  int cell2_index = index_at_pos(pos2);

  sf::Vector3i dir = static_cast<sf::Vector3i>(pos2 - pos1);

  unsigned char& cell1_bitmap = grid[cell1_index].walls_bitmap;
  unsigned char& cell2_bitmap = grid[cell2_index].walls_bitmap;

  if (dir == sf::Vector3i(-1, 0, 0)) {
    cell1_bitmap &= ~(1 << 4);
    cell2_bitmap &= ~(1 << 2);
  }

  if (dir == sf::Vector3i(1, 0, 0)) {
    cell1_bitmap &= ~(1 << 2);
    cell2_bitmap &= ~(1 << 4);
  }

  if (dir == sf::Vector3i(0, -1, 0)) {
    cell1_bitmap &= ~(1 << 0);
    cell2_bitmap &= ~(1 << 5);
  }

  if (dir == sf::Vector3i(0, 1, 0)) {
    cell1_bitmap &= ~(1 << 5);
    cell2_bitmap &= ~(1 << 0);
  }

  if (dir == sf::Vector3i(0, 0, -1)) {
    cell1_bitmap &= ~(1 << 1);
    cell2_bitmap &= ~(1 << 3);
  }

  if (dir == sf::Vector3i(0, 0, 1)) {
    cell1_bitmap &= ~(1 << 3);
    cell2_bitmap &= ~(1 << 1);
  }
}

void Maze3D::remove_random_walls(int amount) {
  // should work when remove_wall implemented
  const int MAX_TRIES = 100;

  for (int i = 0; i < amount; i++) {
    for (int t = 0; t < MAX_TRIES; t++) {
      if (t > MAX_TRIES) {
        return;
      }

      sf::Vector3i rand_cell = {rand() % grid_size.x, rand() % grid_size.y, rand() % grid_size.z};

      std::vector<sf::Vector3i> next_cells = get_neighbors(rand_cell);
      sf::Vector3i rand_next_cell = next_cells[rand() % next_cells.size()];

      if (!is_path_free(rand_cell, rand_next_cell)) {
        remove_wall(rand_cell, rand_next_cell);
        break;
      }

      t += 1;
    }
  }
}
