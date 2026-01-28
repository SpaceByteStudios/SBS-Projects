#include "maze_generator.hh"
#include "maze.hh"
#include "maze_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <random>
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
