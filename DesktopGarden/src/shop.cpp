#include "shop.h"

#include "constants.h"
#include "raylib.h"

Shop::Shop() {
  shopTexture = LoadTexture("assets/sprites/ui/Shop.png");
  shopButtonsTexture = LoadTexture("assets/sprites/ui/ShopButtons.png");

  pressedButton.resize(4, false);
  hoversButton.resize(4, false);

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      Vector2 position = {(shopPos.x + 1.5f + x * 2.5f) * TILE_SIZE, (shopPos.y + 2.75f + y * 2.25f) * TILE_SIZE};
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

      float tileIndexY = pressedButton[i] ? 1.0f : 0.0f;

      Rectangle source = {i * TILE_SIZE * 1.5f, tileIndexY * TILE_SIZE * 1.5f, TILE_SIZE * 1.5f, TILE_SIZE * 1.5f};
      Vector2 center = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
      Rectangle dest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

      DrawTexturePro(shopButtonsTexture, source, dest, center, 0.0f, WHITE);
    }
  }
}

void Shop::updateShop() {
}
