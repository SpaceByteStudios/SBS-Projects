#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <chrono>
#include <iostream>
#include <ratio>

#include "maze.hh"
#include "maze_generator.hh"
#include "maze_renderer.hh"
#include "maze_solver.hh"

sf::Vector2u maze_size = {100, 100};

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");
  window.setPosition({1920 - (int)window.getSize().x - 25, 25});

  window.setFramerateLimit(0);

  Maze maze(maze_size);
  Maze maze2(maze_size);
  MazeRenderer renderer(window, maze);
  MazeRenderer renderer2(window, maze2);

  sf::Color grid_color = sf::Color(255, 255, 255);
  sf::Color path_color = sf::Color(64, 255, 64);
  sf::Color path2_color = sf::Color(255, 64, 64);

  renderer.set_color(grid_color, path_color);
  renderer2.set_color(grid_color, path2_color);

  maze.start_cell = sf::Vector2u(0, 0);
  maze.end_cell = sf::Vector2u(maze.grid_size.x - 1, maze.grid_size.y - 1);

  maze2.start_cell = maze.start_cell;
  maze2.end_cell = maze.end_cell;

  // generate_depth_first_maze(maze);
  // maze.remove_random_walls(50);
  // maze2.grid = maze.grid;

  const int REPETITIONS = 100;

  double dijkstra_average = 0.0;
  double astar_average = 0.0;

  for (int i = 0; i < REPETITIONS; i++) {
    generate_depth_first_maze(maze);
    maze.remove_random_walls(maze.grid_size.x);
    maze2.grid = maze.grid;

    auto dijkstra_start = std::chrono::high_resolution_clock::now();
    solve_dijkstra_maze(maze);
    auto dijkstra_end = std::chrono::high_resolution_clock::now();

    auto astar_start = std::chrono::high_resolution_clock::now();
    solve_astar_maze(maze2);
    auto astar_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> dijkstra_duration_ms = dijkstra_end - dijkstra_start;
    std::chrono::duration<double, std::milli> astar_duration_ms = astar_end - astar_start;

    dijkstra_average += dijkstra_duration_ms.count();
    astar_average += astar_duration_ms.count();

    window.clear();

    renderer.draw_grid(maze);
    renderer2.draw_path(maze2);
    renderer.draw_path(maze);

    window.display();
  }

  dijkstra_average /= REPETITIONS;
  astar_average /= REPETITIONS;

  std::cout << "Dijkstra average duration in ms: " << dijkstra_average << std::endl;
  std::cout << "Astar average duration in ms:    " << astar_average << std::endl;

  window.clear();

  renderer.draw_grid(maze);
  renderer2.draw_path(maze2);
  renderer.draw_path(maze);

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
          maze.remove_random_walls(50);
          maze2.grid = maze.grid;

          auto dijkstra_start = std::chrono::high_resolution_clock::now();
          solve_dijkstra_maze(maze);
          auto dijkstra_end = std::chrono::high_resolution_clock::now();

          auto astar_start = std::chrono::high_resolution_clock::now();
          solve_astar_maze(maze2);
          auto astar_end = std::chrono::high_resolution_clock::now();

          std::chrono::duration<double, std::milli> dijkstra_duration_ms = dijkstra_end - dijkstra_start;
          std::chrono::duration<double, std::milli> astar_duration_ms = astar_end - astar_start;

          std::cout << "Dijkstra duration in ms: " << dijkstra_duration_ms << std::endl;
          std::cout << "Astar duration in ms: " << astar_duration_ms << std::endl;

          window.clear();

          renderer.draw_grid(maze);
          renderer2.draw_path(maze2);
          renderer.draw_path(maze);

          window.display();
        }
      }
    }

    sf::sleep(sf::milliseconds(16));
  }
}
