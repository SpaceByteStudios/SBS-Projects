#include <SFML/System/Vector3.hpp>
#include <cassert>
#include <vector>

#include "maze3d.hh"

sf::Vector3i maze_size{3, 3, 3};

int main() {
  Maze3D maze(maze_size);

  // Test is_inside
  assert(maze.is_inside({2, 2, 1}));
  assert(!maze.is_inside({3, 2, 1}));

  // Test index_at_pos
  assert(maze.index_at_pos({2, 2, 2}) == maze.grid.size() - 1);
  assert(maze.index_at_pos({0, 2, 1}) == 21);

  // Test pos_at_index
  sf::Vector3i pos = maze.pos_at_index(11);
  assert(pos.x == 2 && pos.y == 1 && pos.z == 0);

  sf::Vector3i index_pos = maze.pos_at_index(22);
  assert(maze.index_at_pos(index_pos) == 22);

  // Test get_neighbors
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

  // Test are_neighbors
  std::vector<sf::Vector3i> dir = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

  for (sf::Vector3i& d : dir) {
    assert(maze.are_neighbors({1, 1, 1}, sf::Vector3i{1, 1, 1} + d));
  }

  // Test are not neighbors
  assert(!maze.are_neighbors({1, 1, 1}, {0, 0, 0}));

  // remove all walls
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos({x, y, z});
        maze.grid[index].walls_bitmap = 0;
      }
    }
  }

  // Test set_wall: Left, Front, Top
  maze.set_wall({1, 1, 1}, sf::Vector3i{0, 1, 1});
  maze.set_wall({1, 1, 1}, sf::Vector3i{1, 0, 1});
  maze.set_wall({1, 1, 1}, sf::Vector3i{1, 1, 0});

  const unsigned char& bitmap1 = maze.grid[maze.index_at_pos({1, 1, 1})].walls_bitmap;

  // Top, Front, Left Wall
  assert(bitmap1 & (1 << 0));
  assert(bitmap1 & (1 << 1));
  assert(bitmap1 & (1 << 4));

  // Back, Right and Bottom Wall
  assert(!(bitmap1 & (1 << 3)));
  assert(!(bitmap1 & (1 << 2)));
  assert(!(bitmap1 & (1 << 5)));

  // Test remove_wall: Left, Front, Top
  maze.remove_wall({1, 1, 1}, sf::Vector3i{0, 1, 1});
  maze.remove_wall({1, 1, 1}, sf::Vector3i{1, 0, 1});
  maze.remove_wall({1, 1, 1}, sf::Vector3i{1, 1, 0});

  // Top, Front, Left Wall
  assert(!(bitmap1 & (1 << 0)));
  assert(!(bitmap1 & (1 << 1)));
  assert(!(bitmap1 & (1 << 4)));

  maze.set_wall({1, 1, 1}, sf::Vector3i{0, 1, 1});
  assert(!(maze.is_path_free({1, 1, 1}, {0, 1, 1})));

  maze.remove_wall({1, 1, 1}, sf::Vector3i{0, 1, 1});
  assert(maze.is_path_free({1, 1, 1}, {0, 1, 1}));

  return 0;
}
