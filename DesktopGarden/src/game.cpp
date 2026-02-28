#include "game.h"

#include <iostream>

#include "plantType.h"
#include "raylib.h"

void DrawDebugGrid(int screenWidth, int screenHeight, int gridSize, Color color) {
  for (int x = 0; x <= screenWidth; x += gridSize) {
    DrawLine(x, 0, x, screenHeight, color);
  }

  for (int y = 0; y <= screenHeight; y += gridSize) {
    DrawLine(0, y, screenWidth, y, color);
  }
}

void Game::init() {
  plantsData.addType(PlantType{0, 100, 1, 6, 1, 1, LoadTexture("assets/sprites/Wheat.png")});
  plantsData.addType(PlantType{1, 100, 1, 6, 1, 1, LoadTexture("assets/sprites/Strawberry.png")});
  plantsData.addType(PlantType{2, 100, 1, 6, 1, 2, LoadTexture("assets/sprites/Sunflower.png")});
  plantsData.addType(PlantType{3, 100, 1, 6, 1, 1, LoadTexture("assets/sprites/Blackberry.png")});
  garden.init(plantsData);
}

void Game::run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);
    garden.drawGarden();
    // DrawDebugGrid(GetMonitorWidth(0), GetMonitorHeight(0), 32, BLACK);
    EndDrawing();
  }
}
