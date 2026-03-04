#pragma once

#include "plantType.h"
#include "raylib.h"

struct Field;

struct Plant {
  Plant(int plantPosX, int plantPosY, PlantType plantType, Field& field);

  void drawPlant();
  void updatePlant();

  Vector2 getGlobalPosition();
  bool isGrown();

  int plantPosX;
  int plantPosY;

  float growth = 0;
  int currentStage = 0;

  float growthFactor = 1.0f;
  float waterFactor = 1.0f;

  PlantType plantType;
  Field& field;

  Shader plantShader;
  int timeLoc;
};
