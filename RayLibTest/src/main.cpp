#include <iostream>
#include <vector>

#include "raylib.h"

const Vector2 screen_size{500, 500};

const Vector2 grid_size{3, 3};
const Vector2 cell_size{screen_size.x / grid_size.x, screen_size.y / grid_size.y};

std::vector<int> grid;

void render_grid() {
  BeginDrawing();

  ClearBackground(WHITE);

  Color cell_color;

  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = y * grid_size.x + x;

      switch (grid[index]) {
        case 0:
          std::cout << "Drawing Black" << std::endl;
          cell_color = {0, 0, 255};
          break;

        case 1:
          std::cout << "Drawing Green" << std::endl;
          cell_color = {0, 255, 0};
          break;

        case 2:
          std::cout << "Drawing Red" << std::endl;
          cell_color = {255, 0, 0};
          break;
      }

      DrawRectangleRec({x * cell_size.x, y * cell_size.y, cell_size.x, cell_size.y}, cell_color);
    }
  }

  EndDrawing();
}

int main() {
  grid.resize(grid_size.x * grid_size.y);

  grid[5] = 1;
  grid[0] = 2;

  std::cout << cell_size.x << cell_size.y << std::endl;

  InitWindow(screen_size.x, screen_size.y, "Snake Raylib");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    render_grid();
  }

  CloseWindow();
}
