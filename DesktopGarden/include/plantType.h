#pragma once

#include "raylib.h"

struct PlantType {
  int id;

  float growthAmount;
  float growthSpeed;
  int growthStages;

  int tileSizeX;
  int tileSizeY;
  Texture2D plantTexture;
};
