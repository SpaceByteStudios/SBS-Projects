#pragma once

#include <vector>

#include "raylib.h"

class Game;

struct UI {
  UI(Game& game);

  void drawButtons();
  void drawSelection();
  void drawCursor();

  float buttonSize = 1.5f;
  float buttonSpacing = 0.25f;

  std::vector<Vector2> buttonPositions;
  std::vector<Vector2> buttonSizes;

  Game& game;

  Texture2D selectionTexture;
  Texture2D buttonsTexture;
  Texture2D cursorTexture;

  int cursorType;
};
