#pragma once

#include <vector>

#include "raylib.h"

struct Field {
  Field(float tilesWidth, float tilesHeight, int fieldPosX, int fieldPosY);

  void drawField();

  float tilesWidth;
  float tilesHeight;

  int fieldPosX;
  int fieldPosY;

  const int fieldWidth = 5;
  const int fieldHeight = 5;

  std::vector<int> isWatered;

  Texture2D fieldTileset;
};
