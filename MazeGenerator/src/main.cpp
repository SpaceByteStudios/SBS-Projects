#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

#include "cell.hh"

sf::Vector2u grid_size = {20, 20};
sf::Vector2f cell_size = {0, 0};

std::vector<Cell> grid;

sf::Vector2u start_cell = {0, 0};
sf::Vector2u end_cell = {grid_size.x - 1, grid_size.y - 1};

int index_at_pos(int x, int y) { return y * grid_size.x + x; }

void generate_maze(std::vector<Cell> &grid) {
  std::vector<bool> visited_cells(grid_size.x * grid_size.y);

  // Just set randomly
  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(x, y);

      grid[index] = Cell();
      grid[index].walls_bitmap = rand() % 16;
    }
  }

  std::vector<int> maze_stack(0);
  maze_stack.push_back(rand() % grid.size());

  while (!maze_stack.empty()) {
    maze_stack.pop_back();
  }
}

void draw_grid(sf::RenderWindow &window, std::vector<Cell> &grid) {
  sf::RectangleShape start_rect(cell_size);
  start_rect.setPosition(
      {start_cell.x * cell_size.x, start_cell.y * cell_size.y});
  start_rect.setFillColor(sf::Color(0, 128, 255));

  sf::RectangleShape end_rect(cell_size);
  end_rect.setPosition({end_cell.x * cell_size.x, end_cell.y * cell_size.y});
  end_rect.setFillColor(sf::Color(255, 128, 0));

  sf::VertexArray lines(sf::PrimitiveType::Lines,
                        (grid_size.x + 1) * (grid_size.y + 1) * 8);

  int cell_vertex = 0;

  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(x, y);

      std::vector<sf::Vector2f> vert_pos = {
          {x * cell_size.x, y * cell_size.y},
          {(x + 1) * cell_size.x, y * cell_size.y},
          {(x + 1) * cell_size.x, (y + 1) * cell_size.y},
          {x * cell_size.x, (y + 1) * cell_size.y}};

      std::vector<sf::Color> debug_col = {
          {255, 0, 0}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}};

      for (int i = 0; i < 4; i++) {
        if (grid[index].walls_bitmap & (1 << i)) {
          int point1 = i * 2;

          lines[cell_vertex + point1].position = vert_pos[i % 4];
          lines[cell_vertex + point1 + 1].position = vert_pos[(i + 1) % 4];

          lines[cell_vertex + point1].color = sf::Color(255, 255, 255);
          lines[cell_vertex + point1 + 1].color = sf::Color(255, 255, 255);

          lines[cell_vertex + point1].color = debug_col[i];
          lines[cell_vertex + point1 + 1].color = debug_col[i];
        }
      }

      cell_vertex += 8;
    }
  }

  window.draw(start_rect);
  window.draw(end_rect);

  window.draw(lines);
}

int main() {
  sf::RenderWindow window(sf::VideoMode({500, 500}), "Maze Generator");

  window.setPosition({1920 - (int)window.getSize().x - 50, 50});

  cell_size = {static_cast<float>(window.getSize().x) / grid_size.x,
               static_cast<float>(window.getSize().y) / grid_size.y};

  grid.resize(grid_size.x * grid_size.y);

  generate_maze(grid);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    window.clear();

    draw_grid(window, grid);

    window.display();
  }
}
