#include "maze3d_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "maze3d.hh"

Maze3DRenderer::Maze3DRenderer(sf::RenderWindow& new_window) : window(new_window) {
}

sf::Vector3f rotate_x(const sf::Vector3f& v, float angle) {
  float new_y = v.y * std::cos(angle) - v.z * std::sin(angle);
  float new_z = v.y * std::sin(angle) + v.z * std::cos(angle);
  return {v.x, new_y, new_z};
}

sf::Vector3f rotate_y(const sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) + v.z * std::sin(angle);
  float new_z = v.z * std::cos(angle) - v.x * std::sin(angle);
  return {new_x, v.y, new_z};
}

sf::Vector3f rotate_z(const sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) - v.y * std::sin(angle);
  float new_y = v.x * std::sin(angle) + v.y * std::cos(angle);
  return {new_x, new_y, v.z};
}

sf::Vector3f translate(const sf::Vector3f& v, const sf::Vector3f& t) {
  return v + t;
}

sf::Vector3f ortho_project(const sf::Vector3f& v, const sf::Vector2u& window_size) {
  float normalized_x = v.x * (2.0 / window_size.x);
  float normalized_y = v.y * (2.0 / window_size.y);

  float new_x = (normalized_x + 1.0f) * 0.5f * window_size.x;
  float new_y = (normalized_y + 1.0f) * 0.5f * window_size.y;
  return {new_x, new_y, v.z};
}

void Maze3DRenderer::draw_grid(const Maze3D& maze) {
  const int vertices_per_cell = 60;
  sf::VertexArray lines(sf::PrimitiveType::Lines,
                        maze.grid_size.x * maze.grid_size.y * maze.grid_size.z * vertices_per_cell);

  int cell_vertex = 0;

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos(sf::Vector3i(x, y, z));

        std::vector<sf::Vector3f> vertex_pos = {
            {x * maze.cell_size.x, y * maze.cell_size.y, z * maze.cell_size.z},
            {x * maze.cell_size.x, y * maze.cell_size.y, (z + 1) * maze.cell_size.z},
            {x * maze.cell_size.x, (y + 1) * maze.cell_size.y, z * maze.cell_size.z},
            {x * maze.cell_size.x, (y + 1) * maze.cell_size.y, (z + 1) * maze.cell_size.z},
            {(x + 1) * maze.cell_size.x, y * maze.cell_size.y, z * maze.cell_size.z},
            {(x + 1) * maze.cell_size.x, y * maze.cell_size.y, (z + 1) * maze.cell_size.z},
            {(x + 1) * maze.cell_size.x, (y + 1) * maze.cell_size.y, z * maze.cell_size.z},
            {(x + 1) * maze.cell_size.x, (y + 1) * maze.cell_size.y, (z + 1) * maze.cell_size.z}};

        // Vertex indeces for walls
        std::vector<std::vector<int>> walls = {{2, 3, 7, 6, 2, 7}, {4, 5, 6, 7, 4, 6}, {1, 3, 5, 7, 1, 5},
                                               {0, 1, 2, 3, 0, 2}, {0, 2, 4, 6, 0, 4}, {0, 1, 4, 5, 0, 4}};

        sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                               maze.cell_size.z * maze.grid_size.z * 0.5f};

        for (sf::Vector3f& v : vertex_pos) {
          v -= center;
          v = rotate_x(v, rotation.x);
          v = rotate_y(v, rotation.y);
          v = rotate_z(v, rotation.z);
          v += center;

          v = translate(v, {0, 0, 0});
          v = ortho_project(v, window.getSize());
        }

        // Top: Green, Front: Red, Right: Blue, Back: Cyan,  Left: Yellow,  Bottom: Purple
        std::vector<sf::Color> wall_colors = {sf::Color{0, 255, 0},   sf::Color{255, 0, 0},   sf::Color{0, 0, 255},
                                              sf::Color{0, 255, 255}, sf::Color{255, 255, 0}, sf::Color{255, 0, 255}};

        // Each Wall
        for (int i = 0; i < 1; i++) {
          if (maze.grid[index].walls_bitmap & (1 << i)) {
            for (int j = 0; j < 5; j++) {
              int point = i * 6 + j * 2;

              sf::Vector3f pos1 = vertex_pos[walls[i][j]];
              sf::Vector3f pos2 = vertex_pos[walls[i][j + 1]];

              lines[cell_vertex + point].position = {pos1.x, pos1.y};
              lines[cell_vertex + point + 1].position = {pos2.x, pos2.y};

              lines[cell_vertex + point].color = wall_colors[i];
              lines[cell_vertex + point + 1].color = wall_colors[i];
            }
          }
        }

        for (int i = 5; i < 6; i++) {
          if (maze.grid[index].walls_bitmap & (1 << i)) {
            for (int j = 0; j < 5; j++) {
              int point = i * 6 + j * 2;

              sf::Vector3f pos1 = vertex_pos[walls[i][j]];
              sf::Vector3f pos2 = vertex_pos[walls[i][j + 1]];

              lines[cell_vertex + point].position = {pos1.x, pos1.y};
              lines[cell_vertex + point + 1].position = {pos2.x, pos2.y};

              lines[cell_vertex + point].color = wall_colors[i];
              lines[cell_vertex + point + 1].color = wall_colors[i];
            }
          }
        }

        cell_vertex += vertices_per_cell;
      }
    }
  }

  window.draw(lines);
}

void Maze3DRenderer::draw_path(const Maze3D& maze) {
  return;

  // Add path rendering
  sf::CircleShape start_circle(maze.cell_size.x / 2 - 2.0);
  start_circle.setPosition({maze.start_cell.x * maze.cell_size.x + 3, maze.start_cell.y * maze.cell_size.y + 3});
  start_circle.setFillColor(sf::Color(0, 128, 255));

  sf::CircleShape end_circle(maze.cell_size.x / 2 - 2.0);
  end_circle.setPosition({maze.end_cell.x * maze.cell_size.x + 3, maze.end_cell.y * maze.cell_size.y + 3});
  end_circle.setFillColor(sf::Color(255, 128, 0));

  window.draw(start_circle);
  window.draw(end_circle);

  sf::VertexArray line_path(sf::PrimitiveType::LineStrip, maze.path.size());

  for (int i = 0; i < maze.path.size(); i++) {
    sf::Vector2f path_point(maze.path[i].x * maze.cell_size.x + maze.cell_size.x / 2,
                            maze.path[i].y * maze.cell_size.y + maze.cell_size.y / 2);
    line_path[i].position = path_point + sf::Vector2f(1.0, 1.0);
    line_path[i].color = path_color;
  }

  window.draw(line_path);
}

void Maze3DRenderer::set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color) {
  grid_color = new_grid_color;
  path_color = new_path_color;
}
