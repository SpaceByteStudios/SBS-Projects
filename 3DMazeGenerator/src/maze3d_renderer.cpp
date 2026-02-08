#include "maze3d_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>
#include <numbers>
#include <vector>

#include "maze3d.hh"

Maze3DRenderer::Maze3DRenderer(sf::RenderWindow& new_window) : window(new_window) {
}

void rotate_x(sf::Vector3f& v, float angle) {
  float new_y = v.y * std::cos(angle) - v.z * std::sin(angle);
  float new_z = v.y * std::sin(angle) + v.z * std::cos(angle);
  v = {v.x, new_y, new_z};
}

void rotate_y(sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) + v.z * std::sin(angle);
  float new_z = v.z * std::cos(angle) - v.x * std::sin(angle);
  v = {new_x, v.y, new_z};
}

void rotate_z(sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) - v.y * std::sin(angle);
  float new_y = v.x * std::sin(angle) + v.y * std::cos(angle);
  v = {new_x, new_y, v.z};
}

void translate(sf::Vector3f& v, const sf::Vector3f& t) {
  v += t;
}

void apply_camera(sf::Vector3f& v, Camera& camera) {
  v -= camera.position;

  rotate_z(v, -camera.rotation.z);
  rotate_y(v, -camera.rotation.y);
  rotate_x(v, -camera.rotation.x);
}

void ortho_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float zoom = 100.0f) {
  float screen_x = (v.x * zoom) + (window_size.x / 2.0f);
  float screen_y = (window_size.y / 2.0f) - (v.y * zoom);
  v = {screen_x, screen_y, v.z};
}

void pers_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float fov_deg = 90.0f) {
  float aspect_ratio = static_cast<float>(window_size.x) / static_cast<float>(window_size.y);

  float fov_rad = fov_deg * std::numbers::pi_v<float> / 180.0f;
  float fov = 1.0 / std::tan(fov_rad / 2.0f);

  float x_proj = (v.x * fov) / (v.z * aspect_ratio);
  float y_proj = (v.y * fov) / v.z;

  float screen_x = (1.0f - x_proj) * 0.5 * window_size.x;
  float screen_y = (y_proj + 1.0f) * 0.5 * window_size.y;

  v = {screen_x, screen_y, v.z};
}

bool clipLineToNearPlane(sf::Vector3f& v1, sf::Vector3f& v2, float near) {
  float zNear = -near;

  bool v1_inside = v1.z <= zNear;
  bool v2_inside = v2.z <= zNear;

  if (v1_inside && v2_inside)
    return true; // keep line

  if (!v1_inside && !v2_inside)
    return false; // discard line

  // Exactly one point is outside → clip
  sf::Vector3f inside = v1_inside ? v1 : v2;
  sf::Vector3f outside = v1_inside ? v2 : v1;

  float t = (zNear - inside.z) / (outside.z - inside.z);
  sf::Vector3f clipped = inside + t * (outside - inside);

  if (v1_inside)
    v2 = clipped;
  else
    v1 = clipped;

  return true;
}

