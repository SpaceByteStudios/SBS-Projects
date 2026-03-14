#pragma once

#include <memory>

#include "inventorySystem.h"
#include "plantsData.h"
#include "raylib.h"
#include "settings.h"
#include "storage.h"

class Game;

struct Menu {
  Menu(Game& game, InventorySystem& inventorySystem, PlantsData& plantsData);

  void updateMenu();
  void drawMenu();

  Game& game;
  InventorySystem& inventorySystem;
  PlantsData& plantsData;

  float buttonSize = 1.25f;
  float buttonSpacing = 0.25f;

  bool showUI = true;

  std::vector<bool> pressedButton;
  std::vector<bool> hoversButton;

  std::vector<Vector2> buttonsPos;
  std::vector<Vector2> buttonsSize;

  Texture2D menuButtonsTexture;

  std::unique_ptr<Storage> storage;
  std::unique_ptr<Settings> settings;

  Sound buttonSFX;
};
