#pragma once

#include <vector>

#include "inventorySystem.h"
#include "plantsData.h"
#include "raylib.h"

struct Storage {
  Storage(InventorySystem& inventorySystem, PlantsData& plantsData);

  void updateStorage();
  void drawStorage();

  std::vector<int> stockAmount;
  std::vector<int> moneyAmount;

  std::vector<Vector2> stockPositions;
  std::vector<Vector2> moneyPositions;

  Texture2D storageTexture;
  Texture2D iconsTexture;
  Texture2D infoTexture;

  Font storageFont;

  InventorySystem& inventorySystem;
  PlantsData& plantsData;
};
