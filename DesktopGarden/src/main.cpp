#include "garden.h"
#include "raylib.h"
#include "win_overlay.h"

void DrawDebugGrid(int screenWidth, int screenHeight, int gridSize, Color color) {
  for (int x = 0; x <= screenWidth; x += gridSize) {
    DrawLine(x, 0, x, screenHeight, color);
  }

  for (int y = 0; y <= screenHeight; y += gridSize) {
    DrawLine(0, y, screenWidth, y, color);
  }
}

int main() {
  // SetTraceLogLevel(LOG_NONE);

  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  InitWindow(1, 1, "Desktop Garden");
  SetWindowState(FLAG_WINDOW_UNDECORATED);
  SetWindowState(FLAG_WINDOW_TOPMOST);

  const int screenWidth = GetMonitorWidth(0);
  const int screenHeight = GetMonitorHeight(0);

  const int windowHeight = 256;
  const int windowPosHeight = screenHeight - windowHeight;

  SetWindowSize(screenWidth, windowHeight);
  SetWindowPosition(0, windowPosHeight);

  MakeWindowClickThrough(GetWindowHandle());
  SetTargetFPS(60);

  float tileWidth = 32;
  float tileHeight = 32;

  Garden garden{32.0f, 32.0f, 1, 1};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);
    garden.drawGarden();
    DrawDebugGrid(screenWidth, windowHeight, 32, BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
