#include "maze_renderer.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

#include "maze.hh"

MazeRenderer::MazeRenderer(sf::RenderWindow& new_window, Maze& maze) : window(new_window) {
  maze.cell_size = {static_cast<float>(window.getSize().x - 1) / maze.grid_size.x,
                    static_cast<float>(window.getSize().y - 1) / maze.grid_size.y};
}

void MazeRenderer::draw_grid(const Maze& maze) {
  sf::VertexArray lines(sf::PrimitiveType::Lines, (maze.grid_size.x) * (maze.grid_size.y) * 8);

  int cell_vertex = 0;

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      int index = maze.index_at_pos(sf::Vector2u(x, y));

      std::vector<sf::Vector2f> vert_pos = {{x * maze.cell_size.x, y * maze.cell_size.y},
                                            {(x + 1) * maze.cell_size.x, y * maze.cell_size.y},
                                            {(x + 1) * maze.cell_size.x, (y + 1) * maze.cell_size.y},
                                            {x * maze.cell_size.x, (y + 1) * maze.cell_size.y}};

      for (int i = 0; i < 4; i++) {
        if (maze.grid[index].walls_bitmap & (1 << i)) {
          int point1 = i * 2;

          lines[cell_vertex + point1].position = vert_pos[i % 4] + sf::Vector2f(1.0, 1.0);
          lines[cell_vertex + point1 + 1].position = vert_pos[(i + 1) % 4] + sf::Vector2f(1.0, 1.0);

          // lines[cell_vertex + point1].color = sf::Color(255, 255, 255);
          // lines[cell_vertex + point1 + 1].color = sf::Color(255, 255, 255);

          lines[cell_vertex + point1].color = grid_color;
          lines[cell_vertex + point1 + 1].color = grid_color;
        }
      }

      cell_vertex += 8;
    }
  }

  window.draw(lines);
}

void MazeRenderer::draw_path(const Maze& maze) {
  sf::CircleShape start_circle(maze.cell_size.x / 2 - 2.0);
  start_circle.setPosition({maze.start_cell.x * maze.cell_size.x + 3, maze.start_cell.y * maze.cell_size.y + 3});
  start_circle.setFillColor(sf::Color(0, 128, 255));

  sf::CircleShape end_circle(maze.cell_size.x / 2 - 2.0);
  end_circle.setPosition({maze.end_cell.x * maze.cell_size.x + 3, maze.end_cell.y * maze.cell_size.y + 3});
  end_circle.setFillColor(sf::Color(255, 128, 0));

  // window.draw(start_circle);
  // window.draw(end_circle);

  sf::VertexArray line_path(sf::PrimitiveType::LineStrip, maze.path.size());

  for (int i = 0; i < maze.path.size(); i++) {
    sf::Vector2f path_point(maze.path[i].x * maze.cell_size.x + maze.cell_size.x / 2,
                            maze.path[i].y * maze.cell_size.y + maze.cell_size.y / 2);
    line_path[i].position = path_point + sf::Vector2f(1.0, 1.0);
    // line_path[i].color = sf::Color(64, 255, 64);
    line_path[i].color = path_color;
  }

  window.draw(line_path);
}

void MazeRenderer::draw_graph(const Maze& maze) {
  float outer_circle_radius = maze.cell_size.x / 2.0f - 15.0f;
  float inner_circle_radius = maze.cell_size.x / 2.0f - 19.0f;

  sf::CircleShape outer_circle{outer_circle_radius};
  outer_circle.setOrigin({outer_circle_radius, outer_circle_radius});
  outer_circle.setFillColor(sf::Color::White);

  sf::CircleShape inner_circle{inner_circle_radius};
  inner_circle.setOrigin({inner_circle_radius, inner_circle_radius});

  float rect_width = 5.0f;

  sf::Vector2f offset = {maze.cell_size.x / 2.0f, maze.cell_size.y / 2.0f};

  for (int y = 0; y < maze.grid_size.y; y++) {
    for (int x = 0; x < maze.grid_size.x; x++) {
      sf::Vector2f uv = {float(x) / maze.grid_size.x, float(y) / maze.grid_size.y};
      uv = {sqrtf(uv.x), sqrtf(uv.y)};

      sf::Vector2f circle_pos{x * maze.cell_size.x, y * maze.cell_size.y};

      // Draw Rectangles
      if (maze.is_path_free(sf::Vector2u(x, y), sf::Vector2u(x, y + 1))) {
        sf::RectangleShape rect;

        sf::Vector2f rect_pos = circle_pos + sf::Vector2f{0.0f, maze.cell_size.y / 2.0f};
        rect.setPosition(rect_pos + offset);

        rect.setSize({rect_width, maze.cell_size.y});
        rect.setOrigin(0.5f * rect.getSize());

        rect.setFillColor(sf::Color(225, 225, 225));
        window.draw(rect);
      }

      if (maze.is_path_free(sf::Vector2u(x, y), sf::Vector2u(x + 1, y))) {
        sf::RectangleShape rect;

        sf::Vector2f rect_pos = circle_pos + sf::Vector2f{maze.cell_size.x / 2.0f, 0.0f};
        rect.setPosition(rect_pos + offset);

        rect.setSize({maze.cell_size.x, rect_width});
        rect.setOrigin(0.5f * rect.getSize());

        rect.setFillColor(sf::Color(225, 225, 225));
        window.draw(rect);
      }

      // Draw Circles
      outer_circle.setPosition(circle_pos + offset);

      inner_circle.setPosition(circle_pos + offset);
      sf::Color inner_color = sf::Color(uv.x * 255, uv.y * 255, 0);
      inner_circle.setFillColor(inner_color);

      window.draw(outer_circle);
      window.draw(inner_circle);
    }
  }
}

void MazeRenderer::set_color(const sf::Color& new_grid_color, const sf::Color& new_path_color) {
  grid_color = new_grid_color;
  path_color = new_path_color;
}
