#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cassert>
#include <codecvt>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

struct Cell {
  int walls_bitmap;
};

sf::Vector2u grid_size = {40, 40};
sf::Vector2f cell_size = {0, 0};

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
  } else {
    return -1;
  }
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

bool are_neighbours(sf::Vector2u cell_pos1, sf::Vector2u cell_pos2) {
  int index_cell1 = index_at_pos(cell_pos1.x, cell_pos1.y);
  int index_cell2 = index_at_pos(cell_pos2.x, cell_pos2.y);

  if (index_cell2 - 1 == index_cell1 || index_cell2 + 1 == index_cell1 ||
      index_cell2 - grid_size.x == index_cell1 ||
      index_cell2 + grid_size.x == index_cell1) {
    return true;
  } else {
    return false;
  }
}

bool is_path_free(std::vector<Cell> &grid, sf::Vector2u cell_pos1,
                  sf::Vector2u cell_pos2) {
  if (!is_inside(cell_pos1.x, cell_pos1.y)) {
    return false;
  }

  if (!is_inside(cell_pos2.x, cell_pos2.y)) {
    return false;
  }

  if (are_neighbours(cell_pos1, cell_pos2)) {
    int index_cell1 = index_at_pos(cell_pos1.x, cell_pos1.y);
    int index_cell2 = index_at_pos(cell_pos2.x, cell_pos2.y);

    sf::Vector2i dir = static_cast<sf::Vector2i>(cell_pos2 - cell_pos1);

    int &cell1_bitmap = grid[index_cell1].walls_bitmap;
    int &cell2_bitmap = grid[index_cell2].walls_bitmap;

    if (dir == sf::Vector2i(0, -1)) {
      if (!(cell1_bitmap & (1 << 0)) && !(cell2_bitmap & (1 << 2))) {
        return true;
      }
    }

    if (dir == sf::Vector2i(1, 0)) {
      if (!(cell1_bitmap & (1 << 1)) && !(cell2_bitmap & (1 << 3))) {
        return true;
      }
    }

    if (dir == sf::Vector2i(0, 1)) {
      if (!(cell1_bitmap & (1 << 2)) && !(cell2_bitmap & (1 << 0))) {
        return true;
      }
    }

    if (dir == sf::Vector2i(-1, 0)) {
      if (!(cell1_bitmap & (1 << 3)) && !(cell2_bitmap & (1 << 1))) {
        return true;
      }
    }

    return false;
  } else {
    return false;
  }
}

void generate_depth_first_maze(std::vector<Cell> &grid) {
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

void generate_maze(std::vector<Cell> &grid) {
  // Just set to all Wall
  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = index_at_pos(x, y);

      grid[index] = Cell();
      grid[index].walls_bitmap = 15;
    }
  }

  generate_depth_first_maze(grid);
}

void solve_maze(std::vector<Cell> &grid, std::vector<sf::Vector2u> &path) {
  path.clear();

  path.push_back(start_cell);

  sf::Vector2u curr_pos = start_cell;

  std::vector<bool> visited_cells(grid_size.x * grid_size.y);
  visited_cells[index_at_pos(curr_pos.x, curr_pos.y)] = true;

  while (curr_pos != end_cell && !path.empty()) {
    std::vector<sf::Vector2u> next_cells = get_next_cells(path.back(), grid);

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = index_at_pos(next_cells[i].x, next_cells[i].y);

      if (!is_path_free(grid, path.back(), next_cells[i])) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
        continue;
      }

      if (visited_cells[next_index]) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
      }
    }

    if (next_cells.size() > 0) {
      sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

      path.push_back(rand_cell);
      visited_cells[index_at_pos(rand_cell.x, rand_cell.y)] = true;

      if (rand_cell == end_cell) {
        return;
      }
    } else {
      path.pop_back();
    }
  }
}

void draw_grid(sf::RenderWindow &window, std::vector<Cell> &grid) {
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

          lines[cell_vertex + point1].position =
              vert_pos[i % 4] + sf::Vector2f(1.0, 0.0);
          lines[cell_vertex + point1 + 1].position =
              vert_pos[(i + 1) % 4] + sf::Vector2f(1.0, 0.0);
          ;

          lines[cell_vertex + point1].color = sf::Color(255, 255, 255);
          lines[cell_vertex + point1 + 1].color = sf::Color(255, 255, 255);
        }
      }

      cell_vertex += 8;
    }
  }

  window.draw(lines);
}

