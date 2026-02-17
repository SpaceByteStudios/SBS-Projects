#include "maze3d_solver.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <vector>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void solve_depth_first_maze(Maze3D& maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y * maze.grid_size.z);
  visited_cells[maze.index_at_pos(maze.start_cell)] = true;

  while (!maze.path.empty()) {
    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(maze.path.back());

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
      sf::Vector3i rand_cell = next_cells[rand() % next_cells.size()];

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

void animate_solve_depth_first_maze(Maze3DRenderer& renderer, Maze3D& maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  sf::Vector3i curr_pos = maze.start_cell;

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y * maze.grid_size.z);
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

    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(maze.path.back());

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
      sf::Vector3i rand_cell = next_cells[rand() % next_cells.size()];

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
