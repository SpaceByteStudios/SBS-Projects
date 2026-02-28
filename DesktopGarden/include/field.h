#pragma once

#include <vector>

#include "plant.h"
#include "plantsData.h"
#include "raylib.h"

struct Field {
  Field(int fieldPosX, int fieldPosY, int fieldWidth, int fieldHeight, PlantsData plantsData);

  void drawField();
  void drawPlants();
  bool cellIsWatered(int x, int y);
  Vector2 getWateredTile(int x, int y);
  bool mouseIsOnField();

  const int fieldPosX;
  const int fieldPosY;

  const int fieldWidth;
  const int fieldHeight;

  std::vector<int> isWatered;
  std::vector<Plant> plants;

  Texture2D fieldTileset;
};
