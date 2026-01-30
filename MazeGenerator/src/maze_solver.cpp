#include "maze_solver.hh"
#include "maze.hh"
#include "maze_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <iterator>
#include <queue>
#include <vector>

void solve_random_walk_maze(Maze &maze) {
  maze.path.clear();
  maze.path.push_back(maze.start_cell);

  while (true) {
    std::vector<sf::Vector2u> next_cells = maze.get_neighbors(maze.path.back());

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = maze.index_at_pos(next_cells[i]);

      if (!maze.is_path_free(maze.path.back(), next_cells[i])) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
        continue;
      }
    }

    sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

    maze.path.push_back(rand_cell);

    if (rand_cell == maze.end_cell) {
      return;
    }
  }
}

void solve_wall_follower_maze(bool follow_right, Maze &maze) {
  maze.path.clear();
  maze.path.push_back(maze.start_cell);

  std::vector<sf::Vector2i> directions{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
  int curr_dir = 2;

  std::array<int, 4> turn_order;

  while (maze.path.back() != maze.end_cell) {
    if (follow_right) {
      turn_order = {1, 0, 3, 2};
    } else {
      turn_order = {3, 0, 1, 2};
    }

    sf::Vector2u curr_cell = maze.path.back();

    for (int turn : turn_order) {
      sf::Vector2i next_dir = directions[(curr_dir + turn) % 4];
      sf::Vector2u next_cell = {maze.path.back().x + next_dir.x,
                                maze.path.back().y + next_dir.y};

      if (maze.is_path_free(curr_cell, next_cell)) {
        curr_dir = (curr_dir + turn) % 4;
        maze.path.push_back(next_cell);
        break;
      }
    }
  }
}

void solve_depth_first_maze(Maze &maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);
  visited_cells[maze.index_at_pos(maze.start_cell)] = true;

  while (!maze.path.empty()) {
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

void solve_breadth_first_maze(Maze &maze) {
  maze.path.clear();

  std::queue<sf::Vector2u> maze_queue;
  maze_queue.push(maze.start_cell);

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y, false);
  visited_cells[maze.index_at_pos(maze_queue.back())] = true;

  std::vector<int> parent(maze.grid_size.x * maze.grid_size.y, -1);

  while (!maze_queue.empty()) {
    sf::Vector2u curr_cell = maze_queue.front();
    maze_queue.pop();

    int curr_index = maze.index_at_pos(curr_cell);

    if (curr_cell == maze.end_cell) {
      break;
    }

    std::vector<sf::Vector2u> next_cells = maze.get_neighbors(curr_cell);

    for (const sf::Vector2u &next_cell : next_cells) {
      int next_index = maze.index_at_pos(next_cell);

      if (visited_cells[next_index]) {
        continue;
      }

      if (!maze.is_path_free(curr_cell, next_cell)) {
        continue;
      }

      visited_cells[next_index] = true;
      parent[next_index] = curr_index;
      maze_queue.push(next_cell);
    }
  }

  int curr_index = maze.index_at_pos(maze.end_cell);

  while (curr_index != -1) {
    maze.path.push_back(maze.pos_at_index(curr_index));
    curr_index = parent[curr_index];
  }

  std::reverse(maze.path.begin(), maze.path.end());
}
