#pragma once

#include <vector>

#include "raylib.h"

struct Shop {
  Shop();

  void drawShop();
  void updateShop();

  Vector2 shopPos = {53, 1};

  bool isOpen = false;

  std::vector<bool> hoversButton;

  std::vector<Vector2> buttonsPos;
  std::vector<Vector2> buttonsSize;

  Texture2D shopTexture;
  Texture2D shopButtonsTexture;
};
