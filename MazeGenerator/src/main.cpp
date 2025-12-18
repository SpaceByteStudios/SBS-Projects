#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "cell.hh"

sf::Vector2u grid_size = {100, 100};
sf::Vector2f cell_size = {0, 0};

std::vector<Cell> grid;

sf::Vector2u start_cell = {0, 0};
sf::Vector2u end_cell = {grid_size.x - 1, grid_size.y - 1};

bool is_inside(int x, int y) {
  if (x < 0 || x >= grid_size.x || y < 0 || y >= grid_size.y) {
    return false;
  }

  return true;
}

int index_at_pos(int x, int y) {
  if (is_inside(x, y)) {
    return y * grid_size.x + x;
  }

  return -1;
}

std::vector<sf::Vector2u> get_next_cells(sf::Vector2u cell_pos,
                                         std::vector<Cell> &grid) {

  std::vector<sf::Vector2u> cells_pos = {{cell_pos.x, cell_pos.y - 1},
                                         {cell_pos.x, cell_pos.y + 1},
                                         {cell_pos.x - 1, cell_pos.y},
                                         {cell_pos.x + 1, cell_pos.y}};

  std::vector<sf::Vector2u> next_cells;

  for (int i = 0; i < 4; i++) {
    if (is_inside(cells_pos[i].x, cells_pos[i].y)) {
      next_cells.push_back({cells_pos[i].x, cells_pos[i].y});
    }
  }

  return next_cells;
}

void generate_maze(sf::RenderWindow &window, std::vector<Cell> &grid) {
  // Just set to all Wall
  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(x, y);

      grid[index] = Cell();
      grid[index].walls_bitmap = 15;
    }
  }

  // Randomized Depth First Search
  std::vector<sf::Vector2u> maze_stack(0);
  std::vector<bool> visited_cells(grid_size.x * grid_size.y);

  sf::Vector2u cell_pos = {rand() % grid_size.x, rand() % grid_size.y};

  maze_stack.push_back(cell_pos);
  visited_cells[index_at_pos(cell_pos.x, cell_pos.y)] = true;

  while (!maze_stack.empty()) {
    std::vector<sf::Vector2u> next_cells =
        get_next_cells(maze_stack.back(), grid);

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = index_at_pos(next_cells[i].x, next_cells[i].y);

      if (visited_cells[next_index]) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
      }
    }

    if (next_cells.size() > 0) {
      sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

      int rand_cell_index = index_at_pos(rand_cell.x, rand_cell.y);
      int curr_cell_index =
          index_at_pos(maze_stack.back().x, maze_stack.back().y);

      sf::Vector2i dir =
          static_cast<sf::Vector2i>(rand_cell - maze_stack.back());

      int &rand_cell_bitmap = grid[rand_cell_index].walls_bitmap;
      int &curr_cell_bitmap = grid[curr_cell_index].walls_bitmap;

      if (dir == sf::Vector2i(0, -1)) {
        curr_cell_bitmap &= ~(1 << 0);
        rand_cell_bitmap &= ~(1 << 2);
      }

      if (dir == sf::Vector2i(1, 0)) {
        curr_cell_bitmap &= ~(1 << 1);
        rand_cell_bitmap &= ~(1 << 3);
      }

      if (dir == sf::Vector2i(0, 1)) {
        curr_cell_bitmap &= ~(1 << 2);
        rand_cell_bitmap &= ~(1 << 0);
      }

      if (dir == sf::Vector2i(-1, 0)) {
        curr_cell_bitmap &= ~(1 << 3);
        rand_cell_bitmap &= ~(1 << 1);
      }

      maze_stack.push_back(rand_cell);
      visited_cells[index_at_pos(rand_cell.x, rand_cell.y)] = true;
    } else {
      maze_stack.pop_back();
    }
  }
}

void draw_grid(sf::RenderWindow &window, std::vector<Cell> &grid) {
  sf::CircleShape start_circle(cell_size.x / 2);
  start_circle.setPosition(
      {start_cell.x * cell_size.x, start_cell.y * cell_size.y});
  start_circle.setFillColor(sf::Color(0, 128, 255));

  sf::CircleShape end_circle(cell_size.x / 2);
  end_circle.setPosition({end_cell.x * cell_size.x, end_cell.y * cell_size.y});
  end_circle.setFillColor(sf::Color(255, 128, 0));

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

      for (int i = 0; i < 4; i++) {
        if (grid[index].walls_bitmap & (1 << i)) {
          int point1 = i * 2;

          lines[cell_vertex + point1].position = vert_pos[i % 4];
          lines[cell_vertex + point1 + 1].position = vert_pos[(i + 1) % 4];

          lines[cell_vertex + point1].color = sf::Color(255, 255, 255);
          lines[cell_vertex + point1 + 1].color = sf::Color(255, 255, 255);
        }
      }

      cell_vertex += 8;
    }
  }

  window.draw(start_circle);
  window.draw(end_circle);

  window.draw(lines);
}

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Maze Generator");

  window.setPosition({1920 - (int)window.getSize().x - 50, 50});

  cell_size = {static_cast<float>(window.getSize().x) / grid_size.x,
               static_cast<float>(window.getSize().y) / grid_size.y};

  grid.resize(grid_size.x * grid_size.y);

  generate_maze(window, grid);

  draw_grid(window, grid);

  window.display();

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // window.clear();

    // draw_grid(window, grid);

    // window.display();
  }
}
