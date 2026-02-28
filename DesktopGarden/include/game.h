#pragma once

#include <memory>

#include "garden.h"
#include "plantsData.h"
#include "ui.h"

enum class GameState { Idle, Watering, Shopping, Settings };

class Game {
 public:
  Game();
  void init();
  void run();

  void setState(GameState gameState);
  GameState getState();

 private:
  std::unique_ptr<PlantsData> plantsData;
  std::unique_ptr<UI> ui;
  std::unique_ptr<Garden> garden;

  GameState currentState = GameState::Idle;
};
