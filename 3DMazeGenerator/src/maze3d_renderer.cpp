#include "maze3d_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
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

sf::Vector3f apply_camera(sf::Vector3f v, Camera camera) {
  v = translate(v, -camera.position);
  v = rotate_x(v, -camera.rotation.x);
  v = rotate_y(v, -camera.rotation.y);
  v = rotate_z(v, -camera.rotation.z);

  return v;
}

sf::Vector3f ortho_projection(const sf::Vector3f& v, const sf::Vector2u& window_size) {
  float normalized_x = v.x * (2.0 / window_size.x);
  float normalized_y = v.y * (2.0 / window_size.y);

  float new_x = (normalized_x + 1.0f) * 0.5f * window_size.x;
  float new_y = (1.0f - (normalized_y + 1.0f) * 0.5f) * window_size.y;
  return {new_x, new_y, v.z};
}

sf::Vector3f pers_projection(const sf::Vector3f& v, const sf::Vector2u& window_size, float fov_deg = 120.0f) {
  float fov_rad = fov_deg * 3.14159 / 180.0f;
  float fov = window_size.y / (2.0f * std::tan(fov_rad / 2.0f));

  if (v.z <= 0.1) {
    return {-1.0f, -1.0f, -1.0f};
  }

  float x_proj = (v.x * fov) / v.z;
  float y_proj = (v.y * fov) / v.z;

  float screen_x = (x_proj + 1.0f) * 0.5f * window_size.x;
  float screen_y = (1.0f - (y_proj + 1.0f) * 0.5f) * window_size.y;

  return {screen_x, screen_y, 0.0};
}

void Maze3DRenderer::draw_grid(const Maze3D& maze) {
  const int vertices_per_cell = 60;
  sf::VertexArray lines(sf::PrimitiveType::Lines,
                        maze.grid_size.x * maze.grid_size.y * maze.grid_size.z * vertices_per_cell);

  int cell_vertex = 0;

  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  // Vertex indeces for walls
  std::vector<std::vector<int>> walls = {{2, 3, 7, 6, 2, 7}, {4, 5, 7, 6, 4, 7}, {1, 3, 7, 5, 1, 7},
                                         {0, 1, 3, 2, 0, 3}, {0, 2, 6, 4, 0, 6}, {0, 1, 5, 4, 0, 5}};

  // Top: Green, Front: Red, Right: Blue, Back: Cyan,  Left: Yellow,  Bottom: Purple
  std::vector<sf::Color> wall_colors = {sf::Color{0, 255, 0},   sf::Color{255, 0, 0},   sf::Color{0, 0, 255},
                                        sf::Color{0, 255, 255}, sf::Color{255, 255, 0}, sf::Color{255, 0, 255}};

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

        for (sf::Vector3f& v : vertex_pos) {
          v -= center;
          v = rotate_x(v, cube_rotation.x);
          v = rotate_y(v, cube_rotation.y);
          v = rotate_z(v, cube_rotation.z);
          v += center;
          v = translate(v, -center);
          v = translate(v, {0.0f, 0.0f, 0.0f});

          v = apply_camera(v, camera);

          v = pers_projection(v, window.getSize(), 100.0f);
        }

        // Each Wall
        int vertex_offset = 0;
        for (int i = 0; i < 6; i++) {
          if (maze.grid[index].walls_bitmap & (1 << i)) {
            for (int j = 0; j < 5; j++) {
              int point = i * 6 + j * 2;

              sf::Vector3f pos1 = vertex_pos[walls[i][j]];
              sf::Vector3f pos2 = vertex_pos[walls[i][j + 1]];

              if (pos1.x != -1.0f && pos2.x != -1.0f) {
                lines[cell_vertex + vertex_offset].position = {pos1.x, pos1.y};
                lines[cell_vertex + vertex_offset + 1].position = {pos2.x, pos2.y};

                lines[cell_vertex + vertex_offset].color = wall_colors[i];
                lines[cell_vertex + vertex_offset + 1].color = wall_colors[i];
              }

              vertex_offset += 2;
            }
          }
        }

        cell_vertex += vertices_per_cell;
      }
    }
  }

  window.draw(lines);

  sf::Vector3f center_pos = pers_projection(apply_camera({0.0f, 0.0f, 0.0f}, camera), window.getSize());
  sf::Vector3f x_pos = pers_projection(apply_camera({200.0f, 0.0f, 0.0f}, camera), window.getSize());
  sf::Vector3f y_pos = pers_projection(apply_camera({0.0f, 200.0f, 0.0f}, camera), window.getSize());
  sf::Vector3f z_pos = pers_projection(apply_camera({0.0f, 0.0f, 200.0f}, camera), window.getSize());

  sf::Vertex x_axis[] = {sf::Vertex({center_pos.x, center_pos.y}, sf::Color::Red),
                         sf::Vertex({x_pos.x, x_pos.y}, sf::Color::Red)};

  sf::Vertex y_axis[] = {sf::Vertex({center_pos.x, center_pos.y}, sf::Color::Green),
                         sf::Vertex({y_pos.x, y_pos.y}, sf::Color::Green)};

  sf::Vertex z_axis[] = {sf::Vertex({center_pos.x, center_pos.y}, sf::Color::Blue),
                         sf::Vertex({z_pos.x, z_pos.y}, sf::Color::Blue)};

  window.draw(x_axis, 2, sf::PrimitiveType::Lines);
  window.draw(y_axis, 2, sf::PrimitiveType::Lines);
  window.draw(z_axis, 2, sf::PrimitiveType::Lines);

  std::cout << "Camera pos: " << camera.position.x << " " << camera.position.y << camera.position.z << std::endl;
  std::cout << "Camera rot: " << camera.rotation.x << " " << camera.rotation.y << camera.rotation.z << std::endl;
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
