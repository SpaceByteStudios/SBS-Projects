#include <chrono>
#include <cmath>
#include <iostream>

#include "maze3d.hh"
#include "maze3d_generator.hh"
#include "maze3d_solver.hh"

sf::Vector3i maze_size = {10, 10, 10};

int main() {
  srand(time(nullptr));

  std::cout << "How big should the maze be (width height):" << std::endl;

  std::cin >> maze_size.x >> maze_size.y;

  int REPETITIONS;

  std::cout << "How many repetitions: ";
  std::cin >> REPETITIONS;

  Maze3D maze(maze_size);

  maze.start_cell = sf::Vector3i(0, 0, 0);
  maze.end_cell = sf::Vector3i(maze.grid_size.x - 1, maze.grid_size.y - 1, maze.grid_size.z - 1);

  double depth_first_average = 0.0;
  double breadth_first_average = 0.0;

  double dijkstra_average = 0.0;
  double astar_average = 0.0;

  for (int i = 0; i < REPETITIONS; i++) {
    generate_recursive_division_maze(maze);
    maze.remove_random_walls(std::sqrt(maze.grid_size.x * maze.grid_size.y));

    auto depth_first_start = std::chrono::high_resolution_clock::now();
    solve_depth_first_maze(maze);
    auto depth_first_end = std::chrono::high_resolution_clock::now();

    auto breadth_first_start = std::chrono::high_resolution_clock::now();
    solve_breadth_first_maze(maze);
    auto breadth_first_end = std::chrono::high_resolution_clock::now();

    auto dijkstra_start = std::chrono::high_resolution_clock::now();
    solve_dijkstra_maze(maze);
    auto dijkstra_end = std::chrono::high_resolution_clock::now();

    auto astar_start = std::chrono::high_resolution_clock::now();
    solve_astar_maze(maze);
    auto astar_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> depth_first_duration_ms = depth_first_end - depth_first_start;
    std::chrono::duration<double, std::milli> breadth_first_duration_ms = breadth_first_end - breadth_first_start;
    std::chrono::duration<double, std::milli> dijkstra_duration_ms = dijkstra_end - dijkstra_start;
    std::chrono::duration<double, std::milli> astar_duration_ms = astar_end - astar_start;

    depth_first_average += depth_first_duration_ms.count();
    breadth_first_average += breadth_first_duration_ms.count();
    dijkstra_average += dijkstra_duration_ms.count();
    astar_average += astar_duration_ms.count();
  }

  depth_first_average /= REPETITIONS;
  breadth_first_average /= REPETITIONS;
  dijkstra_average /= REPETITIONS;
  astar_average /= REPETITIONS;

  std::cout << "Depth First   average duration in ms: " << depth_first_average << std::endl;
  std::cout << "Breadth First average duration in ms: " << breadth_first_average << std::endl;
  std::cout << "Dijkstra      average duration in ms: " << dijkstra_average << std::endl;
  std::cout << "Astar         average duration in ms: " << astar_average << std::endl;
}
