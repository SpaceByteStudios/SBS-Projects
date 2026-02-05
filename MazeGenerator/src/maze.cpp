#include "maze.hh"

#include <stdlib.h>

#include <SFML/System/Vector2.hpp>
#include <vector>

Maze::Maze() : grid_size(sf::Vector2u(5, 5)) {
  grid.resize(grid_size.x * grid_size.y);

  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(sf::Vector2u(x, y));

      grid[index].walls_bitmap = 15;
    }
  }
}

Maze::Maze(sf::Vector2u new_grid_size) : grid_size(new_grid_size) {
  grid.resize(grid_size.x * grid_size.y);

  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(sf::Vector2u(x, y));

      grid[index].walls_bitmap = 15;
    }
  }
}

bool Maze::is_inside(const sf::Vector2u& pos) const {
  if (pos.x < 0 || pos.x >= grid_size.x || pos.y < 0 || pos.y >= grid_size.y) {
    return false;
  }

  return true;
}

int Maze::index_at_pos(const sf::Vector2u& pos) const {
  if (is_inside(pos)) {
    return pos.y * grid_size.x + pos.x;
  } else {
    return -1;
  }
}

sf::Vector2u Maze::pos_at_index(int index) const {
  if (index > grid.size() - 1) {
    return sf::Vector2u(0, 0);
  } else {
    return sf::Vector2u(index % grid_size.x, index / grid_size.x);
  }
}

std::vector<sf::Vector2u> Maze::get_neighbors(const sf::Vector2u& pos) const {
  std::vector<sf::Vector2u> cells_pos = {
      {pos.x, pos.y - 1}, {pos.x, pos.y + 1}, {pos.x - 1, pos.y}, {pos.x + 1, pos.y}};

  std::vector<sf::Vector2u> neigbor_cells;

  for (int i = 0; i < 4; i++) {
    if (is_inside(cells_pos[i])) {
      neigbor_cells.push_back({cells_pos[i].x, cells_pos[i].y});
    }
  }

  return neigbor_cells;
}

bool Maze::are_neighbors(const sf::Vector2u& pos1, const sf::Vector2u& pos2) const {
  int index_cell1 = index_at_pos(pos1);
  int index_cell2 = index_at_pos(pos2);

  if (index_cell2 - 1 == index_cell1 || index_cell2 + 1 == index_cell1 || index_cell2 - grid_size.x == index_cell1 ||
      index_cell2 + grid_size.x == index_cell1) {
    return true;
  } else {
    return false;
  }
}

bool Maze::is_path_free(const sf::Vector2u& pos1, const sf::Vector2u& pos2) const {
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

  sf::Vector2i dir = static_cast<sf::Vector2i>(pos2 - pos1);

  const unsigned char& cell1_bitmap = grid[index_cell1].walls_bitmap;
  const unsigned char& cell2_bitmap = grid[index_cell2].walls_bitmap;

  if (dir == sf::Vector2i(0, -1)) {
    if (!(cell1_bitmap & (1 << 0)) && !(cell2_bitmap & (1 << 2))) {
      return true;
    }
  }

  if (dir == sf::Vector2i(1, 0)) {
    if (!(cell1_bitmap & (1 << 1)) && !(cell2_bitmap & (1 << 3))) {
      return true;
    }
  }

  if (dir == sf::Vector2i(0, 1)) {
    if (!(cell1_bitmap & (1 << 2)) && !(cell2_bitmap & (1 << 0))) {
      return true;
    }
  }

  if (dir == sf::Vector2i(-1, 0)) {
    if (!(cell1_bitmap & (1 << 3)) && !(cell2_bitmap & (1 << 1))) {
      return true;
    }
  }

  return false;
}

void Maze::set_wall(const sf::Vector2u& pos1, const sf::Vector2u& pos2) {
  int cell1_index = index_at_pos(pos1);
  int cell2_index = index_at_pos(pos2);

  sf::Vector2i dir = static_cast<sf::Vector2i>(pos2 - pos1);

  unsigned char& cell1_bitmap = grid[cell1_index].walls_bitmap;
  unsigned char& cell2_bitmap = grid[cell2_index].walls_bitmap;

  if (dir == sf::Vector2i(0, -1)) {
    cell1_bitmap |= (1 << 0);
    cell2_bitmap |= (1 << 2);
  }

  if (dir == sf::Vector2i(1, 0)) {
    cell1_bitmap |= (1 << 1);
    cell2_bitmap |= (1 << 3);
  }

  if (dir == sf::Vector2i(0, 1)) {
    cell1_bitmap |= (1 << 2);
    cell2_bitmap |= (1 << 0);
  }

  if (dir == sf::Vector2i(-1, 0)) {
    cell1_bitmap |= (1 << 3);
    cell2_bitmap |= (1 << 1);
  }
}

void Maze::remove_wall(const sf::Vector2u& pos1, const sf::Vector2u& pos2) {
  int cell1_index = index_at_pos(pos1);
  int cell2_index = index_at_pos(pos2);

  sf::Vector2i dir = static_cast<sf::Vector2i>(pos2 - pos1);

  unsigned char& cell1_bitmap = grid[cell1_index].walls_bitmap;
  unsigned char& cell2_bitmap = grid[cell2_index].walls_bitmap;

  if (dir == sf::Vector2i(0, -1)) {
    cell1_bitmap &= ~(1 << 0);
    cell2_bitmap &= ~(1 << 2);
  }

  if (dir == sf::Vector2i(1, 0)) {
    cell1_bitmap &= ~(1 << 1);
    cell2_bitmap &= ~(1 << 3);
  }

  if (dir == sf::Vector2i(0, 1)) {
    cell1_bitmap &= ~(1 << 2);
    cell2_bitmap &= ~(1 << 0);
  }

  if (dir == sf::Vector2i(-1, 0)) {
    cell1_bitmap &= ~(1 << 3);
    cell2_bitmap &= ~(1 << 1);
  }
}

void Maze::remove_random_walls(int amount) {
  const int MAX_TRIES = 100;

  for (int i = 0; i < amount; i++) {
    for (int t = 0; t < MAX_TRIES; t++) {
      if (t > MAX_TRIES) {
        return;
      }

      sf::Vector2u rand_cell = {rand() % grid_size.x, rand() % grid_size.y};

      std::vector<sf::Vector2u> next_cells = get_neighbors(rand_cell);
      sf::Vector2u rand_next_cell = next_cells[rand() % next_cells.size()];

      if (!is_path_free(rand_cell, rand_next_cell)) {
        remove_wall(rand_cell, rand_next_cell);
        break;
      }

      t += 1;
    }
  }

  return;
}
