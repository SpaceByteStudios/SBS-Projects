#include "maze_solver.hh"
#include "maze.hh"
#include "maze_renderer.hh"

#include <SFML/Graphics.hpp>

void solve_depth_first_maze(Maze &maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  sf::Vector2u curr_pos = maze.start_cell;

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);
  visited_cells[maze.index_at_pos(curr_pos)] = true;

  while (curr_pos != maze.end_cell && !maze.path.empty()) {
    std::vector<sf::Vector2u> next_cells = maze.get_neighbors(maze.path.back());

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = maze.index_at_pos(next_cells[i]);

      if (!maze.is_path_free(maze.path.back(), next_cells[i])) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
        continue;
      }

      if (visited_cells[next_index]) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
      }
    }

    if (next_cells.size() > 0) {
      sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

      maze.path.push_back(rand_cell);
      visited_cells[maze.index_at_pos(rand_cell)] = true;

      if (rand_cell == maze.end_cell) {
        return;
      }
    } else {
      maze.path.pop_back();
    }
  }
}

void animate_solve_depth_first_maze(MazeRenderer &renderer, Maze &maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  sf::Vector2u curr_pos = maze.start_cell;

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);
  visited_cells[maze.index_at_pos(curr_pos)] = true;

  while (curr_pos != maze.end_cell && !maze.path.empty()) {
    while (const std::optional event = renderer.window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        renderer.window.close();
      }
    }

    renderer.window.clear();

    renderer.draw_path(maze);
    renderer.draw_grid(maze);

    renderer.window.display();

    const int STEPS_PER_FRAME = 3;

    for (int i = 0; i < STEPS_PER_FRAME; i++) {

      std::vector<sf::Vector2u> next_cells =
          maze.get_neighbors(maze.path.back());

      for (int i = 0; i < next_cells.size(); i++) {
        int next_index = maze.index_at_pos(next_cells[i]);

        if (!maze.is_path_free(maze.path.back(), next_cells[i])) {
          next_cells.erase(next_cells.begin() + i);
          i -= 1;
          continue;
        }

        if (visited_cells[next_index]) {
          next_cells.erase(next_cells.begin() + i);
          i -= 1;
        }
      }

      if (next_cells.size() > 0) {
        sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

        maze.path.push_back(rand_cell);
        visited_cells[maze.index_at_pos(rand_cell)] = true;

        if (rand_cell == maze.end_cell) {
          return;
        }
      } else {
        maze.path.pop_back();
      }
    }
  }
}
