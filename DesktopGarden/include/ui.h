#pragma once

#include <vector>

#include "raylib.h"

class Game;

struct UI {
  UI(Game& game);

  void updateUI();

  void drawButtons();
  void drawSelection();
  void drawCursor();

  float buttonSize = 1.6f;
  float buttonSpacing = 0.25f;

  std::vector<bool> pressedButton;
  std::vector<bool> hoversButton;

  std::vector<Vector2> buttonsPos;
  std::vector<Vector2> buttonsSize;

  Game& game;

  Texture2D selectionTexture;
  Texture2D buttonsTexture;
  Texture2D cursorTexture;
  Texture2D wateringTexture;

  int cursorType = 0;

  const Vector2 mouseNormalOffset = {-6, -4};
  const Vector2 mouseWaterOffset = {16, -32};
  const Vector2 waterOffset = {6, -46};
};
