#include "storage.h"

#include <string>

#include "constants.h"
#include "raylib.h"
#include "raymath.h"

Storage::Storage(InventorySystem& inventorySystem, PlantsData& plantsData)
    : inventorySystem(inventorySystem), plantsData(plantsData) {
  storageTexture = LoadTexture("assets/sprites/ui/Storage.png");
  iconsTexture = LoadTexture("assets/sprites/ui/StorageIcons.png");
  infoTexture = LoadTexture("assets/sprites/ui/StorageInfo.png");

  storageFont = LoadFontEx("assets/m6x11.ttf", 128, 0, 0);
  SetTextureFilter(storageFont.texture, TEXTURE_FILTER_POINT);

  stockAmount.resize(4);
  moneyAmount.resize(4);

  stockPositions.resize(4);
  moneyPositions.resize(4);
}

void Storage::updateStorage() {
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      int i = y * 2 + x;

      float iconPosX = 46.35f + x * 6.0f;
      float iconPosY = 2.0f + y * 2.25f;
      Vector2 iconPos = {iconPosX * TILE_SIZE, iconPosY * TILE_SIZE};

      stockAmount[i] = inventorySystem.getStock(i);
      int plantGross = inventorySystem.getLifetimeStock(i) * plantsData.get(i).plantValue;
      int plantCost = inventorySystem.getLifetimeStock(i) * plantsData.get(i).plantCost;
      moneyAmount[i] = plantGross - plantCost;

      std::string stockText = std::to_string(stockAmount[i]);
      std::string moneyText = std::to_string(moneyAmount[i]);

      Vector2 stockLeftPos = iconPos + Vector2(2.25 * TILE_SIZE, -TILE_SIZE / 5);
      Vector2 moneyLeftPos = iconPos + Vector2(4.15 * TILE_SIZE, -TILE_SIZE / 5);

      stockPositions[i] = stockLeftPos;
      moneyPositions[i] = moneyLeftPos;
    }
  }
}

void Storage::drawStorage() {
  DrawTexture(storageTexture, 46 * TILE_SIZE, 1.25 * TILE_SIZE, WHITE);

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      int i = y * 2 + x;

      float iconPosX = 46.35f + x * 6.0f;
      float iconPosY = 2.0f + y * 2.25f;

      Rectangle iconSource = {x * 48.0f, y * 48.0f, 48.0f, 48.0f};
      Vector2 iconPos = {iconPosX * TILE_SIZE, iconPosY * TILE_SIZE};

      DrawTextureRec(iconsTexture, iconSource, iconPos, WHITE);

      Rectangle boxSource = {0.0f, 0.0f, 32.0f, 32.0f};
      Vector2 boxPos = Vector2Add(iconPos, {48.0f, -10.0f});
      Rectangle boxRect = {boxPos.x, boxPos.y, 24.0f, 24.0f};

      DrawTextureRec(infoTexture, boxSource, boxPos, WHITE);

      Rectangle moneySource = {32.0f, 0.0f, 32.0f, 32.0f};
      Vector2 moneyPos = Vector2Add(iconPos, {104.0f, -10.0f});

      DrawTextureRec(infoTexture, moneySource, moneyPos, WHITE);

      Rectangle growthSource = {0.0f, 32.0f + 64.0f * i, 128.0f, 32.0f};
      Vector2 growthPos = Vector2Add(iconPos, {48.0f, 2.0f});

      DrawTextureRec(infoTexture, growthSource, growthPos, WHITE);

      Rectangle waterSource = {0.0f, 64.0f + 64.0f * i, 128.0f, 32.0f};
      Vector2 waterPos = Vector2Add(iconPos, {48.0f, 20.0f});

      DrawTextureRec(infoTexture, waterSource, waterPos, WHITE);

      std::string stockText = std::to_string(stockAmount[i]);
      std::string moneyText = std::to_string(moneyAmount[i]);

      BeginBlendMode(BLEND_ALPHA);
      DrawTextEx(storageFont, stockText.c_str(), stockPositions[i], 16, 0.5f, WHITE);
      DrawTextEx(storageFont, moneyText.c_str(), moneyPositions[i], 16, 0.5f, WHITE);
      EndBlendMode();
    }
  }
}
