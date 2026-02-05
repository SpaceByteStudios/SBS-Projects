#include "maze3d_solver.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void solve_random_walk_maze(Maze3D& maze) {
  maze.path.clear();
  maze.path.push_back(maze.start_cell);

  while (true) {
    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(maze.path.back());

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = maze.index_at_pos(next_cells[i]);

      if (!maze.is_path_free(maze.path.back(), next_cells[i])) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
        continue;
      }
    }

    sf::Vector3i rand_cell = next_cells[rand() % next_cells.size()];

    maze.path.push_back(rand_cell);

    if (rand_cell == maze.end_cell) {
      return;
    }
  }
}

void solve_depth_first_maze(Maze3D& maze) {
  maze.path.clear();

  maze.path.push_back(maze.start_cell);

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);
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
}

void solve_breadth_first_maze(Maze3D& maze) {
  maze.path.clear();

  std::queue<sf::Vector3i> maze_queue;
  maze_queue.push(maze.start_cell);

  const int total_cells = maze.grid_size.x * maze.grid_size.y;
  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y, false);
  visited_cells[maze.index_at_pos(maze_queue.back())] = true;

  std::vector<int> parent(maze.grid_size.x * maze.grid_size.y, -1);

  int visited_count = 1;

  const double progress_interval = 0.01; // 0,01%
  double last_displayed_percent = -1.0;
  const int cells_per_update = std::max(1, static_cast<int>(total_cells * progress_interval / 100.0));
  int next_update_cell = cells_per_update;

  while (!maze_queue.empty()) {
    sf::Vector3i curr_cell = maze_queue.front();
    maze_queue.pop();

    int curr_index = maze.index_at_pos(curr_cell);

    if (curr_cell == maze.end_cell) {
      break;
    }

    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(curr_cell);

    for (const sf::Vector3i& next_cell : next_cells) {
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

      visited_count++;

      if (visited_count >= next_update_cell) {
        double percent = (visited_count * 100.0) / total_cells;
        double displayed_percent = std::floor(percent * 100.0) / 100.0; // 2 Nachkommastellen
        if (displayed_percent != last_displayed_percent) {
          last_displayed_percent = displayed_percent;
          std::cout << "\rSolving maze: " << std::fixed << std::setprecision(2) << displayed_percent << "% completed"
                    << std::flush;
        }
        next_update_cell += cells_per_update;
      }
    }
  }

  int curr_index = maze.index_at_pos(maze.end_cell);

  while (curr_index != -1) {
    // maze.path.push_back(maze.pos_at_index(curr_index));
    curr_index = parent[curr_index];
  }

  std::reverse(maze.path.begin(), maze.path.end());

  std::cout << "\rSolving maze: 100% completed" << std::endl;
}

struct Node {
  int dist;
  int estimated_dist;
  sf::Vector3i pos;
  bool operator>(const Node& other) const {
    if (estimated_dist == other.estimated_dist) {
      return dist < other.dist;
    }
    return estimated_dist > other.estimated_dist;
  }
};

void solve_dijkstra_maze(Maze3D& maze) {
  std::vector<int> dist(maze.grid_size.x * maze.grid_size.y, 1e9);
  std::vector<int> parent(maze.grid_size.x * maze.grid_size.y, -1);

  dist[maze.index_at_pos(maze.start_cell)] = 0;

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> p_queue;

  p_queue.push({0, 0, maze.start_cell});

  while (!p_queue.empty()) {
    Node top_node = p_queue.top();

    p_queue.pop();

    if (top_node.pos == maze.end_cell) {
      break;
    }

    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(top_node.pos);

    int top_index = maze.index_at_pos(top_node.pos);

    for (sf::Vector3i& next_cell : next_cells) {
      if (!maze.is_path_free(top_node.pos, next_cell)) {
        continue;
      }

      int next_index = maze.index_at_pos(next_cell);

      int alt_dist = top_node.dist + 1;

      if (alt_dist < dist[next_index]) {
        dist[next_index] = alt_dist;
        parent[next_index] = top_index;
        p_queue.push({alt_dist, alt_dist, next_cell});
      }
    }
  }

  maze.path.clear();
  int curr_index = maze.index_at_pos(maze.end_cell);

  if (parent[curr_index] != -1 || maze.start_cell == maze.end_cell) {
    // Reconstruct path
    while (curr_index != -1) {
      // maze.path.push_back(maze.pos_at_index(curr_index));
      curr_index = parent[curr_index];
    }

    std::reverse(maze.path.begin(), maze.path.end());
  }
}

int heuristic(sf::Vector3i pos1, sf::Vector3i pos2) {
  int dx = std::abs((int)pos1.x - (int)pos2.x);
  int dy = std::abs((int)pos1.y - (int)pos2.y);
  return std::max(dx, dy);
}

void solve_astar_maze(Maze3D& maze) {
  std::vector<int> dist(maze.grid_size.x * maze.grid_size.y, 1e9);
  std::vector<int> parent(maze.grid_size.x * maze.grid_size.y, -1);
  std::vector<bool> closed(maze.grid_size.x * maze.grid_size.y, false);

  dist[maze.index_at_pos(maze.start_cell)] = 0;

  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> p_queue;

  int est_dist_start = heuristic(maze.start_cell, maze.end_cell);

  p_queue.push({0, est_dist_start, maze.start_cell});

  while (!p_queue.empty()) {
    Node top_node = p_queue.top();
    int top_index = maze.index_at_pos(top_node.pos);

    p_queue.pop();

    if (closed[top_index]) {
      continue;
    }

    if (top_node.dist > dist[top_index]) {
      continue;
    }

    closed[top_index] = true;

    if (top_node.pos == maze.end_cell) {
      break;
    }

    std::vector<sf::Vector3i> next_cells = maze.get_neighbors(top_node.pos);

    for (sf::Vector3i& next_cell : next_cells) {
      if (!maze.is_path_free(top_node.pos, next_cell)) {
        continue;
      }

      int next_index = maze.index_at_pos(next_cell);

      if (closed[next_index]) {
        continue;
      }

      int alt_dist = top_node.dist + 1;
      int est_dist = alt_dist + heuristic(next_cell, maze.end_cell);

      if (alt_dist < dist[next_index]) {
        dist[next_index] = alt_dist;
        parent[next_index] = top_index;
        p_queue.push({alt_dist, est_dist, next_cell});
      }
    }
  }

  maze.path.clear();
  int curr_index = maze.index_at_pos(maze.end_cell);

  if (parent[curr_index] != -1 || maze.start_cell == maze.end_cell) {
    // Reconstruct path
    while (curr_index != -1) {
      // maze.path.push_back(maze.pos_at_index(curr_index));
      curr_index = parent[curr_index];
    }

    std::reverse(maze.path.begin(), maze.path.end());
  }
}
