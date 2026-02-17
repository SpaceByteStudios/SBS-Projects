#include "maze3d_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>
#include <numbers>
#include <vector>

#include "maze3d.hh"

Maze3DRenderer::Maze3DRenderer(sf::RenderWindow& new_window) : window(new_window) {
}

void Maze3DRenderer::scale(sf::Vector3f& v, const sf::Vector3f& s) {
  v = v.componentWiseMul(s);
}

void Maze3DRenderer::rotate_x(sf::Vector3f& v, float angle) {
  float new_y = v.y * std::cos(angle) - v.z * std::sin(angle);
  float new_z = v.y * std::sin(angle) + v.z * std::cos(angle);
  v = {v.x, new_y, new_z};
}

void Maze3DRenderer::rotate_y(sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) + v.z * std::sin(angle);
  float new_z = v.z * std::cos(angle) - v.x * std::sin(angle);
  v = {new_x, v.y, new_z};
}

void Maze3DRenderer::rotate_z(sf::Vector3f& v, float angle) {
  float new_x = v.x * std::cos(angle) - v.y * std::sin(angle);
  float new_y = v.x * std::sin(angle) + v.y * std::cos(angle);
  v = {new_x, new_y, v.z};
}

void Maze3DRenderer::translate(sf::Vector3f& v, const sf::Vector3f& t) {
  v += t;
}

void Maze3DRenderer::apply_camera(sf::Vector3f& v, Camera& camera) {
  v -= camera.position;

  rotate_z(v, -camera.rotation.z);
  rotate_y(v, -camera.rotation.y);
  rotate_x(v, -camera.rotation.x);
}

void Maze3DRenderer::ortho_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float zoom) {
  float screen_x = (v.x * zoom) + (window_size.x / 2.0f);
  float screen_y = (window_size.y / 2.0f) - (v.y * zoom);
  v = {screen_x, screen_y, v.z};
}

void Maze3DRenderer::pers_projection(sf::Vector3f& v, const sf::Vector2u& window_size, float fov_deg) {
  float aspect_ratio = static_cast<float>(window_size.x) / static_cast<float>(window_size.y);

  float fov_rad = fov_deg * std::numbers::pi_v<float> / 180.0f;
  float fov = 1.0 / std::tan(fov_rad / 2.0f);

  float x_proj = (v.x * fov) / (v.z * aspect_ratio);
  float y_proj = (v.y * fov) / v.z;

  float screen_x = (1.0f - x_proj) * 0.5 * window_size.x;
  float screen_y = (y_proj + 1.0f) * 0.5 * window_size.y;

  v = {screen_x, screen_y, v.z};
}

