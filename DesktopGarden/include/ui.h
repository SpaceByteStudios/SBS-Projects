#pragma once

#include "raylib.h"
struct UI {
  UI();

  void drawButtons();
  void drawSelection();

  Texture2D selectionTexture;
};
