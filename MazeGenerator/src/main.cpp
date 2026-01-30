#include <SFML/Graphics.hpp>

#include "maze.hh"
#include "maze_generator.hh"
#include "maze_renderer.hh"
#include "maze_solver.hh"

sf::Vector2u maze_size = {20, 20};

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(180);

  Maze maze(maze_size);
  MazeRenderer renderer(window, maze);

  maze.start_cell = sf::Vector2u(0, 0);
  maze.end_cell = sf::Vector2u(maze.grid_size.x - 1, maze.grid_size.y - 1);

  generate_recursive_division_maze(maze);
  solve_breadth_first_maze(maze);

  window.clear();

  renderer.draw_grid(maze);
  renderer.draw_path(maze);

  window.display();

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
          generate_recursive_division_maze(maze);
          solve_breadth_first_maze(maze);

          window.clear();

          renderer.draw_grid(maze);
          renderer.draw_path(maze);

          window.display();
        }
      }
    }

    sf::sleep(sf::milliseconds(16));
  }
}
