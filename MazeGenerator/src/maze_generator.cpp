#include "maze_generator.hh"
#include "maze.hh"
#include "maze_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

void generate_depth_first_maze(Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  // Randomized Depth First Search
  std::vector<sf::Vector2u> maze_stack(0);
  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);

  sf::Vector2u cell_pos = {rand() % maze.grid_size.x,
                           rand() % maze.grid_size.y};

  maze_stack.push_back(cell_pos);
  visited_cells[maze.index_at_pos(cell_pos)] = true;

  while (!maze_stack.empty()) {
    std::vector<sf::Vector2u> next_cells =
        maze.get_neighbors(maze_stack.back());

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = maze.index_at_pos(next_cells[i]);

      if (visited_cells[next_index]) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
      }
    }

    if (next_cells.size() > 0) {
      sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

      maze.remove_wall(maze_stack.back(), rand_cell);

      maze_stack.push_back(rand_cell);
      visited_cells[maze.index_at_pos(rand_cell)] = true;
    } else {
      maze_stack.pop_back();
    }
  }
}

void animate_generate_depth_first_maze(MazeRenderer &renderer, Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  // Randomized Depth First Search
  std::vector<sf::Vector2u> maze_stack(0);
  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);

  sf::Vector2u cell_pos = {rand() % maze.grid_size.x,
                           rand() % maze.grid_size.y};

  maze_stack.push_back(cell_pos);
  visited_cells[maze.index_at_pos(cell_pos)] = true;

  while (!maze_stack.empty()) {
    // Window drawing
    while (const std::optional event = renderer.window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        renderer.window.close();
      }
    }

    renderer.window.clear();

    for (int i = 0; i < maze_stack.size() - 1; i++) {
      sf::RectangleShape maze_cell({maze.cell_size.x, maze.cell_size.y});
      sf::Vector2f maze_cell_pos =
          sf::Vector2f(maze_stack[i].x * maze.cell_size.x,
                       maze_stack[i].y * maze.cell_size.y) +
          sf::Vector2f(1.0f, 1.0f);
      maze_cell.setPosition(maze_cell_pos);

      float col_mult = (float)i / maze_stack.size();
      maze_cell.setFillColor(
          sf::Color(24 * col_mult, 196 * col_mult, 24 * col_mult));
      renderer.window.draw(maze_cell);
    }

    sf::RectangleShape current_cell({maze.cell_size.x, maze.cell_size.y});
    current_cell.setPosition({maze_stack.back().x * maze.cell_size.x,
                              maze_stack.back().y * maze.cell_size.y});
    current_cell.setFillColor(sf::Color(32, 255, 32));

    renderer.window.draw(current_cell);
    renderer.draw_grid(maze);
    renderer.window.display();

    const int STEPS_PER_FRAME = 3;

    for (int i = 0; i < STEPS_PER_FRAME; i++) {
      if (maze_stack.empty()) {
        break;
      }

      // Grid creation
      std::vector<sf::Vector2u> next_cells =
          maze.get_neighbors(maze_stack.back());

      for (int i = 0; i < next_cells.size(); i++) {
        int next_index = maze.index_at_pos(next_cells[i]);

        if (visited_cells[next_index]) {
          next_cells.erase(next_cells.begin() + i);
          i -= 1;
        }
      }

      if (next_cells.size() > 0) {
        sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

        maze.remove_wall(maze_stack.back(), rand_cell);

        maze_stack.push_back(rand_cell);
        visited_cells[maze.index_at_pos(rand_cell)] = true;
      } else {
        maze_stack.pop_back();
      }
    }
  }
}

