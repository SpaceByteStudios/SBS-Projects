#pragma once

#include "raylib.h"

struct PlantType {
  int id;

  float growthSpeed;
  int growthStages;

  float waterConsumption;

  int plantCost;
  int plantValue;

  int tileSizeX;
  int tileSizeY;
  Texture2D plantTexture;
};
