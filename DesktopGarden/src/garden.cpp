#include "garden.h"

#include <cstdlib>
#include <vector>

#include "raylib.h"
#include "raymath.h"

Garden::Garden(float tilesWidth, float tilesHeight, int potsRows, int potsColumns) {
  this->tilesWidth = tilesWidth;
  this->tilesHeight = tilesHeight;

  tilesColumns = GetScreenWidth() / tilesWidth;
  tilesRows = GetScreenHeight() / tilesHeight - 1;

  this->potsRows = potsRows;
  this->potsColumns = potsColumns;

  grass_tileset = LoadTexture("assets/sprites/Tileset.png");
  fence_tileset = LoadTexture("assets/sprites/Fences.png");
  props_atlas = LoadTexture("assets/sprites/Props.png");

  props_map.resize(tilesColumns * tilesRows, -1);
  props_flips_map.resize(tilesColumns * tilesRows, -1);

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      if (rand() % 4 == 0) {
        int index = y * tilesColumns + x;

        props_map[index] = rand() % 4;
        props_flips_map[index] = rand() % 2;
      }
    }
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

      DrawTextureRec(grass_tileset, source, Vector2Add(position, {0, tilesHeight}), WHITE);
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
        Vector2 position = {x * tilesWidth, y * tilesHeight + 24};

        DrawTextureRec(fence_tileset, source, position, WHITE);
      }
    }
  }

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int prop_id = props_map[y * tilesColumns + x];

      if (prop_id != -1) {
        Rectangle source;

        if (props_flips_map[y * tilesColumns + x]) {
          source = {prop_id * tilesWidth + tilesWidth, 0, -tilesWidth, tilesHeight};
        } else {
          source = {prop_id * tilesWidth, 0, tilesWidth, tilesHeight};
        }

        Vector2 position = {x * tilesWidth, y * tilesHeight + 48};

        DrawTextureRec(props_atlas, source, position, WHITE);
      }
    }
  }
}
