#include "garden.h"

#include <cstdlib>
#include <memory>
#include <vector>

#include "constants.h"
#include "game.h"
#include "plantsData.h"
#include "raylib.h"
#include "raymath.h"

Garden::Garden(Game& game, UI& ui, PlantsData& plantsData) : game(game), ui(ui) {
  tilesColumns = GetScreenWidth() / TILE_SIZE;
  tilesRows = GetScreenHeight() / TILE_SIZE - 1;

  grassTileset = LoadTexture("assets/sprites/tileset/Grass.png");
  fenceTileset = LoadTexture("assets/sprites/tileset/Fences.png");
  propsAtlas = LoadTexture("assets/sprites/Props.png");
  treeAtlas = LoadTexture("assets/sprites/Trees.png");

  propsMap.resize(tilesColumns * tilesRows, -1);
  propsFlipsMap.resize(tilesColumns * tilesRows, -1);

  for (int y = 0; y < tilesRows; y++) {
    for (int x = 0; x < tilesColumns; x++) {
      int index = y * tilesColumns + x;

      if (rand() % 100 >= 10) {
        propsMap[index] = 0;
        propsFlipsMap[index] = rand() % 2;
      }

      if (rand() % 8 == 0) {
        propsMap[index] = rand() % 3 + 1;
      }
    }
  }

  for (int i = 0; i < 6; i++) {
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

    if (game.getState() != GameState::Shopping) {
      if (field->mouseIsOnField()) {
        ui.drawSelection();
      }
    }
  }

  for (const std::unique_ptr<Field>& field : fields) {
    field->drawPlants();
  }

  Rectangle source = {3 * TILE_SIZE * 2, 0, TILE_SIZE * 2, TILE_SIZE * 3};
  Vector2 tree1Position = {2 * TILE_SIZE, 0 * TILE_SIZE};
  Vector2 tree2Position = {56 * TILE_SIZE, 0 * TILE_SIZE};

  DrawTextureRec(treeAtlas, source, tree1Position, WHITE);
  DrawTextureRec(treeAtlas, source, tree2Position, WHITE);
}

void Garden::updateGarden() {
  // LATER: Update all Fields that contain all plants

  if (game.getState() == GameState::Watering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    for (const std::unique_ptr<Field>& field : fields) {
      if (!field->mouseIsOnField()) {
        continue;
      }

      // Play Watering Animation
      // Water that specific Tile

      ui.playWaterAnimation();
      field->waterCell();
    }
  }
}
