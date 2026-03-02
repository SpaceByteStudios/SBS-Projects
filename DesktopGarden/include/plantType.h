#pragma once

#include "raylib.h"

struct PlantType {
  int id;

  // 100.0 to reach next Stage
  // value/Second
  float growthSpeed;
  float waterConsumption;

  int plantCost;
  int plantValue;

  int tileSizeX;
  int tileSizeY;
  Texture2D plantTexture;
};