void generate_prim_maze(Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y);
  std::vector<Wall> walls;

  sf::Vector2u start_pos = {rand() % maze.grid_size.x,
                            rand() % maze.grid_size.y};
  visited_cells[maze.index_at_pos(start_pos)] = true;

  std::vector<sf::Vector2u> next_cells = maze.get_neighbors(start_pos);

  for (int i = 0; i < next_cells.size(); i++) {
    Wall new_wall = Wall();
    new_wall.cell1 = start_pos;
    new_wall.cell2 = next_cells[i];
    walls.push_back(new_wall);
  }

  while (!walls.empty()) {
    int random_wall_index = rand() % walls.size();
    Wall random_wall = walls[random_wall_index];

    int second_cell_index = maze.index_at_pos(random_wall.cell2);

    walls.erase(walls.begin() + random_wall_index);

    if (visited_cells[second_cell_index] == false) {
      // Remove the wall from list and on grid walls_bitmap
      // Add new_cell walls to walls and check visited

      maze.remove_wall(random_wall.cell1, random_wall.cell2);

      int cell2_index = maze.index_at_pos(random_wall.cell2);
      visited_cells[cell2_index] = true;

      std::vector<sf::Vector2u> next_cells =
          maze.get_neighbors(random_wall.cell2);

      for (int i = next_cells.size() - 1; i >= 0; i--) {
        int next_index = maze.index_at_pos(next_cells[i]);

        if (visited_cells[next_index]) {
          next_cells.erase(next_cells.begin() + i);
        }
      }

      for (int i = 0; i < next_cells.size(); i++) {
        Wall new_wall = Wall();
        new_wall.cell1 = random_wall.cell2;
        new_wall.cell2 = next_cells[i];
        walls.push_back(new_wall);
      }
    }
  }
}

void generate_kruskal_maze(Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  std::vector<int> parent;
  std::vector<int> rank;

  parent.resize(maze.grid.size());
  rank.resize(maze.grid.size());

  for (int i = 0; i < maze.grid.size(); i++) {
    parent[i] = i;
  }

  // Create all Walls
  std::vector<Wall> walls;

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      sf::Vector2u cell(x, y);

      sf::Vector2u right_cell = cell + sf::Vector2u(1, 0);
      sf::Vector2u down_cell = cell + sf::Vector2u(0, 1);

      if (maze.is_inside(right_cell)) {
        walls.push_back({cell, right_cell});
      }

      if (maze.is_inside(down_cell)) {
        walls.push_back({cell, down_cell});
      }
    }
  }

  std::random_device rd;
  std::mt19937 rng(rd());
  std::shuffle(walls.begin(), walls.end(), rng);

  for (const Wall &wall : walls) {
    int cell1_index = maze.index_at_pos(wall.cell1);
    int cell2_index = maze.index_at_pos(wall.cell2);

    // Find parent of cell1
    while (parent[cell1_index] != cell1_index) {
      cell1_index = parent[cell1_index];
    }
    int cell1_parent = cell1_index;

    // Find parent of cell2
    while (parent[cell2_index] != cell2_index) {
      cell2_index = parent[cell2_index];
    }
    int cell2_parent = cell2_index;

    if (cell1_parent == cell2_parent) {
      continue;
    }

    if (rank[cell1_parent] < rank[cell2_parent]) {
      parent[cell1_parent] = cell2_parent;
    } else {
      parent[cell2_parent] = cell1_parent;
      if (rank[cell1_parent] == rank[cell2_parent]) {
        rank[cell1_parent] += 1;
      }
    }

    maze.remove_wall(wall.cell1, wall.cell2);
  }
}

