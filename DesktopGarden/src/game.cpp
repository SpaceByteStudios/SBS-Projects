#include "game.h"

#include <memory>

#include "audioPlayer.h"
#include "effects.h"
#include "garden.h"
#include "inventorySystem.h"
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
  inventorySystem = std::make_unique<InventorySystem>();
  effects = std::make_unique<Effects>();
  plantsData = std::make_unique<PlantsData>();
  audioPlayer = std::make_unique<AudioPlayer>("assets/music");
  ui = std::make_unique<UI>(*this, *moneySystem, *plantsData, *inventorySystem);
  garden = std::make_unique<Garden>(*this, *ui, *plantsData);
}

void Game::init() {
  plantsData->addType(PlantType{0, 2.0f, 0.9f, 5, 8, 1, 1, LoadTexture("assets/sprites/crop/Strawberry.png")});
  plantsData->addType(PlantType{1, 3.0f, 1.6f, 12, 20, 1, 1, LoadTexture("assets/sprites/crop/Blackberry.png")});
  plantsData->addType(PlantType{2, 1.0f, 0.6f, 21, 37, 1, 1, LoadTexture("assets/sprites/crop/Wheat.png")});
  plantsData->addType(PlantType{3, 0.5f, 0.2f, 50, 125, 1, 2, LoadTexture("assets/sprites/crop/Sunflower.png")});

  plantsData->addType(PlantType{0, 200.0f, 0.9f, 5, 8, 1, 1, LoadTexture("assets/sprites/crop/Strawberry.png")});
  plantsData->addType(PlantType{1, 200.0f, 1.6f, 12, 20, 1, 1, LoadTexture("assets/sprites/crop/Blackberry.png")});
  plantsData->addType(PlantType{2, 200.0f, 0.4f, 21, 37, 1, 1, LoadTexture("assets/sprites/crop/Wheat.png")});
}

void Game::run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLANK);
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);

    garden->updateGarden();
    ui->updateUI();
    effects->updateEffects();

    garden->drawGarden();
    garden->drawHouse();
    effects->drawClouds();
    garden->drawGardenIcons();

    ui->drawShop();
    ui->drawButtons();
    ui->drawMoney();
    ui->drawCursor();

    audioPlayer->updateMusic();

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

void Game::addStock(int id, int amount) {
  inventorySystem->addStock(id, amount);
}

void Game::removeStock(int id, int amount) {
  inventorySystem->removeStock(id, amount);
}

int Game::getStock(int id) {
  return inventorySystem->getStock(id);
}

void Game::updateVolume(float mVolume, float sVolume) {
  audioPlayer->updateVolume(mVolume, sVolume);
}
