#include <cstdlib>
#include <ctime>
#include <vector>

#include "raylib.h"
#include "raymath.h"

const Vector2 screen_size{500, 500};

const Vector2 grid_size{25, 25};
const Vector2 cell_size{screen_size.x / grid_size.x, screen_size.y / grid_size.y};

// Head is at index zero
std::vector<Vector2> snake_pos;
Vector2 snake_dir{1, 0};

Vector2 apple_pos;

std::vector<Color> cell_colors{{0, 0, 0, 255}, {0, 255, 0, 255}, {255, 0, 0, 255}};
std::vector<int> grid;

void handle_input() {
  if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && !Vector2Equals(snake_dir, {0, -1})) {
    snake_dir = {0, 1};
  }

  if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && !Vector2Equals(snake_dir, {0, 1})) {
    snake_dir = {0, -1};
  }

  if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && !Vector2Equals(snake_dir, {1, 0})) {
    snake_dir = {-1, 0};
  }

  if ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && !Vector2Equals(snake_dir, {-1, 0})) {
    snake_dir = {1, 0};
  }
}

void update_game() {
  std::vector<Vector2> new_snake_pos(snake_pos);
  new_snake_pos[0] = Vector2Add(snake_pos[0], snake_dir);

  for (int i = 1; i < snake_pos.size(); i++) {
    Vector2 move_dir = snake_pos[i - 1] - snake_pos[i];
    new_snake_pos[i] = Vector2Add(snake_pos[i], move_dir);
  }

  if (Vector2Equals(snake_pos[0], apple_pos)) {
    new_snake_pos.push_back(snake_pos[snake_pos.size() - 1]);
    apple_pos = Vector2(rand() % int(grid_size.x), rand() % int(grid_size.y));
  }

  snake_pos = new_snake_pos;
}

bool check_game_over() {
  if (snake_pos[0].x < 0 || snake_pos[0].x >= grid_size.x || snake_pos[0].y < 0 || snake_pos[0].y >= grid_size.y) {
    return true;
  }

  for (int i = 0; i < snake_pos.size(); i++) {
    for (int j = i + 1; j < snake_pos.size(); j++) {
      if (Vector2Equals(snake_pos[i], snake_pos[j])) {
        return true;
      }
    }
  }

  return false;
}

void render_grid() {
  BeginDrawing();

  ClearBackground(BLACK);

  for (int& g : grid) {
    g = 0;
  }

  int apple_index = apple_pos.y * grid_size.x + apple_pos.x;
  grid[apple_index] = 2;

  for (Vector2& s_pos : snake_pos) {
    int snake_index = s_pos.y * grid_size.y + s_pos.x;
    grid[snake_index] = 1;
  }

  for (int y = 0; y < grid_size.y; y++) {
    for (int x = 0; x < grid_size.x; x++) {
      int index = y * grid_size.x + x;

      Rectangle rect{x * cell_size.x, y * cell_size.y, cell_size.x - 1, cell_size.y - 1};

      if (grid[index] != 0) {
        DrawRectangleRec(rect, cell_colors[grid[index]]);
      }
    }
  }

  EndDrawing();
}

int main() {
  srand(time(nullptr));

  grid.resize(grid_size.x * grid_size.y);

  apple_pos = Vector2(rand() % int(grid_size.x), rand() % int(grid_size.y));
  snake_pos = {{0, 0}};

  InitWindow(screen_size.x, screen_size.y, "Snake Raylib");
  SetTargetFPS(10);

  bool running = true;

  while (running && !WindowShouldClose()) {
    handle_input();
    update_game();

    if (check_game_over()) {
      running = false;
    } else {
      render_grid();
    }
  }

  CloseWindow();
}
