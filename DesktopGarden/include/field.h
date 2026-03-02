#pragma once

#include <memory.h>

#include <optional>
#include <vector>

#include "plant.h"
#include "plantsData.h"
#include "raylib.h"

struct Field {
  Field(int fieldPosX, int fieldPosY, int fieldWidth, int fieldHeight, PlantsData& plantsData);

  void drawField();
  void drawPlants();

  void updateField();

  Vector2 getMouseField();

  void waterCell();
  void plantSeed(int plantID);

  void useWater(int posX, int posY, float waterAmount);
  bool isCellWatered(int x, int y);

  Vector2 getWateredTile(int x, int y);
  bool mouseIsOnField();
  bool isCellFree();

  const int fieldPosX;
  const int fieldPosY;

  const int fieldWidth;
  const int fieldHeight;

  std::vector<float> waterAmount;
  std::vector<std::optional<Plant>> plants;

  Texture2D fieldTileset;

  PlantsData& plantsData;
};
