#include "garden.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "game.h"
#include "plantsData.h"
#include "raylib.h"
#include "raymath.h"

Garden::Garden(Game& game, UI& ui, PlantsData& plantsData) : game(game), ui(ui), plantsData(plantsData) {
  tilesColumns = GetScreenWidth() / TILE_SIZE;
  tilesRows = GetScreenHeight() / TILE_SIZE - 1;

  grassTileset = LoadTexture("assets/sprites/tileset/Grass.png");
  fenceTileset = LoadTexture("assets/sprites/tileset/Fences.png");
  propsAtlas = LoadTexture("assets/sprites/Props.png");
  treeAtlas = LoadTexture("assets/sprites/Trees.png");

  houseTexture = LoadTexture("assets/sprites/House.png");
  mailTexture = LoadTexture("assets/sprites/Mail.png");

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

  for (int i = 0; i < fieldsAmount; i++) {
    int fieldPosX = 7 + i * 9;
    fields.push_back(std::make_unique<Field>(fieldPosX, 2, 5, 5, plantsData));
  }

  const std::unique_ptr<Field>& frontField = fields[fieldsAmount - 1];

  float posX = frontField->fieldPosX + frontField->fieldWidth / 2.0f - 1.0f;
  float posY = frontField->fieldPosY + frontField->fieldHeight / 2.0f;

  posX += 9.0f;

  fieldButtonPos = {posX, posY};

  upgradeCosts[0] = {{0, 10}};
  upgradeCosts[1] = {{1, 20}};
  upgradeCosts[2] = {{0, 20}, {1, 30}};
  upgradeCosts[3] = {{2, 40}};
  upgradeCosts[4] = {{1, 40}, {2, 50}};
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

  for (int i = 0; i < 7; i++) {
    Vector2 treePos = {(4 + i * 9) * TILE_SIZE, 0 * TILE_SIZE};
    DrawTextureRec(treeAtlas, source, treePos, WHITE);
  }

  ui.drawFieldButton(fieldsAmount - 1, fieldButtonPos.x, fieldButtonPos.y, canUpgrade(fieldsAmount - 1));
}

void Garden::drawGardenIcons() {
  for (const std::unique_ptr<Field>& field : fields) {
    for (std::optional<Plant>& plant : field->plants) {
      if (plant.has_value()) {
        if (!field->isCellWatered(plant->plantPosX, plant->plantPosY)) {
          Vector2 plantPos = plant->getGlobalPosition();
          ui.drawWaterIcon(plantPos.x, plantPos.y);
        }
      }
    }
  }
}

void Garden::drawHouse() {
  DrawTexture(houseTexture, 0.25 * TILE_SIZE, 0, WHITE);
  DrawTexture(mailTexture, 5 * TILE_SIZE, 3.75 * TILE_SIZE, WHITE);
}

void Garden::updateGarden() {
  for (const std::unique_ptr<Field>& field : fields) {
    field->updateField();
  }

  if (game.getState() == GameState::Watering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    for (const std::unique_ptr<Field>& field : fields) {
      if (!field->mouseIsOnField()) {
        continue;
      }

      ui.playWaterAnimation();
      field->waterCell();
    }
  }

  if (game.getState() == GameState::Planting && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    for (const std::unique_ptr<Field>& field : fields) {
      if (!field->mouseIsOnField()) {
        continue;
      }

      int plantID = game.getPlantID();
      int plantCost = plantsData.get(plantID).plantCost;

      if (game.hasEnoughMoney(plantCost) && field->isCellFree()) {
        game.removeMoney(plantCost);
        ui.playMoneyAnimation(-plantCost);

        field->plantSeed(plantID);
      }
    }
  }

  if (game.getState() == GameState::Idle && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    for (const std::unique_ptr<Field>& field : fields) {
      if (!field->mouseIsOnField()) {
        continue;
      }

      Vector2 fieldPos = field->getMouseField();
      int index = fieldPos.y * field->fieldWidth + fieldPos.x;

      std::optional<Plant>& plant = field->plants[index];

      if (plant.has_value() && plant->isGrown()) {
        int plantValue = plant->plantType.plantValue;

        game.addMoney(plantValue);
        ui.playMoneyAnimation(plantValue);
        game.addStock(plant->plantType.id, 1);

        plant.reset();
      }
    }
  }

  if (fieldsAmount >= 6) {
    return;
  }

  if (ui.updateFieldButton(fieldButtonPos.x, fieldButtonPos.y) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (!canUpgrade(fieldsAmount - 1)) {
      return;
    }

    auto levelIt = upgradeCosts.find(fieldsAmount - 1);

    for (const auto& cost : levelIt->second) {
      int resourceId = cost.first;
      int requiredAmount = cost.second;

      game.removeStock(resourceId, requiredAmount);
    }

    int fieldPosX = 7 + fieldsAmount * 9;
    fields.push_back(std::make_unique<Field>(fieldPosX, 2, 5, 5, plantsData));
    fieldsAmount += 1;

    const std::unique_ptr<Field>& frontField = fields[fieldsAmount - 1];

    float posX = frontField->fieldPosX + frontField->fieldWidth / 2.0f - 1.0f;
    float posY = frontField->fieldPosY + frontField->fieldHeight / 2.0f;

    posX += 9.0f;

    fieldButtonPos = {posX, posY};
  }
}

bool Garden::canUpgrade(int level) {
  auto levelIt = upgradeCosts.find(level);

  if (levelIt == upgradeCosts.end()) {
    return false;
  }

  for (const auto& cost : levelIt->second) {
    int resourceId = cost.first;
    int requiredAmount = cost.second;

    if (game.getStock(resourceId) < requiredAmount) {
      return false;
    }
  }

  return true;
}
