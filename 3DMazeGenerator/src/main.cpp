#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

sf::Vector3i maze_size{4, 4, 4};
sf::Vector3f cells_size{1.0f, 1.0f, 1.0f};

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(0);

  Maze3D maze(maze_size);
  Maze3DRenderer renderer(window, maze, cells_size);

  maze.start_cell = sf::Vector3i(0, 0, 0);
  maze.end_cell = sf::Vector3i(maze.grid_size.x - 1, maze.grid_size.y - 1, maze.grid_size.z - 1);

  window.clear();

  renderer.draw_grid(maze);

  window.display();

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
          window.clear();

          window.display();
        }
      }
    }

    sf::sleep(sf::milliseconds(16));
  }
}
