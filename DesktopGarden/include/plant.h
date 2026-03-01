#pragma once

#include "plantType.h"

struct Field;

struct Plant {
  Plant(int plantPosX, int plantPosY, PlantType plantType, Field& field);

  void drawPlant();
  void updatePlant();

  int plantPosX;
  int plantPosY;

  float growth = 0;
  int currentStage = 0;

  PlantType plantType;
  Field& field;
};
