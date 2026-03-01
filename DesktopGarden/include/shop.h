#pragma once

#include <vector>

#include "moneySystem.h"
#include "plantsData.h"
#include "raylib.h"

class Game;

struct Shop {
  Shop(Game& game, MoneySystem& moneySystem, PlantsData& plantsData);

  void drawShop();
  void updateShop();

  Vector2 shopPos = {53, 1};

  bool isOpen = false;

  std::vector<bool> hoversButton;

  std::vector<Vector2> buttonsPos;
  std::vector<Vector2> buttonsSize;

  Texture2D shopTexture;
  Texture2D shopButtonsTexture;

  Game& game;
  MoneySystem& moneySystem;
  PlantsData& plantsData;
};
