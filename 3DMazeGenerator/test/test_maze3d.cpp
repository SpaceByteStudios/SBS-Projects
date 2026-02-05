#include <SFML/System/Vector3.hpp>
#include <cassert>
#include <vector>

#include "maze3d.hh"

sf::Vector3i maze_size{3, 3, 3};

int main() {
  Maze3D maze(maze_size);

  assert(maze.is_inside({2, 2, 1}));
  assert(!maze.is_inside({3, 2, 1}));

  assert(maze.index_at_pos({2, 2, 2}) == maze.grid.size() - 1);
  assert(maze.index_at_pos({0, 2, 1}) == 21);

  sf::Vector3i pos = maze.pos_at_index(11);
  assert(pos.x == 2 && pos.y == 1 && pos.z == 0);

  sf::Vector3i index_pos = maze.pos_at_index(22);
  assert(maze.index_at_pos(index_pos) == 22);

  std::vector<sf::Vector3i> next_cells = maze.get_neighbors({1, 1, 1});
  std::vector<sf::Vector3i> cells = {{0, 1, 1}, {2, 1, 1}, {1, 0, 1}, {1, 2, 1}, {1, 1, 0}, {1, 1, 2}};

  for (sf::Vector3i& n : next_cells) {
    bool exists = false;

    for (sf::Vector3i& c : cells) {
      if (n == c) {
        exists = true;
      }
    }

    assert(exists);
  }

  std::vector<sf::Vector3i> dir = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

  for (sf::Vector3i& d : dir) {
    assert(maze.are_neighbors({1, 1, 1}, sf::Vector3i{1, 1, 1} + d));
  }

  assert(!maze.are_neighbors({1, 1, 1}, {0, 0, 0}));

  return 0;
}
