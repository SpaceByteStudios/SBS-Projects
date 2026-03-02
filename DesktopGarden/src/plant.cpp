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
  waterFactor = 0.85f + (float)rand() / RAND_MAX * 0.3f;
}

void Plant::drawPlant() {
  const int plantOffset = 12;

  int tileIndexX = currentStage;

  float width = plantType.tileSizeX * TILE_SIZE;
  float height = plantType.tileSizeY * TILE_SIZE;

  Rectangle plantSource = {tileIndexX * TILE_SIZE, 0, width, height};

  float scale = 1.25f;

  Vector2 worldPos = getGlobalPosition() * TILE_SIZE;

  worldPos.x += TILE_SIZE / 2.0f;
  worldPos.y += TILE_SIZE;

  if (currentStage > 0) {
    worldPos.y -= plantOffset;
  }

  Rectangle dest = {worldPos.x, worldPos.y, width * scale, height * scale};
  Vector2 origin = {(width * scale) / 2.0f, height * scale};

  DrawTexturePro(plantType.plantTexture, plantSource, dest, origin, 0.0f, WHITE);
}

void Plant::updatePlant() {
  if (!field.isCellWatered(plantPosX, plantPosY)) {
    return;
  }

  float delta = GetFrameTime();

  if (currentStage < 5) {
    field.useWater(plantPosX, plantPosY, plantType.waterConsumption * waterFactor * delta);
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

bool Plant::isGrown() {
  return currentStage >= 5;
}