bool Maze3DRenderer::clipLineToNearPlane(sf::Vector3f& v1, sf::Vector3f& v2, float near) {
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
  // 6 Faces * 2 Lines * 2 Vertices per Line
  const int vertices_per_cell = 6 * 6 * 2;
  sf::VertexArray lines(sf::PrimitiveType::Lines,
                        maze.grid_size.x * maze.grid_size.y * maze.grid_size.z * vertices_per_cell);

  int cell_vertex = 0;

  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  //       Top,   Right, Front, Left,  Back,  Bottom
  // 6bit: 0: +y, 1: +x, 2: +z, 3: -x, 4: -z, 5: -y
  std::vector<std::vector<int>> diagonals = {{2, 7, 6, 3}, {4, 7, 6, 5}, {1, 7, 5, 3},
                                             {0, 3, 2, 1}, {0, 6, 4, 2}, {0, 5, 4, 1}};

  std::vector<std::vector<int>> solid_walls = {
      {2, 3, 3, 7, 7, 6, 6, 2, 2, 7, 6, 3}, {4, 6, 6, 7, 7, 5, 5, 4, 4, 7, 6, 5}, {1, 3, 3, 7, 7, 5, 5, 1, 1, 7, 5, 3},
      {0, 1, 1, 3, 3, 2, 2, 0, 0, 3, 2, 1}, {0, 2, 2, 6, 6, 4, 4, 0, 0, 6, 4, 2}, {0, 1, 1, 5, 5, 4, 4, 0, 0, 5, 4, 1}};

  // For Testing
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
          scale(v, cube_scale);
          rotate_x(v, cube_rotation.x);
          rotate_y(v, cube_rotation.y);
          rotate_z(v, cube_rotation.z);
          v += center;
          translate(v, -center);
          translate(v, cube_position);
        }

        // Each Wall
        int wall_offset = 0;

        for (int i = 0; i < 6; i++) {
          // Skip outer Walls
          // Top, Right, Front, Left, Back, Bottom
          if ((x == 0 && i == 3) || (x == maze.grid_size.x - 1 && i == 1) || (y == 0 && i == 5) ||
              (y == maze.grid_size.y - 1 && i == 0) || (z == 0 && i == 4) || (z == maze.grid_size.z - 1 && i == 2)) {
            continue;
          }

          // sf::Color test_color = sf::Color{0, 0, 255};

          if (!(maze.grid[index].walls_bitmap & (1 << i))) {
            // test_color = sf::Color{255, 0, 0};
            continue;
          }

          // test_color = wall_colors[i];

          const std::vector<int>& wall = solid_walls[i];
          const int lines_amount = wall.size() / 2;

          std::vector<sf::Vector3f> plane_lines(2 * lines_amount);

          // Construct lines of plane
          for (int j = 0; j < lines_amount; j++) {
            plane_lines[2 * j] = vertex_pos[wall[2 * j]];
            plane_lines[2 * j + 1] = vertex_pos[wall[2 * j + 1]];
          }

          // Project and clip all lines
          for (int j = 0; j < lines_amount; j++) {
            bool visible_line = true;

            apply_camera(plane_lines[2 * j], camera);
            apply_camera(plane_lines[2 * j + 1], camera);

            if (project_perspective) {
              visible_line = clipLineToNearPlane(plane_lines[2 * j], plane_lines[2 * j + 1], 0.01f);
              pers_projection(plane_lines[2 * j], window.getSize());
              pers_projection(plane_lines[2 * j + 1], window.getSize());
            } else {
              ortho_projection(plane_lines[2 * j], window.getSize(), ortho_zoom);
              ortho_projection(plane_lines[2 * j + 1], window.getSize(), ortho_zoom);
            }

            // Add lines to VertexArray
            if (visible_line) {
              lines[cell_vertex + wall_offset + 2 * j].position = {plane_lines[2 * j].x, plane_lines[2 * j].y};
              lines[cell_vertex + wall_offset + 2 * j + 1].position = {plane_lines[2 * j + 1].x,
                                                                       plane_lines[2 * j + 1].y};

              lines[cell_vertex + wall_offset + 2 * j].color = grid_color;
              lines[cell_vertex + wall_offset + 2 * j + 1].color = grid_color;
            }
          }

          wall_offset += 2 * lines_amount;
        }

        cell_vertex += vertices_per_cell;
      }
    }
  }

  window.draw(lines);

  sf::VertexArray cube_lines(sf::PrimitiveType::Lines, 24);

  std::vector<sf::Vector3f> vertex_pos = {
      {0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, maze.grid_size.z * maze.cell_size.z},
      {0.0f, maze.grid_size.y * maze.cell_size.y, 0.0f},
      {0.0f, maze.grid_size.y * maze.cell_size.y, maze.grid_size.z * maze.cell_size.z},
      {maze.grid_size.x * maze.cell_size.x, 0.0f, 0.0f},
      {maze.grid_size.x * maze.cell_size.x, 0.0f, maze.grid_size.z * maze.cell_size.z},
      {maze.grid_size.x * maze.cell_size.x, maze.grid_size.y * maze.cell_size.y, 0.0f},
      {maze.grid_size.x * maze.cell_size.x, maze.grid_size.y * maze.cell_size.y, maze.grid_size.z * maze.cell_size.z}};

  std::vector<std::vector<int>> lines_index = {{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
                                               {2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}};

  for (sf::Vector3f& v : vertex_pos) {
    v -= center;
    scale(v, cube_scale);
    rotate_x(v, cube_rotation.x);
    rotate_y(v, cube_rotation.y);
    rotate_z(v, cube_rotation.z);
    v += center;
    translate(v, -center);
    translate(v, cube_position);
  }

  int vertex_index = 0;
  for (std::vector<int>& line : lines_index) {
    sf::Vector3f pos1 = vertex_pos[line[0]];
    sf::Vector3f pos2 = vertex_pos[line[1]];

    apply_camera(pos1, camera);
    apply_camera(pos2, camera);

    bool visible_line = true;

    if (project_perspective) {
      visible_line = clipLineToNearPlane(pos1, pos2, 0.01f);
      pers_projection(pos1, window.getSize());
      pers_projection(pos2, window.getSize());
    } else {
      ortho_projection(pos1, window.getSize(), ortho_zoom);
      ortho_projection(pos2, window.getSize(), ortho_zoom);
    }

    if (visible_line) {
      cube_lines[vertex_index].position = {pos1.x, pos1.y};
      cube_lines[vertex_index + 1].position = {pos2.x, pos2.y};

      cube_lines[vertex_index].color = sf::Color::White;
      cube_lines[vertex_index + 1].color = sf::Color::White;
    }

    vertex_index += 2;
  }

  window.draw(cube_lines);
}

