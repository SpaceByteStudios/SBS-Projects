#include "maze_solver.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <queue>
#include <vector>

#include "maze.hh"
#include "maze_renderer.hh"

void solve_random_walk_maze(Maze& maze) {
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

void solve_wall_follower_maze(bool follow_right, Maze& maze) {
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
      sf::Vector2u next_cell = {maze.path.back().x + next_dir.x, maze.path.back().y + next_dir.y};

      if (maze.is_path_free(curr_cell, next_cell)) {
        curr_dir = (curr_dir + turn) % 4;
        maze.path.push_back(next_cell);
        break;
      }
    }
  }
}

void solve_depth_first_maze(Maze& maze) {
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

void animate_solve_depth_first_maze(MazeRenderer& renderer, Maze& maze) {
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
}

void solve_breadth_first_maze(Maze& maze) {
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

    for (const sf::Vector2u& next_cell : next_cells) {
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

struct Node {
  int dist;
  int estimated_dist;
  sf::Vector2u pos;
  bool operator>(const Node& other) const {
    if (estimated_dist == other.estimated_dist) {
      return dist < other.dist;
    }
    return estimated_dist > other.estimated_dist;
  }
};

void solve_dijkstra_maze(Maze& maze) {
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

    std::vector<sf::Vector2u> next_cells = maze.get_neighbors(top_node.pos);

    int top_index = maze.index_at_pos(top_node.pos);

    for (sf::Vector2u& next_cell : next_cells) {
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
      maze.path.push_back(maze.pos_at_index(curr_index));
      curr_index = parent[curr_index];
    }

    std::reverse(maze.path.begin(), maze.path.end());
  }
}

int heuristic(sf::Vector2u pos1, sf::Vector2u pos2) {
  int dx = std::abs((int)pos1.x - (int)pos2.x);
  int dy = std::abs((int)pos1.y - (int)pos2.y);
  return std::max(dx, dy);
}

void solve_astar_maze(Maze& maze) {
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

    std::vector<sf::Vector2u> next_cells = maze.get_neighbors(top_node.pos);

    for (sf::Vector2u& next_cell : next_cells) {
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
      maze.path.push_back(maze.pos_at_index(curr_index));
      curr_index = parent[curr_index];
    }

    std::reverse(maze.path.begin(), maze.path.end());
  }
}