void generate_wilson_maze(Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  std::vector<bool> in_maze;
  in_maze.resize(maze.grid.size(), false);

  int seed = rand() % maze.grid.size();
  in_maze[seed] = true;

  int remaining_cells = in_maze.size() - 1;

  while (remaining_cells > 0) {
    sf::Vector2u start_cell;
    do {
      start_cell = {rand() % maze.grid_size.x, rand() % maze.grid_size.y};
    } while (in_maze[maze.index_at_pos(start_cell)]);

    std::vector<sf::Vector2u> path;
    std::unordered_map<int, int> pos_in_path;

    path.push_back(start_cell);
    pos_in_path[maze.index_at_pos(start_cell)] = 0;

    while (!in_maze[maze.index_at_pos(path.back())]) {
      std::vector<sf::Vector2u> neighbors = maze.get_neighbors(path.back());
      sf::Vector2u next = neighbors[rand() % neighbors.size()];
      int next_index = maze.index_at_pos(next);

      if (pos_in_path.count(next_index)) {
        int loop_start = pos_in_path[next_index];
        // Remove all entries in pos_in_path for erased cells
        for (size_t i = loop_start + 1; i < path.size(); i++) {
          pos_in_path.erase(maze.index_at_pos(path[i]));
        }
        path.erase(path.begin() + loop_start + 1, path.end());
      } else {
        path.push_back(next);
        pos_in_path[next_index] = path.size() - 1;
      }
    }

    for (int i = 0; i + 1 < path.size(); i++) {
      maze.remove_wall(path[i], path[i + 1]);

      int index = maze.index_at_pos(path[i]);
      if (!in_maze[index]) {
        in_maze[index] = true;
        remaining_cells--;
      }
    }

    int index = maze.index_at_pos(path.back());
    if (!in_maze[index]) {
      in_maze[index] = true;
      remaining_cells--;
    }
  }
}

void animate_generate_wilson_maze(MazeRenderer &renderer, Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 15;
    }
  }

  std::vector<bool> in_maze;
  in_maze.resize(maze.grid.size(), false);

  int seed = rand() % maze.grid.size();
  in_maze[seed] = true;

  int remaining_cells = in_maze.size() - 1;

  while (remaining_cells > 0) {
    sf::Vector2u start_cell;
    do {
      start_cell = {rand() % maze.grid_size.x, rand() % maze.grid_size.y};
    } while (in_maze[maze.index_at_pos(start_cell)]);

    std::vector<sf::Vector2u> path;
    std::unordered_map<int, int> pos_in_path;

    path.push_back(start_cell);
    pos_in_path[maze.index_at_pos(start_cell)] = 0;

    while (!in_maze[maze.index_at_pos(path.back())]) {
      // Window drawing
      while (const std::optional event = renderer.window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
          renderer.window.close();
        }
      }

      renderer.window.clear();

      for (int y = 0; y < maze.grid_size.y; y++) {
        for (int x = 0; x < maze.grid_size.x; x++) {
          if (!in_maze[maze.index_at_pos(sf::Vector2u(x, y))]) {
            continue;
          }

          sf::RectangleShape maze_cell({maze.cell_size.x, maze.cell_size.y});
          sf::Vector2f maze_cell_pos =
              sf::Vector2f(x * maze.cell_size.x, y * maze.cell_size.y) +
              sf::Vector2f(1.0f, 1.0f);
          maze_cell.setPosition(maze_cell_pos);

          maze_cell.setFillColor(sf::Color(64, 196, 64));
          renderer.window.draw(maze_cell);
        }
      }

      for (int i = 0; i < path.size(); i++) {
        sf::RectangleShape path_cell({maze.cell_size.x, maze.cell_size.y});
        sf::Vector2f path_cell_pos =
            sf::Vector2f(path[i].x * maze.cell_size.x,
                         path[i].y * maze.cell_size.y) +
            sf::Vector2f(1.0f, 1.0f);
        path_cell.setPosition(path_cell_pos);

        path_cell.setFillColor(sf::Color(196, 64, 64));
        renderer.window.draw(path_cell);
      }

      renderer.draw_grid(maze);
      renderer.window.display();

      std::vector<sf::Vector2u> neighbors = maze.get_neighbors(path.back());
      sf::Vector2u next = neighbors[rand() % neighbors.size()];
      int next_index = maze.index_at_pos(next);

      if (pos_in_path.count(next_index)) {
        int loop_start = pos_in_path[next_index];
        // Remove all entries in pos_in_path for erased cells
        for (size_t i = loop_start + 1; i < path.size(); i++) {
          pos_in_path.erase(maze.index_at_pos(path[i]));
        }
        path.erase(path.begin() + loop_start + 1, path.end());
      } else {
        path.push_back(next);
        pos_in_path[next_index] = path.size() - 1;
      }
    }

    for (int i = 0; i + 1 < path.size(); i++) {
      maze.remove_wall(path[i], path[i + 1]);

      int index = maze.index_at_pos(path[i]);
      if (!in_maze[index]) {
        in_maze[index] = true;
        remaining_cells--;
      }
    }

    int index = maze.index_at_pos(path.back());
    if (!in_maze[index]) {
      in_maze[index] = true;
      remaining_cells--;
    }
  }
}