void draw_path(sf::RenderWindow &window, std::vector<sf::Vector2u> &path) {
  sf::CircleShape start_circle(cell_size.x / 2 - 2.0);
  start_circle.setPosition(
      {start_cell.x * cell_size.x + 2, start_cell.y * cell_size.y + 2});
  start_circle.setFillColor(sf::Color(0, 128, 255));

  sf::CircleShape end_circle(cell_size.x / 2 - 2.0);
  end_circle.setPosition(
      {end_cell.x * cell_size.x + 2, end_cell.y * cell_size.y + 2});
  end_circle.setFillColor(sf::Color(255, 128, 0));

  window.draw(start_circle);
  window.draw(end_circle);

  sf::VertexArray line_path(sf::PrimitiveType::LineStrip, path.size());

  for (int i = 0; i < path.size(); i++) {
    sf::Vector2f path_point(path[i].x * cell_size.x + cell_size.x / 2,
                            path[i].y * cell_size.y + cell_size.y / 2);
    line_path[i].position = path_point;
    line_path[i].color = sf::Color(64, 255, 64);
  }

  window.draw(line_path);
}

void do_maze_stuff(sf::RenderWindow &window, std::vector<Cell> &grid,
                   std::vector<sf::Vector2u> &path) {
  window.clear();

  generate_maze(grid);
  solve_maze(grid, path);

  draw_grid(window, grid);
  draw_path(window, path);

  window.display();
}

void animate_generate_maze(sf::RenderWindow &window, std::vector<Cell> &grid){
  //Set every Wall
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
    window.clear();
    
    for(int i = 0; i < maze_stack.size() - 1; i++){
      sf::RectangleShape maze_cell({cell_size.x, cell_size.y});
      maze_cell.setPosition(
        {maze_stack[i].x * cell_size.x, maze_stack[i].y * cell_size.y});

      float col_mult = (float) i / maze_stack.size();
      maze_cell.setFillColor(sf::Color(24 * col_mult, 196 * col_mult, 24 * col_mult));
      window.draw(maze_cell);
    }

    sf::RectangleShape current_cell({cell_size.x, cell_size.y});
    current_cell.setPosition(
      {maze_stack.back().x * cell_size.x, maze_stack.back().y * cell_size.y});
    current_cell.setFillColor(sf::Color(32, 255, 32));
    window.draw(current_cell);

    draw_grid(window, grid);

    window.display();

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

void animate_solve_maze(sf::RenderWindow &window, std::vector<Cell> &grid, std::vector<sf::Vector2u> &path){
  path.clear();

  path.push_back(start_cell);

  sf::Vector2u curr_pos = start_cell;

  std::vector<bool> visited_cells(grid_size.x * grid_size.y);
  visited_cells[index_at_pos(curr_pos.x, curr_pos.y)] = true;

  while (curr_pos != end_cell && !path.empty()) {
    std::vector<sf::Vector2u> next_cells = get_next_cells(path.back(), grid);

    for (int i = 0; i < next_cells.size(); i++) {
      int next_index = index_at_pos(next_cells[i].x, next_cells[i].y);

      if (!is_path_free(grid, path.back(), next_cells[i])) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
        continue;
      }

      if (visited_cells[next_index]) {
        next_cells.erase(next_cells.begin() + i);
        i -= 1;
      }
    }

    if (next_cells.size() > 0) {
      sf::Vector2u rand_cell = next_cells[rand() % next_cells.size()];

      path.push_back(rand_cell);
      visited_cells[index_at_pos(rand_cell.x, rand_cell.y)] = true;

      if (rand_cell == end_cell) {
        return;
      }
    } else {
      path.pop_back();
    }
  }
}

void animated_maze_generation(sf::RenderWindow &window, std::vector<Cell> &grid, std::vector<sf::Vector2u> &path){
  animate_generate_maze(window, grid);

  window.clear();

  draw_grid(window, grid);

  window.display();
}

int main() {
  srand(time(nullptr));

  sf::RenderWindow window(sf::VideoMode({1001, 1001}), "Maze Generator");

  window.setPosition({1920 - (int)window.getSize().x - 50, 50});

  cell_size = {static_cast<float>(window.getSize().x - 1) / grid_size.x,
               static_cast<float>(window.getSize().y - 1) / grid_size.y};

  std::vector<Cell> grid;
  grid.resize(grid_size.x * grid_size.y);

  std::vector<sf::Vector2u> path;

  animated_maze_generation(window, grid, path);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          window.close();
        } else if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
          do_maze_stuff(window, grid, path);
        }
      }
    }
  }
}
