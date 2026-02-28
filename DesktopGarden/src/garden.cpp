#include "garden.h"

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <vector>

#include "constants.h"
#include "plantsData.h"
#include "raylib.h"
#include "raymath.h"

Garden::Garden(UI& ui, PlantsData& plantsData) : ui(ui) {
  tilesColumns = GetScreenWidth() / TILE_SIZE;
  tilesRows = GetScreenHeight() / TILE_SIZE - 1;

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

  for (int i = 0; i < 1; i++) {
    int fieldPosX = 5 + i * 9;
    fields.push_back(std::make_unique<Field>(fieldPosX, 2, 5, 5, plantsData));
  }

  // fields.push_back(std::make_unique<Field>(5, 2, 5, 5, plantsData));
}

void Garden::drawGarden() {
  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int tileIndexX = 1;
      int tileIndexY = 1;

      if (y == 0) {
        tileIndexY = 0;
      }

      Rectangle source = {tileIndexX * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
      Vector2 position = {x * TILE_SIZE, y * TILE_SIZE};

      DrawTextureRec(grassTileset, source, Vector2Add(position, {0, TILE_SIZE}), WHITE);
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
        Rectangle source = {tileIndexX * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
        Vector2 position = {x * TILE_SIZE, y * TILE_SIZE + fenceOffset};

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
          source = {prop_id * TILE_SIZE + TILE_SIZE, 0, -TILE_SIZE, TILE_SIZE};
        } else {
          source = {prop_id * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
        }

        Vector2 position = {x * TILE_SIZE, y * TILE_SIZE + propsOffset};

        DrawTextureRec(propsAtlas, source, position, WHITE);
      }
    }
  }

  for (const std::unique_ptr<Field>& field : fields) {
    field->drawField();

    if (field->mouseIsOnField()) {
      ui.drawSelection();
    }
  }

  for (const std::unique_ptr<Field>& field : fields) {
    field->drawPlants();
  }
}
