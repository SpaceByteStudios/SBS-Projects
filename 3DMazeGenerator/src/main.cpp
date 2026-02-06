#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector3.hpp>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

sf::Vector3i maze_size{1, 1, 1};
sf::Vector3f cell_size{100.0f, 100.0f, 100.0f};

sf::Clock deltaClock;

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(60);

  Maze3D maze(maze_size);
  Maze3DRenderer renderer(window);

  sf::Color grid_color = sf::Color(255, 255, 255);
  sf::Color path_color = sf::Color(64, 255, 64);

  renderer.set_color(grid_color, path_color);

  maze.start_cell = sf::Vector3i(0, 0, 0);
  maze.end_cell = sf::Vector3i(maze.grid_size.x - 1, maze.grid_size.y - 1, maze.grid_size.z - 1);
  maze.cell_size = cell_size;

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos({x, y, z});
        maze.grid[index].walls_bitmap = 63;
      }
    }
  }

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        }
      }
    }

    float delta_time = deltaClock.restart().asSeconds();

    renderer.rotation.x += (3.14f / 16.0f) * delta_time;

    window.clear();

    renderer.draw_grid(maze);

    window.display();
  }
}
