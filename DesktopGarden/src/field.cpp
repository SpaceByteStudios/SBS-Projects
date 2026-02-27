#include "field.h"

#include "raylib.h"
#include "raymath.h"

Field::Field(float tilesWidth, float tilesHeight, int fieldPosX, int fieldPosY) {
  this->tilesWidth = tilesWidth;
  this->tilesHeight = tilesHeight;

  this->fieldPosX = fieldPosX;
  this->fieldPosY = fieldPosY;

  fieldTileset = LoadTexture("assets/sprites/Soil.png");
}

void Field::drawField() {
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

      Rectangle source = {tileIndexX * tilesWidth, tileIndexY * tilesHeight, tilesWidth, tilesHeight};
      Vector2 position = {x * tilesWidth, y * tilesHeight};
      Vector2 offset = {fieldPosX * tilesWidth, fieldPosY * tilesHeight};

      DrawTextureRec(fieldTileset, source, Vector2Add(position, offset), WHITE);
    }
  }
}
