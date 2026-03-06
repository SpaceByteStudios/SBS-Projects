#pragma once

#include <memory>

#include "effects.h"
#include "garden.h"
#include "inventorySystem.h"
#include "moneySystem.h"
#include "plantsData.h"
#include "ui.h"

enum class GameState { Idle, Watering, Shopping, Planting, Menus };

class Game {
 public:
  Game();
  void init();
  void run();

  void setState(GameState gameState);
  GameState getState();

  void setPlantID(int plantID);
  int getPlantID();

  void addMoney(int amount);
  void removeMoney(int amount);
  bool hasEnoughMoney(int neededAmount);

  void addStock(int id, int amount);
  void removeStock(int id, int amount);
  int getStock(int id);

 private:
  std::unique_ptr<PlantsData> plantsData;
  std::unique_ptr<UI> ui;
  std::unique_ptr<Garden> garden;
  std::unique_ptr<MoneySystem> moneySystem;
  std::unique_ptr<InventorySystem> inventorySystem;
  std::unique_ptr<Effects> effects;

  GameState currentState = GameState::Idle;

  int plantingPlantID = 0;
};
