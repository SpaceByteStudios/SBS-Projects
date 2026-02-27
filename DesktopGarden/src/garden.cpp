#include "garden.h"

#include <cstdlib>
#include <vector>

#include "raylib.h"
#include "raymath.h"

Garden::Garden(float tilesWidth, float tilesHeight) {
  this->tilesWidth = tilesWidth;
  this->tilesHeight = tilesHeight;

  tilesColumns = GetScreenWidth() / tilesWidth;
  tilesRows = GetScreenHeight() / tilesHeight - 1;

  grassTileset = LoadTexture("assets/sprites/Grass.png");
  fenceTileset = LoadTexture("assets/sprites/Fences.png");
  propsAtlas = LoadTexture("assets/sprites/Props.png");

  propsMap.resize(tilesColumns * tilesRows, -1);
  propsFlipsMap.resize(tilesColumns * tilesRows, -1);

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      if (rand() % 4 == 0) {
        int index = y * tilesColumns + x;

        propsMap[index] = rand() % 4;
        propsFlipsMap[index] = rand() % 2;
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    int fieldPosX = 2 + i * 7;
    fields.push_back(Field{tilesWidth, tilesHeight, fieldPosX, 2});
  }
}

void Garden::drawGarden() {
  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int tileIndexX = 1;
      int tileIndexY = 1;

      if (y == 0) {
        tileIndexY = 0;
      }

      Rectangle source = {tileIndexX * tilesWidth, tileIndexY * tilesHeight, tilesWidth, tilesHeight};
      Vector2 position = {x * tilesWidth, y * tilesHeight};

      DrawTextureRec(grassTileset, source, Vector2Add(position, {0, tilesHeight}), WHITE);
    }
  }

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int tileIndexX = -1;
      int tileIndexY = -1;

      if (y == 0) {
        tileIndexY = 0;
        tileIndexX = 2;
      }

      if (tileIndexX != -1 && tileIndexY != -1) {
        Rectangle source = {tileIndexX * tilesWidth, tileIndexY * tilesHeight, tilesWidth, tilesHeight};
        Vector2 position = {x * tilesWidth, y * tilesHeight + fenceOffset};

        DrawTextureRec(fenceTileset, source, position, WHITE);
      }
    }
  }

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int prop_id = propsMap[y * tilesColumns + x];

      if (prop_id != -1) {
        Rectangle source;

        if (propsFlipsMap[y * tilesColumns + x]) {
          source = {prop_id * tilesWidth + tilesWidth, 0, -tilesWidth, tilesHeight};
        } else {
          source = {prop_id * tilesWidth, 0, tilesWidth, tilesHeight};
        }

        Vector2 position = {x * tilesWidth, y * tilesHeight + propsOffset};

        DrawTextureRec(propsAtlas, source, position, WHITE);
      }
    }
  }

  for (Field& field : fields) {
    field.drawField();
  }
}
