#include <chrono>
#include <iostream>

#include "maze.hh"
#include "maze_generator.hh"
#include "maze_solver.hh"

sf::Vector2u maze_size = {10, 10};

int main() {
  srand(time(nullptr));

  std::cout << "How big should the maze be (width height):" << std::endl;

  std::cin >> maze_size.x >> maze_size.y;

  int REPETITIONS;

  std::cout << "How many repetitions: ";
  std::cin >> REPETITIONS;

  Maze maze(maze_size);

  maze.start_cell = sf::Vector2u(0, 0);
  maze.end_cell = sf::Vector2u(maze.grid_size.x - 1, maze.grid_size.y - 1);

  double depth_first_average = 0.0;
  double prim_average = 0.0;

  double kruskal_average = 0.0;
  double recursive_division_average = 0.0;

  for (int i = 0; i < REPETITIONS; i++) {
    auto depth_first_start = std::chrono::high_resolution_clock::now();
    generate_depth_first_maze(maze);
    auto depth_first_end = std::chrono::high_resolution_clock::now();

    auto prim_start = std::chrono::high_resolution_clock::now();
    generate_prim_maze(maze);
    auto prim_end = std::chrono::high_resolution_clock::now();

    auto kruskal_start = std::chrono::high_resolution_clock::now();
    generate_kruskal_maze(maze);
    auto kruskal_end = std::chrono::high_resolution_clock::now();

    auto recursive_division_start = std::chrono::high_resolution_clock::now();
    generate_recursive_division_maze(maze);
    auto recursive_division_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> depth_first_duration = depth_first_end - depth_first_start;
    std::chrono::duration<double, std::milli> prim_duration = prim_end - prim_start;
    std::chrono::duration<double, std::milli> kruskal_duration = kruskal_end - kruskal_start;
    std::chrono::duration<double, std::milli> recursive_division_duration =
        recursive_division_end - recursive_division_start;

    depth_first_average += depth_first_duration.count();
    prim_average += prim_duration.count();
    kruskal_average += kruskal_duration.count();
    recursive_division_average += recursive_division_duration.count();
  }

  depth_first_average /= REPETITIONS;
  prim_average /= REPETITIONS;
  kruskal_average /= REPETITIONS;
  recursive_division_average /= REPETITIONS;

  std::cout << "Depth First        average duration in ms: " << depth_first_average << std::endl;
  std::cout << "Prim               average duration in ms: " << prim_average << std::endl;
  std::cout << "Kruskal            average duration in ms: " << kruskal_average << std::endl;
  std::cout << "Recursive Division average duration in ms: " << recursive_division_average << std::endl;
}
