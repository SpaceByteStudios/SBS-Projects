#include "field.h"

#include <memory>

#include "constants.h"
#include "plant.h"
#include "plantType.h"
#include "plantsData.h"
#include "raylib.h"
#include "raymath.h"

Field::Field(int fieldPosX, int fieldPosY, int fieldWidth, int fieldHeight, PlantsData& plantsData)
    : fieldPosX(fieldPosX),
      fieldPosY(fieldPosY),
      fieldWidth(fieldWidth),
      fieldHeight(fieldHeight),
      plantsData(plantsData) {
  fieldTileset = LoadTexture("assets/sprites/Soil.png");

  isWatered.resize(fieldWidth * fieldHeight);
  plants.resize(fieldWidth * fieldHeight);
}

bool Field::cellIsWatered(int x, int y) {
  if (x < 0 || y < 0 || x >= fieldWidth || y >= fieldHeight)
    return false;

  return isWatered[y * fieldWidth + x];
}

Vector2 Field::getWateredTile(int x, int y) {
  if (!isWatered[y * fieldWidth + x]) {
    return Vector2(-1, -1);
  }

  bool up = cellIsWatered(x, y - 1);
  bool down = cellIsWatered(x, y + 1);
  bool left = cellIsWatered(x - 1, y);
  bool right = cellIsWatered(x + 1, y);

  int tileX = 0;
  int tileY = 0;

  // Full surrounded tile
  if (up && down && left && right) {
    tileX = 2;
    tileY = 1;
  }
  // Top edge
  else if (!up && down && left && right) {
    tileX = 2;
    tileY = 0;
  }
  // Bottom edge
  else if (up && !down && left && right) {
    tileX = 2;
    tileY = 2;
  }
  // Left edge
  else if (up && down && !left && right) {
    tileX = 1;
    tileY = 1;
  }
  // Right edge
  else if (up && down && left && !right) {
    tileX = 3;
    tileY = 1;
  }
  // Top Left Corner
  else if (!up && down && !left && right) {
    tileX = 1;
    tileY = 0;
  }
  // Top Right Corner
  else if (!up && down && left && !right) {
    tileX = 3;
    tileY = 0;
  }
  // Bottom Left Corner
  else if (up && !down && !left && right) {
    tileX = 1;
    tileY = 2;
  }
  // Bottom Right Corner
  else if (up && !down && left && !right) {
    tileX = 3;
    tileY = 2;
  }
  // Top & Bottom
  else if (up && down && !left && !right) {
    tileX = 0;
    tileY = 1;
  }
  // Top
  else if (!up && down && !left && !right) {
    tileX = 0;
    tileY = 0;
  }
  // Bottom
  else if (up && !down && !left && !right) {
    tileX = 0;
    tileY = 2;
  }
  // Left & Right
  else if (!up && !down && left && right) {
    tileX = 2;
    tileY = 3;
  }
  // Left
  else if (!up && !down && !left && right) {
    tileX = 1;
    tileY = 3;
  }
  // Right
  else if (!up && !down && left && !right) {
    tileX = 3;
    tileY = 3;
  }
  // Single tile
  else {
    tileX = 0;
    tileY = 3;
  }

  return {(float)tileX, (float)tileY};
}

void Field::drawField() {
  Vector2 offset = {fieldPosX * TILE_SIZE, fieldPosY * TILE_SIZE};

  for (int y = 0; y < fieldHeight; y++) {
    for (int x = 0; x < fieldWidth; x++) {
      int tileIndexX = 2;
      int tileIndexY = 1;

      if (x == 0) {
        tileIndexX = 1;
      }
      if (x == fieldWidth - 1) {
        tileIndexX = 3;
      }

      if (y == 0) {
        tileIndexY = 0;
      }
      if (y == fieldHeight - 1) {
        tileIndexY = 2;
      }

      Rectangle source = {tileIndexX * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
      Vector2 position = {x * TILE_SIZE, y * TILE_SIZE};

      DrawTextureRec(fieldTileset, source, Vector2Add(position, offset), WHITE);
    }
  }

  for (int y = 0; y < fieldHeight; y++) {
    for (int x = 0; x < fieldWidth; x++) {
      Vector2 tileIndex = getWateredTile(x, y);

      if (tileIndex.x != -1) {
        Rectangle source = {(tileIndex.x + 4) * TILE_SIZE, tileIndex.y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Vector2 position = {x * TILE_SIZE, y * TILE_SIZE};

        DrawTextureRec(fieldTileset, source, Vector2Add(position, offset), WHITE);
      }
    }
  }
}

void Field::drawPlants() {
  for (Plant& plant : plants) {
    plant.drawPlant();
  }
}

bool Field::mouseIsOnField() {
  Rectangle bounds = Rectangle(fieldPosX * TILE_SIZE + 2, fieldPosY * TILE_SIZE + 2, fieldWidth * TILE_SIZE - 4,
                               fieldHeight * TILE_SIZE - 4);

  return CheckCollisionPointRec(GetMousePosition(), bounds);
}
