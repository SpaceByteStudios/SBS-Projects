#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>

#include "maze.hh"
#include "maze_generator.hh"
#include "maze_renderer.hh"
#include "maze_solver.hh"

// sf::Vector2u maze_size = {96, 54};
sf::Vector2u maze_size = {5, 5};

const int remove_walls_amount = 50;
std::vector<sf::Vector2u> rand_cells(remove_walls_amount * 2);

void choose_random_walls(Maze& maze) {
  const int MAX_TRIES = 100;

  for (int i = 0; i < remove_walls_amount; i++) {
    for (int t = 0; t < MAX_TRIES; t++) {
      if (t > MAX_TRIES) {
        break;
      }

      sf::Vector2u rand_cell = {rand() % maze.grid_size.x, rand() % maze.grid_size.y};

      std::vector<sf::Vector2u> next_cells = maze.get_neighbors(rand_cell);
      sf::Vector2u rand_next_cell = next_cells[rand() % next_cells.size()];

      if (!maze.is_path_free(rand_cell, rand_next_cell)) {
        rand_cells[2 * i] = rand_cell;
        rand_cells[2 * i + 1] = rand_next_cell;
        break;
      }

      t += 1;
    }
  }
}

void remove_walls(Maze& maze) {
  for (int i = 0; i < remove_walls_amount; i++) {
    maze.remove_wall(rand_cells[2 * i], rand_cells[2 * i + 1]);
  }
}

void set_walls(Maze& maze) {
  for (int i = 0; i < remove_walls_amount; i++) {
    maze.set_wall(rand_cells[2 * i], rand_cells[2 * i + 1]);
  }
}

int main() {
  srand(time(nullptr));

  // sf::RenderWindow window(sf::VideoMode({1280, 720}), "Maze Generator");
  sf::RenderWindow window(sf::VideoMode({720, 720}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(60);

  Maze maze(maze_size);
  Maze maze2(maze_size);
  MazeRenderer renderer(window, maze);
  MazeRenderer renderer2(window, maze2);

  sf::Color grid_color = sf::Color(255, 255, 255);
  sf::Color path_color = sf::Color(64, 255, 64);
  sf::Color path_color2 = sf::Color(255, 64, 64);

  renderer.set_color(grid_color, path_color);
  renderer2.set_color(grid_color, path_color2);

  maze.start_cell = sf::Vector2u(0, 0);
  maze.end_cell = sf::Vector2u(maze.grid_size.x - 1, maze.grid_size.y - 1);
  maze2.start_cell = maze.start_cell;
  maze2.end_cell = maze.end_cell;

  // generate_depth_first_maze(maze);
  // solve_breadth_first_maze(maze);

  window.clear();

  renderer.draw_grid(maze);
  // renderer.draw_path(maze);

  window.display();

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
          generate_depth_first_maze(maze);

          window.clear();

          renderer.draw_grid(maze);
          // renderer.draw_path(maze);

          window.display();
        }
      }
    }

    sf::sleep(sf::milliseconds(16));
  }
}