void Maze3DRenderer::draw_path(const Maze3D& maze) {
  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  sf::Vector3f start_circle_pos{maze.cell_size.x / 2.0f, maze.cell_size.y / 2.0f, maze.cell_size.z / 2.0f};
  sf::Vector3f end_circle_pos{(maze.grid_size.x - 1) * maze.cell_size.x + maze.cell_size.x / 2.0f,
                              (maze.grid_size.y - 1) * maze.cell_size.y + maze.cell_size.y / 2.0f,
                              (maze.grid_size.z - 1) * maze.cell_size.z + maze.cell_size.z / 2.0f};

  std::vector<sf::Vector3f> circle_pos{start_circle_pos, end_circle_pos};
  std::vector<sf::Color> circle_colors{sf::Color(0, 128, 255), sf::Color(255, 128, 0)};

  for (int i = 0; i < 2; i++) {
    sf::Vector3f& pos = circle_pos[i];
    sf::Color& circle_color = circle_colors[i];

    scale(pos, cube_scale);
    rotate_x(pos, cube_rotation.x);
    rotate_y(pos, cube_rotation.y);
    rotate_z(pos, cube_rotation.z);
    translate(pos, -center);
    translate(pos, cube_position);

    apply_camera(pos, camera);

    bool visible_circle = true;

    if (project_perspective) {
      visible_circle = clipLineToNearPlane(pos, pos, 0.01f);
      pers_projection(pos, window.getSize());

    } else {
      ortho_projection(pos, window.getSize(), ortho_zoom);
    }

    sf::CircleShape circle(10.0f);
    circle.setPosition({pos.x, pos.y});
    circle.setOrigin({10.0f, 10.0f});
    circle.setFillColor(circle_color);

    if (visible_circle) {
      window.draw(circle);
    }
  }

  if (maze.path.size() < 2) {
    return;
  }

  std::vector<sf::Vector3f> vertices;
  sf::VertexArray line_path(sf::PrimitiveType::Lines, 2 * (maze.path.size() - 1));
  int point_index = 0;

  for (const sf::Vector3i& p : maze.path) {
    sf::Vector3f path_point(p.x * maze.cell_size.x + maze.cell_size.x / 2,
                            p.y * maze.cell_size.y + maze.cell_size.y / 2,
                            p.z * maze.cell_size.z + maze.cell_size.z / 2);

    scale(path_point, cube_scale);
    rotate_x(path_point, cube_rotation.x);
    rotate_y(path_point, cube_rotation.y);
    rotate_z(path_point, cube_rotation.z);
    translate(path_point, -center);
    translate(path_point, cube_position);

    apply_camera(path_point, camera);

    vertices.push_back(path_point);
  }

  for (int i = 0; i < vertices.size() - 1; i++) {
    bool visible_line = true;

    sf::Vector3f path_point1 = vertices[i];
    sf::Vector3f path_point2 = vertices[i + 1];

    if (project_perspective) {
      visible_line = clipLineToNearPlane(path_point1, path_point2, 0.01f);
      pers_projection(path_point1, window.getSize());
      pers_projection(path_point2, window.getSize());

    } else {
      ortho_projection(path_point1, window.getSize(), ortho_zoom);
      ortho_projection(path_point2, window.getSize(), ortho_zoom);
    }

    if (visible_line) {
      line_path[point_index].position = {path_point1.x, path_point1.y};
      line_path[point_index + 1].position = {path_point2.x, path_point2.y};

      line_path[point_index].color = path_color;
      line_path[point_index + 1].color = path_color;
    }

    point_index += 2;
  }

  window.draw(line_path);
}

struct RenderItem {
  float depth; // z after camera transform
  enum Type { Circle, Connection } type;

  // circle_date
  sf::Vector2f screenPos; // after projection
  sf::Color color;

  // connection data
  sf::Vector2f point1; // after projection
  sf::Vector2f point2; // after projection
};

