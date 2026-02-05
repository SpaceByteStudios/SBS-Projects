#include <SFML/System/Vector3.hpp>

#include "maze3d.hh"

sf::Vector3i maze_size{4, 4, 4};

int main() {
  Maze3D maze(maze_size);

  return 0;
}