void recursive_divide_grid(Maze &maze, sf::Vector2u &pos, sf::Vector2u &dim) {
  if (dim.x < 2 || dim.y < 2) {
    return;
  }

  unsigned int rand_x = (pos.x + rand() % (dim.x - 1));
  unsigned int rand_y = (pos.y + rand() % (dim.y - 1));

  sf::Vector2u rand_cell = sf::Vector2u(rand_x, rand_y);

  // Split horizontal or vertical
  // horizontal

  bool horizontal = dim.y > dim.x;
  if (horizontal) {
    int x_passage = pos.x + rand() % dim.x;

    for (int x_cell = pos.x; x_cell < pos.x + dim.x; x_cell++) {
      if (x_cell == x_passage) {
        continue;
      }

      sf::Vector2u above_cell = sf::Vector2u(x_cell, rand_cell.y);
      sf::Vector2u below_cell = sf::Vector2u(x_cell, rand_cell.y + 1);

      maze.set_wall(above_cell, below_cell);
    }

    unsigned int split_y = rand_cell.y;

    sf::Vector2u above_pos = pos;
    sf::Vector2u below_pos = sf::Vector2u(pos.x, split_y + 1);

    sf::Vector2u above_dim = sf::Vector2u(dim.x, split_y - pos.y + 1);
    sf::Vector2u below_dim = sf::Vector2u(dim.x, pos.y + dim.y - (split_y + 1));

    recursive_divide_grid(maze, above_pos, above_dim);
    recursive_divide_grid(maze, below_pos, below_dim);
  }
  // vertical
  else {
    int y_passage = pos.y + rand() % dim.y;

    for (int y_cell = pos.y; y_cell < pos.y + dim.y; y_cell++) {
      if (y_cell == y_passage) {
        continue;
      }

      sf::Vector2u left_cell = sf::Vector2u(rand_cell.x, y_cell);
      sf::Vector2u right_cell = sf::Vector2u(rand_cell.x + 1, y_cell);

      maze.set_wall(left_cell, right_cell);
    }

    unsigned int split_x = rand_cell.x;

    sf::Vector2u left_pos = pos;
    sf::Vector2u right_pos = sf::Vector2u(split_x + 1, pos.y);

    sf::Vector2u left_dim = sf::Vector2u(split_x - pos.x + 1, dim.y);
    sf::Vector2u right_dim = sf::Vector2u(pos.x + dim.x - (split_x + 1), dim.y);

    recursive_divide_grid(maze, left_pos, left_dim);
    recursive_divide_grid(maze, right_pos, right_dim);
  }
}

void generate_recursive_division_maze(Maze &maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      maze.grid[index].walls_bitmap = 0;
    }
  }

  // Generate Wall border
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int cell_index = maze.index_at_pos(sf::Vector2u(x, y));
      unsigned char &cell_bitmap = maze.grid[cell_index].walls_bitmap;

      if (x == 0) {
        cell_bitmap |= (1 << 3);
      }

      if (x == maze.grid_size.x - 1) {
        cell_bitmap |= (1 << 1);
      }

      if (y == 0) {
        cell_bitmap |= (1 << 0);
      }

      if (y == maze.grid_size.y - 1) {
        cell_bitmap |= (1 << 2);
      }
    }
  }

  sf::Vector2u divide_pos = sf::Vector2u(0, 0);
  sf::Vector2u divide_dim = sf::Vector2u(maze.grid_size.x, maze.grid_size.y);

  recursive_divide_grid(maze, divide_pos, divide_dim);
}