void Maze3DRenderer::draw_graph(const Maze3D& maze) {
  sf::Vector3f center = {maze.cell_size.x * maze.grid_size.x * 0.5f, maze.cell_size.y * maze.grid_size.y * 0.5f,
                         maze.cell_size.z * maze.grid_size.z * 0.5f};

  float outer_circle_radius = maze.cell_size.x / 2.0f - 30.0f;
  float inner_circle_radius = maze.cell_size.x / 2.0f - 27.5f;

  sf::CircleShape outer_circle{outer_circle_radius};
  outer_circle.setOrigin({outer_circle_radius, outer_circle_radius});
  outer_circle.setFillColor(sf::Color::White);

  sf::CircleShape inner_circle{inner_circle_radius};
  inner_circle.setOrigin({inner_circle_radius, inner_circle_radius});

  sf::RectangleShape rect;
  rect.setFillColor(sf::Color::White);

  std::vector<RenderItem> items;
  std::vector<sf::Vector3f> transformed_pos(maze.grid_size.x * maze.grid_size.y * maze.grid_size.z);

  std::vector<sf::Vector3i> dir{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

  float rect_width = 5.0f;

  sf::Vector3f offset = {maze.cell_size.x / 2.0f, maze.cell_size.y / 2.0f, maze.cell_size.z / 2.0f};

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos({x, y, z});

        sf::Vector3f circle_pos{x * maze.cell_size.x, y * maze.cell_size.y, z * maze.cell_size.z};
        circle_pos += offset;

        scale(circle_pos, cube_scale);
        rotate_x(circle_pos, cube_rotation.x);
        rotate_y(circle_pos, cube_rotation.y);
        rotate_z(circle_pos, cube_rotation.z);
        translate(circle_pos, -center);
        translate(circle_pos, cube_position);

        apply_camera(circle_pos, camera);

        transformed_pos[index] = circle_pos;
      }
    }
  }

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int z = 0; z < maze.grid_size.z; z++) {
      for (int x = 0; x < maze.grid_size.x; x++) {
        int index = maze.index_at_pos({x, y, z});

        sf::Vector3f uv{float(x) / maze.grid_size.x, float(y) / maze.grid_size.y, float(z) / maze.grid_size.z};
        uv = {sqrtf(uv.x), sqrtf(uv.y), sqrtf(uv.z)};

        sf::Vector3f circle_pos = transformed_pos[index];

        RenderItem circle;
        circle.type = RenderItem::Type::Circle;
        circle.depth = circle_pos.z;
        circle.color = sf::Color(uv.x * 255, uv.y * 255, uv.z * 255);

        if (project_perspective) {
          if (!clipLineToNearPlane(circle_pos, circle_pos, 0.01f)) {
            continue;
          }

          pers_projection(circle_pos, window.getSize());

        } else {
          ortho_projection(circle_pos, window.getSize(), ortho_zoom);
        }

        circle.screenPos = {circle_pos.x, circle_pos.y};
        items.push_back(circle);

        for (sf::Vector3i& d : dir) {
          sf::Vector3i pos1{x, y, z};
          sf::Vector3i pos2 = pos1 + d;

          if (pos2.x >= maze.grid_size.x || pos2.y >= maze.grid_size.y || pos2.z >= maze.grid_size.z) {
            continue;
          }

          if (maze.is_path_free(pos1, pos2)) {
            sf::Vector3f p1 = transformed_pos[maze.index_at_pos(pos1)];
            sf::Vector3f p2 = transformed_pos[maze.index_at_pos(pos2)];

            RenderItem connection;
            connection.type = RenderItem::Type::Connection;
            connection.depth = (p1.z + p2.z) * 0.5f;

            if (project_perspective) {
              if (clipLineToNearPlane(p1, p2, 0.01f)) {
                pers_projection(p1, window.getSize());
                pers_projection(p2, window.getSize());
              }
            } else {
              ortho_projection(p1, window.getSize(), ortho_zoom);
              ortho_projection(p2, window.getSize(), ortho_zoom);
            }

            connection.point1 = {p1.x, p1.y};
            connection.point2 = {p2.x, p2.y};

            items.push_back(connection);
          }
        }
      }
    }
  }

  std::sort(items.begin(), items.end(), [](const RenderItem& a, const RenderItem& b) {
    return a.depth < b.depth; // farthest first
  });

  for (RenderItem& i : items) {
    if (i.type == RenderItem::Type::Circle) {
      // Draw Circles
      outer_circle.setPosition(i.screenPos);

      inner_circle.setPosition(i.screenPos);
      inner_circle.setFillColor(i.color);

      window.draw(outer_circle);
      window.draw(inner_circle);
    } else {
      sf::Vector2f dir = i.point2 - i.point1;
      float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

      rect.setSize({length, rect_width});
      rect.setOrigin({0, rect_width * 0.5f});
      rect.setPosition(i.point1);

      float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159265f;
      rect.setRotation(sf::degrees(angle));

      window.draw(rect);
    }
  }
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
      ortho_projection(line_pos[2 * i], window.getSize(), ortho_zoom);
      ortho_projection(line_pos[2 * i + 1], window.getSize(), ortho_zoom);
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

void Maze3DRenderer::draw_point(sf::Vector3f& v, const sf::Color& point_color) {
  bool is_visible = true;

  apply_camera(v, camera);

  if (project_perspective) {
    is_visible = clipLineToNearPlane(v, v, 0.01f);
    pers_projection(v, window.getSize());
  } else {
    ortho_projection(v, window.getSize(), ortho_zoom);
  }

  if (is_visible) {
    sf::CircleShape circle{10.0f};
    circle.setOrigin({10.0f, 10.0f});
    circle.setFillColor(point_color);
    circle.setPosition({v.x, v.y});
    window.draw(circle);
  }
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
