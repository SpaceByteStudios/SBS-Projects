#include "plant.h"

#include "constants.h"
#include "field.h"
#include "plantType.h"
#include "raylib.h"
#include "raymath.h"

Plant::Plant(int plantPosX, int plantPosY, PlantType plantType, Field* field) {
  this->plantPosX = plantPosX;
  this->plantPosY = plantPosY;

  this->plantType = plantType;
  this->field = field;
}

void Plant::drawPlant() {
  const int plantOffset = 10;
  int tileIndexX = currentStage;

  if (!field || currentStage >= plantType.growthStages) {
    return;
  }

  Rectangle source = {tileIndexX * TILE_SIZE, 0, plantType.tileSizeX * TILE_SIZE, plantType.tileSizeY * TILE_SIZE};
  Vector2 position = {plantPosX * TILE_SIZE, plantPosY * TILE_SIZE - plantOffset};
  Vector2 field_offset = {field->fieldPosX * TILE_SIZE, field->fieldPosY * TILE_SIZE};
  Vector2 tile_offset = {-(plantType.tileSizeX - 1) * TILE_SIZE, -(plantType.tileSizeY - 1) * TILE_SIZE};
  Vector2 plantPos = Vector2Add(position, Vector2Add(field_offset, tile_offset));

  DrawTextureRec(plantType.plantTexture, source, plantPos, WHITE);
}