void Maze3DRenderer::draw_grid(const Maze3D& maze) {
  // 6 Faces * 5 Lines * 2 Vertices per Line
  const int vertices_per_cell = 60;
  sf::VertexArray lines(sf::PrimitiveType::Lines,
                        maze.grid_size.x * maze.grid_size.y * maze.grid_size.z * vertices_per_cell);

  int cell_vertex = 0;

  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  // Vertex indeces for walls
  std::vector<std::vector<int>> walls = {{2, 3, 7, 6, 2, 7}, {4, 5, 7, 6, 4, 7}, {1, 3, 7, 5, 1, 7},
                                         {0, 1, 3, 2, 0, 3}, {0, 2, 6, 4, 0, 6}, {0, 1, 5, 4, 0, 5}};

  // Top: Green, Right: Red, Front: Blue, Left: Cyan,  Back: Yellow,  Bottom: Purple
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
          rotate_x(v, cube_rotation.x);
          rotate_y(v, cube_rotation.y);
          rotate_z(v, cube_rotation.z);
          v += center;
          translate(v, -center);
        }

        // Each Wall
        int wall_offset = 0;
        for (int i = 0; i < 6; i++) {
          if (!(maze.grid[index].walls_bitmap & (1 << i))) {
            // continue;
          }

          const std::vector<int>& wall = walls[i];
          const int lines_amount = wall.size() - 1;

          std::vector<bool> visible_lines(lines_amount, true);
          std::vector<sf::Vector3f> plane_lines(2 * lines_amount);

          // Construct lines of plane
          for (int j = 0; j < lines_amount; j++) {
            plane_lines[2 * j] = vertex_pos[wall[j]];
            plane_lines[2 * j + 1] = vertex_pos[wall[j + 1]];
          }

          // Project and clip all lines
          for (int j = 0; j < lines_amount; j++) {
            apply_camera(plane_lines[2 * j], camera);
            apply_camera(plane_lines[2 * j + 1], camera);

            if (project_perspective) {
              visible_lines[j] = clipLineToNearPlane(plane_lines[2 * j], plane_lines[2 * j + 1], 0.01f);
              pers_projection(plane_lines[2 * j], window.getSize());
              pers_projection(plane_lines[2 * j + 1], window.getSize());
            } else {
              ortho_projection(plane_lines[2 * j], window.getSize());
              ortho_projection(plane_lines[2 * j + 1], window.getSize());
            }
          }

          // Add lines to VertexArray
          for (int j = 0; j < lines_amount; j++) {
            if (visible_lines[j]) {
              lines[cell_vertex + wall_offset + 2 * j].position = {plane_lines[2 * j].x, plane_lines[2 * j].y};
              lines[cell_vertex + wall_offset + 2 * j + 1].position = {plane_lines[2 * j + 1].x,
                                                                       plane_lines[2 * j + 1].y};

              lines[cell_vertex + wall_offset + 2 * j].color = wall_colors[i];
              lines[cell_vertex + wall_offset + 2 * j + 1].color = wall_colors[i];
            }
          }

          wall_offset += 2 * lines_amount;
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

void Maze3DRenderer::draw_axis() {
  std::vector<sf::Vector3f> line_pos = {{0.0f, 0.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
                                        {0.0f, 2.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 2.0f}};
  std::vector<bool> visible_axis(3, true);

  for (int i = 0; i < 3; i++) {
    apply_camera(line_pos[2 * i], camera);
    apply_camera(line_pos[2 * i + 1], camera);

    if (project_perspective) {
      visible_axis[i] = clipLineToNearPlane(line_pos[2 * i], line_pos[2 * i + 1], 0.01f);
      pers_projection(line_pos[2 * i], window.getSize());
      pers_projection(line_pos[2 * i + 1], window.getSize());

    } else {
      ortho_projection(line_pos[2 * i], window.getSize());
      ortho_projection(line_pos[2 * i + 1], window.getSize());
    }
  }

  sf::Vertex x_axis[] = {sf::Vertex({line_pos[0].x, line_pos[0].y}, sf::Color::Red),
                         sf::Vertex({line_pos[1].x, line_pos[1].y}, sf::Color::Red)};

  sf::Vertex y_axis[] = {sf::Vertex({line_pos[2].x, line_pos[2].y}, sf::Color::Green),
                         sf::Vertex({line_pos[3].x, line_pos[3].y}, sf::Color::Green)};

  sf::Vertex z_axis[] = {sf::Vertex({line_pos[4].x, line_pos[4].y}, sf::Color::Blue),
                         sf::Vertex({line_pos[5].x, line_pos[5].y}, sf::Color::Blue)};

  if (visible_axis[0]) {
    window.draw(x_axis, 2, sf::PrimitiveType::Lines);
  }

  if (visible_axis[1]) {
    window.draw(y_axis, 2, sf::PrimitiveType::Lines);
  }

  if (visible_axis[2]) {
    window.draw(z_axis, 2, sf::PrimitiveType::Lines);
  }
}

void Maze3DRenderer::draw_lines(std::vector<sf::Vector3f> vertex_pos, const sf::Color& color) {
  sf::VertexArray lines(sf::PrimitiveType::Lines, vertex_pos.size() * 2);

  int lines_amount = vertex_pos.size() - 1;

  std::vector<bool> visible_lines(lines_amount, true);
  std::vector<sf::Vector3f> vertex_lines(2 * lines_amount);

  for (int i = 0; i < vertex_pos.size() - 1; i++) {
    sf::Vector3f& vertex1 = vertex_lines[2 * i];
    sf::Vector3f& vertex2 = vertex_lines[2 * i + 1];

    vertex1 = vertex_pos[i];
    vertex2 = vertex_pos[i + 1];

    apply_camera(vertex1, camera);
    apply_camera(vertex2, camera);

    if (project_perspective) {
      visible_lines[i] = clipLineToNearPlane(vertex1, vertex2, 0.01f);
      pers_projection(vertex1, window.getSize());
      pers_projection(vertex2, window.getSize());
    } else {
      ortho_projection(vertex1, window.getSize());
      ortho_projection(vertex2, window.getSize());
    }
  }

  for (int i = 0; i < vertex_pos.size() - 1; i++) {
    if (visible_lines[i]) {
      lines[2 * i].position = {vertex_lines[2 * i].x, vertex_lines[2 * i].y};
      lines[2 * i + 1].position = {vertex_lines[2 * i + 1].x, vertex_lines[2 * i + 1].y};

      lines[2 * i].color = color;
      lines[2 * i + 1].color = color;
    }
  }

  window.draw(lines);
}

void Maze3DRenderer::set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color) {
  grid_color = new_grid_color;
  path_color = new_path_color;
}

void Maze3DRenderer::move_camera(const sf::Vector3f& movement) {
  camera.position += movement;
}

void Maze3DRenderer::move_camera_rel(const sf::Vector3f& movement) {
  float pitch = camera.rotation.x;
  float yaw = camera.rotation.y;

  sf::Vector3f forward = {std::cos(pitch) * std::sin(yaw), -std::sin(pitch), std::cos(pitch) * std::cos(yaw)};
  sf::Vector3f right = {std::cos(yaw), 0.0f, -std::sin(yaw)};
  sf::Vector3f up = {std::sin(pitch) * std::sin(yaw), std::cos(pitch), std::sin(pitch) * std::cos(yaw)};

  forward = forward.normalized();
  right = right.normalized();
  up = up.normalized();

  camera.position += right * movement.x;
  camera.position += up * movement.y;
  camera.position += forward * movement.z;
}

void Maze3DRenderer::rotate_camera(const sf::Vector3f& rotation) {
  camera.rotation += rotation;
}
