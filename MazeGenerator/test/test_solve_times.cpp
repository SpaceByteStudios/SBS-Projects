#include <chrono>
#include <ctime>
#include <iostream>
#include <ratio>

#include "maze.hh"
#include "maze_generator.hh"
#include "maze_solver.hh"

sf::Vector2u maze_size = {100, 100};

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

  double random_walk_average = 0.0;

  double wall_follower_left_average = 0.0;
  double wall_follower_right_average = 0.0;

  double depth_first_average = 0.0;
  double breadth_first_average = 0.0;

  double dijkstra_average = 0.0;
  double astar_average = 0.0;

  for (int i = 0; i < REPETITIONS; i++) {
    std::cout << (float(i) / REPETITIONS) * 100 << "% done" << std::endl;

    generate_depth_first_maze(maze);
    std::cout << "Maze generation done" << std::endl;

    auto random_walk_start = std::chrono::high_resolution_clock::now();
    if (false) {
      solve_random_walk_maze(maze);
    }
    auto random_walk_end = std::chrono::high_resolution_clock::now();

    auto w_f_left_start = std::chrono::high_resolution_clock::now();
    solve_wall_follower_maze(false, maze);
    auto w_f_left_end = std::chrono::high_resolution_clock::now();

    auto w_f_right_start = std::chrono::high_resolution_clock::now();
    solve_wall_follower_maze(true, maze);
    auto w_f_right_end = std::chrono::high_resolution_clock::now();

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

    std::chrono::duration<double, std::milli> random_walk_duration_ms = random_walk_end - random_walk_start;
    std::chrono::duration<double, std::milli> w_f_left_duration_ms = w_f_left_end - w_f_left_start;
    std::chrono::duration<double, std::milli> w_f_right_duration_ms = w_f_right_end - w_f_right_start;
    std::chrono::duration<double, std::milli> depth_first_duration_ms = depth_first_end - depth_first_start;
    std::chrono::duration<double, std::milli> breadth_first_duration_ms = breadth_first_end - breadth_first_start;
    std::chrono::duration<double, std::milli> dijkstra_duration_ms = dijkstra_end - dijkstra_start;
    std::chrono::duration<double, std::milli> astar_duration_ms = astar_end - astar_start;

    random_walk_average += random_walk_duration_ms.count();

    wall_follower_left_average += w_f_left_duration_ms.count();
    wall_follower_right_average += w_f_right_duration_ms.count();

    depth_first_average += depth_first_duration_ms.count();
    breadth_first_average += breadth_first_duration_ms.count();

    dijkstra_average += dijkstra_duration_ms.count();
    astar_average += astar_duration_ms.count();
  }

  random_walk_average /= REPETITIONS;

  wall_follower_left_average /= REPETITIONS;
  wall_follower_right_average /= REPETITIONS;

  depth_first_average /= REPETITIONS;
  breadth_first_average /= REPETITIONS;

  dijkstra_average /= REPETITIONS;
  astar_average /= REPETITIONS;

  std::cout << "Random Walk   average duration in ms: " << random_walk_average << std::endl;

  std::cout << "Wall F Left   average duration in ms: " << wall_follower_left_average << std::endl;
  std::cout << "Wall F Right  average duration in ms: " << wall_follower_right_average << std::endl;

  std::cout << "Depth First   average duration in ms: " << depth_first_average << std::endl;
  std::cout << "Breadth First average duration in ms: " << breadth_first_average << std::endl;

  std::cout << "Dijkstra      average duration in ms: " << dijkstra_average << std::endl;
  std::cout << "Astar         average duration in ms: " << astar_average << std::endl;
}
