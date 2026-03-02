#include "plant.h"

#include <iostream>

#include "constants.h"
#include "field.h"
#include "plantType.h"
#include "raylib.h"
#include "raymath.h"

Plant::Plant(int plantPosX, int plantPosY, PlantType plantType, Field& field) : field(field) {
  this->plantPosX = plantPosX;
  this->plantPosY = plantPosY;

  this->plantType = plantType;

  growthFactor = 0.85f + (float)rand() / RAND_MAX * 0.3f;
}

void Plant::drawPlant() {
  const int plantOffset = 8;
  int tileIndexX = currentStage;

  Rectangle source = {tileIndexX * TILE_SIZE, 0, plantType.tileSizeX * TILE_SIZE, plantType.tileSizeY * TILE_SIZE};

  Vector2 position = getGlobalPosition() * TILE_SIZE + Vector2(0.0f, -plantOffset);
  Vector2 tile_offset = {-(plantType.tileSizeX - 1) * TILE_SIZE, -(plantType.tileSizeY - 1) * TILE_SIZE};

  Vector2 plantPos = Vector2Add(position, tile_offset);

  DrawTextureRec(plantType.plantTexture, source, plantPos, WHITE);
}

void Plant::updatePlant() {
  if (!field.isCellWatered(plantPosX, plantPosY)) {
    return;
  }

  float delta = GetFrameTime();

  if (currentStage < 5) {
    field.useWater(plantPosX, plantPosY, plantType.waterConsumption * growthFactor * delta);
    growth += plantType.growthSpeed * growthFactor * delta;
  }

  if (growth >= 100.0f) {
    growth = 0.0f;
    currentStage += 1;
  }
}

Vector2 Plant::getGlobalPosition() {
  Vector2 position = Vector2(plantPosX, plantPosY);
  Vector2 field_offset = Vector2(field.fieldPosX, field.fieldPosY);

  return Vector2Add(position, field_offset);
}
