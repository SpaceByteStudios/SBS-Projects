#pragma once

#include "plantType.h"

struct Field;

struct Plant {
  Plant(int plantPosX, int plantPosY, PlantType plantType, Field& field);

  void drawPlant();

  int plantPosX;
  int plantPosY;

  float growth;
  int currentStage = 0;

  PlantType plantType;
  Field& field;
};
