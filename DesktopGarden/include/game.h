#pragma once

#include <memory>

#include "garden.h"
#include "moneySystem.h"
#include "plantsData.h"
#include "ui.h"

enum class GameState { Idle, Watering, Shopping, Planting, Settings };

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

 private:
  std::unique_ptr<PlantsData> plantsData;
  std::unique_ptr<UI> ui;
  std::unique_ptr<Garden> garden;
  std::unique_ptr<MoneySystem> moneySystem;

  GameState currentState = GameState::Idle;

  int plantingPlantID = 0;
};
