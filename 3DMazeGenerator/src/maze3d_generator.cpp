#include "maze3d_generator.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <vector>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void generate_depth_first_maze(Maze3D& maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos(sf::Vector3i(x, y, z));

        maze.grid[index].walls_bitmap = 63;
      }
    }
  }

  // Randomized Depth First Search
  std::vector<sf::Vector3i> maze_stack(0);
  maze_stack.reserve((maze.grid_size.x * maze.grid_size.y * maze.grid_size.z) / 16);
  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y * maze.grid_size.z);

  sf::Vector3i cell_pos = {rand() % maze.grid_size.x, rand() % maze.grid_size.y, rand() % maze.grid_size.z};

  maze_stack.push_back(cell_pos);
  visited_cells[maze.index_at_pos(cell_pos)] = true;
  std::vector<sf::Vector3i> next_cells;
  std::vector<sf::Vector3i> unvisited;

  while (!maze_stack.empty()) {
    next_cells.clear();
    unvisited.clear();
    next_cells = maze.get_neighbors(maze_stack.back());

    for (auto& next_cell : next_cells) {
      if (!visited_cells[maze.index_at_pos(next_cell)])
        unvisited.push_back(next_cell);
    }

    if (!unvisited.empty()) {
      sf::Vector3i rand_cell = unvisited[rand() % unvisited.size()];

      maze.remove_wall(maze_stack.back(), rand_cell);

      maze_stack.push_back(rand_cell);
      visited_cells[maze.index_at_pos(rand_cell)] = true;
    } else {
      maze_stack.pop_back();
    }
  }
}

void animate_generate_depth_first_maze(Maze3DRenderer& renderer, Maze3D& maze) {
  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos(sf::Vector3i(x, y, z));

        maze.grid[index].walls_bitmap = 63;
      }
    }
  }

  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  // Randomized Depth First Search
  std::vector<sf::Vector3i> maze_stack(0);
  maze_stack.reserve((maze.grid_size.x * maze.grid_size.y * maze.grid_size.z) / 16);
  std::vector<bool> visited_cells(maze.grid_size.x * maze.grid_size.y * maze.grid_size.z);

  sf::Vector3i cell_pos = {rand() % maze.grid_size.x, rand() % maze.grid_size.y, rand() % maze.grid_size.z};

  maze_stack.push_back(cell_pos);
  visited_cells[maze.index_at_pos(cell_pos)] = true;
  std::vector<sf::Vector3i> next_cells;
  std::vector<sf::Vector3i> unvisited;

  while (!maze_stack.empty()) {
    // Window drawing
    while (const std::optional event = renderer.window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        renderer.window.close();
      }
    }

    renderer.window.clear();

    if (maze_stack.size() >= 2) {
      std::vector<sf::Vector3f> vertices;
      sf::VertexArray line_path(sf::PrimitiveType::Lines, 2 * (maze_stack.size() - 1));
      int point_index = 0;

      for (const sf::Vector3i& p : maze_stack) {
        sf::Vector3f path_point(p.x * maze.cell_size.x + maze.cell_size.x / 2,
                                p.y * maze.cell_size.y + maze.cell_size.y / 2,
                                p.z * maze.cell_size.z + maze.cell_size.z / 2);

        renderer.rotate_x(path_point, renderer.cube_rotation.x);
        renderer.rotate_y(path_point, renderer.cube_rotation.y);
        renderer.rotate_z(path_point, renderer.cube_rotation.z);
        renderer.translate(path_point, -center);

        renderer.apply_camera(path_point, renderer.camera);

        vertices.push_back(path_point);
      }

      for (int i = 0; i < vertices.size() - 1; i++) {
        bool visible_line = true;

        sf::Vector3f path_point1 = vertices[i];
        sf::Vector3f path_point2 = vertices[i + 1];

        if (renderer.project_perspective) {
          visible_line = renderer.clipLineToNearPlane(path_point1, path_point2, 0.01f);
          renderer.pers_projection(path_point1, renderer.window.getSize());
          renderer.pers_projection(path_point2, renderer.window.getSize());

        } else {
          renderer.ortho_projection(path_point1, renderer.window.getSize(), renderer.ortho_zoom);
          renderer.ortho_projection(path_point2, renderer.window.getSize(), renderer.ortho_zoom);
        }

        if (visible_line) {
          line_path[point_index].position = {path_point1.x, path_point1.y};
          line_path[point_index + 1].position = {path_point2.x, path_point2.y};

          line_path[point_index].color = renderer.path_color;
          line_path[point_index + 1].color = renderer.path_color;
        }

        point_index += 2;
      }

      renderer.window.draw(line_path);
      renderer.draw_grid(maze);
      renderer.window.display();
    }

    next_cells.clear();
    unvisited.clear();
    next_cells = maze.get_neighbors(maze_stack.back());

    for (auto& next_cell : next_cells) {
      if (!visited_cells[maze.index_at_pos(next_cell)])
        unvisited.push_back(next_cell);
    }

    if (!unvisited.empty()) {
      sf::Vector3i rand_cell = unvisited[rand() % unvisited.size()];

      maze.remove_wall(maze_stack.back(), rand_cell);

      maze_stack.push_back(rand_cell);
      visited_cells[maze.index_at_pos(rand_cell)] = true;
    } else {
      maze_stack.pop_back();
    }
  }
}
