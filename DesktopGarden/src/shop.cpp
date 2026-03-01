#include "shop.h"

#include "constants.h"
#include "game.h"
#include "moneySystem.h"
#include "plantType.h"
#include "plantsData.h"
#include "raylib.h"

Shop::Shop(Game& game, MoneySystem& moneySystem, PlantsData& plantsData)
    : game(game), moneySystem(moneySystem), plantsData(plantsData) {
  shopTexture = LoadTexture("assets/sprites/ui/Shop.png");
  shopButtonsTexture = LoadTexture("assets/sprites/ui/ShopButtons.png");

  hoversButton.resize(4, false);

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      Vector2 position = {(shopPos.x + 1.25f + x * 2.75f) * TILE_SIZE, (shopPos.y + 2.75f + y * 2.5f) * TILE_SIZE};
      buttonsPos.push_back(position);
      buttonsSize.push_back(Vector2(1.5f * TILE_SIZE, 1.5f * TILE_SIZE));
    }
  }
}

void Shop::drawShop() {
  if (!isOpen) {
    return;
  }

  Rectangle source = {0 * TILE_SIZE, 0 * TILE_SIZE, 8 * TILE_SIZE, 8 * TILE_SIZE};
  Vector2 position = {shopPos.x * TILE_SIZE, shopPos.y * TILE_SIZE};

  DrawTextureRec(shopTexture, source, position, WHITE);

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      int i = y * 2 + x;

      PlantType plantType = *plantsData.get(i);
      int cost = plantType.plantCost;

      Rectangle source = {i * TILE_SIZE * 1.5f, 0, TILE_SIZE * 1.5f, TILE_SIZE * 1.5f};
      Vector2 center = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
      Rectangle dest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

      Color tint = hoversButton[i] ? Color{230, 230, 230, 255} : WHITE;
      tint = moneySystem.hasEnoughMoney(cost) ? tint : Color{128, 128, 128, 255};

      DrawTexturePro(shopButtonsTexture, source, dest, center, 0.0f, tint);
    }
  }
}

void Shop::updateShop() {
  if (!isOpen) {
    return;
  }

  Vector2 mousePos = GetMousePosition();

  for (int i = 0; i < 4; i++) {
    PlantType plantType = *plantsData.get(i);
    int cost = plantType.plantCost;

    if (!moneySystem.hasEnoughMoney(cost)) {
      continue;
    }

    float sizeX = buttonsSize[i].x;
    float sizeY = buttonsSize[i].y;
    Rectangle bounds = {buttonsPos[i].x - sizeX / 2, buttonsPos[i].y - sizeY / 2, sizeX, sizeY};

    if (CheckCollisionPointRec(mousePos, bounds)) {
      buttonsSize[i] = Vector2(1.15 * 1.5f * TILE_SIZE, 1.15 * 1.5f * TILE_SIZE);
      hoversButton[i] = true;
    } else {
      buttonsSize[i] = Vector2(1.5f * TILE_SIZE, 1.5f * TILE_SIZE);
      hoversButton[i] = false;
    }
  }

  for (int i = 0; i < 4; i++) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      break;
    }

    if (!hoversButton[i]) {
      continue;
    }

    game.setPlantID(i);
    game.setState(GameState::Planting);
  }
}
