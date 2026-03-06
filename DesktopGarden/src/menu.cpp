#include "menu.h"

#include "constants.h"
#include "game.h"
#include "raylib.h"

Menu::Menu(Game& game, InventorySystem& inventorySystem, PlantsData& plantsData)
    : game(game), inventorySystem(inventorySystem), plantsData(plantsData) {
  menuButtonsTexture = LoadTexture("assets/sprites/ui/MenuButtons.png");

  pressedButton.resize(4, false);
  hoversButton.resize(4, false);

  for (int i = 0; i < 4; i++) {
    Vector2 position = {(52.75f + i * (buttonSize + buttonSpacing)) * TILE_SIZE, 6.75f * TILE_SIZE};
    buttonsPos.push_back(position);
    buttonsSize.push_back(Vector2(buttonSize * TILE_SIZE, buttonSize * TILE_SIZE));
  }
}

void Menu::updateMenu() {
  if (game.getState() != GameState::Menus) {
    std::fill(pressedButton.begin(), pressedButton.end(), false);
    return;
  }

  Vector2 mousePos = GetMousePosition();

  for (int i = 0; i < 4; i++) {
    float sizeX = buttonsSize[i].x;
    float sizeY = buttonsSize[i].y;
    Rectangle bounds = {buttonsPos[i].x - sizeX / 2, buttonsPos[i].y - sizeY / 2, sizeX, sizeY};

    if (CheckCollisionPointRec(mousePos, bounds) && IsCursorOnScreen()) {
      buttonsSize[i] = Vector2(1.15 * buttonSize * TILE_SIZE, 1.15 * buttonSize * TILE_SIZE);
      hoversButton[i] = true;
    } else {
      buttonsSize[i] = Vector2(buttonSize * TILE_SIZE, buttonSize * TILE_SIZE);
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

    if (pressedButton[i]) {
      std::fill(pressedButton.begin(), pressedButton.end(), false);
    } else {
      std::fill(pressedButton.begin(), pressedButton.end(), false);
      pressedButton[i] = true;

      switch (i) {
        case 0:
          CloseWindow();
          break;
        case 1:
          break;
        case 2:
          break;
        case 3:
          break;
      }
    }
  }
}

void Menu::drawMenu() {
  if (game.getState() != GameState::Menus) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    float tileIndexY = pressedButton[i] ? 1.0f : 0.0f;

    Rectangle source = {i * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    Vector2 center = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
    Rectangle dest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

    Color tint = hoversButton[i] ? Color{230, 230, 230, 255} : WHITE;
    tint = pressedButton[i] ? Color{196, 196, 196, 255} : tint;

    DrawTexturePro(menuButtonsTexture, source, dest, center, 0.0f, tint);
  }
}
