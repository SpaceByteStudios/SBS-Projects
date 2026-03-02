#include "game.h"

#include <iostream>
#include <memory>

#include "garden.h"
#include "moneySystem.h"
#include "plantType.h"
#include "raylib.h"
#include "ui.h"

void DrawDebugGrid(int screenWidth, int screenHeight, int gridSize, Color color) {
  for (int x = 0; x <= screenWidth; x += gridSize) {
    DrawLine(x, 0, x, screenHeight, color);
  }

  for (int y = 0; y <= screenHeight; y += gridSize) {
    DrawLine(0, y, screenWidth, y, color);
  }
}

Game::Game() {
  moneySystem = std::make_unique<MoneySystem>();
  plantsData = std::make_unique<PlantsData>();
  ui = std::make_unique<UI>(*this, *moneySystem, *plantsData);
  garden = std::make_unique<Garden>(*this, *ui, *plantsData);
}

void Game::init() {
  plantsData->addType(PlantType{0, 2.0f, 0.9f, 5, 8, 1, 1, LoadTexture("assets/sprites/crop/Strawberry.png")});

  plantsData->addType(PlantType{0, 100.0f, 0.9f, 5, 8, 1, 1, LoadTexture("assets/sprites/crop/Strawberry.png")});

  plantsData->addType(PlantType{1, 3.0f, 2.3f, 12, 20, 1, 1, LoadTexture("assets/sprites/crop/Blackberry.png")});
  plantsData->addType(PlantType{2, 1.0f, 1.3f, 20, 37, 1, 1, LoadTexture("assets/sprites/crop/Wheat.png")});
  plantsData->addType(PlantType{3, 0.5f, 0.4f, 45, 100, 1, 2, LoadTexture("assets/sprites/crop/Sunflower.png")});

  plantsData->addType(PlantType{3, 100.0f, 0.4f, 45, 100, 1, 2, LoadTexture("assets/sprites/crop/Sunflower.png")});
}

void Game::run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);

    garden->updateGarden();
    ui->updateUI();

    garden->drawGarden();
    garden->drawGardenIcons();

    ui->drawShop();
    ui->drawButtons();
    ui->drawMoney();
    ui->drawCursor();

    // DrawDebugGrid(GetMonitorWidth(0), GetMonitorHeight(0), 32, BLACK);

    EndBlendMode();
    EndDrawing();
  }
}

void Game::setState(GameState gameState) {
  currentState = gameState;
}

GameState Game::getState() {
  return currentState;
}

void Game::setPlantID(int plantID) {
  plantingPlantID = plantID;
}

int Game::getPlantID() {
  return plantingPlantID;
}

void Game::addMoney(int amount) {
  moneySystem->addMoney(amount);
}

void Game::removeMoney(int amount) {
  moneySystem->removeMoney(amount);
}

bool Game::hasEnoughMoney(int neededAmount) {
  return moneySystem->hasEnoughMoney(neededAmount);
}
