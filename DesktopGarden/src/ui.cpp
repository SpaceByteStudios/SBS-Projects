#include "ui.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "constants.h"
#include "field.h"
#include "game.h"
#include "inventorySystem.h"
#include "menu.h"
#include "moneySystem.h"
#include "plantsData.h"
#include "raylib.h"
#include "raymath.h"
#include "shop.h"

UI::UI(Game& game, MoneySystem& moneySystem, PlantsData& plantsData, InventorySystem& inventorySystem)
    : game(game), moneySystem(moneySystem) {
  shop = std::make_unique<Shop>(game, moneySystem, plantsData);
  menu = std::make_unique<Menu>(game, inventorySystem, plantsData);

  selectionTexture = LoadTexture("assets/sprites/ui/Selection.png");
  buttonsTexture = LoadTexture("assets/sprites/ui/Buttons.png");
  cursorTexture = LoadTexture("assets/sprites/ui/Cursor.png");

  wateringTexture = LoadTexture("assets/sprites/ui/Watering.png");
  seedsTexture = LoadTexture("assets/sprites/crop/Seeds.png");
  moneyTexture = LoadTexture("assets/sprites/ui/Money.png");
  waterIconTexture = LoadTexture("assets/sprites/ui/WaterIcon.png");
  fieldButtonsTexture = LoadTexture("assets/sprites/ui/FieldButtons.png");

  moneyDisplayFont = LoadFontEx("assets/m6x11.ttf", 128, 0, 0);
  SetTextureFilter(moneyDisplayFont.texture, TEXTURE_FILTER_POINT);
  moneyDisplayTexture = LoadTexture("assets/sprites/ui/MoneyDisplay.png");

  pressedButton.resize(3, false);
  hoversButton.resize(3, false);

  for (int i = 0; i < 3; i++) {
    Vector2 position = {59 * TILE_SIZE, (3.0f + i * (buttonSize + buttonSpacing)) * TILE_SIZE};
    buttonsPos.push_back(position);
    buttonsSize.push_back(Vector2(buttonSize * TILE_SIZE, buttonSize * TILE_SIZE));
  }

  HideCursor();
}

void UI::updateUI() {
  Vector2 mousePos = GetMousePosition();
  if (game.getState() == GameState::Watering) {
    mousePos += mouseWaterOffset;
  }

  if (game.getState() == GameState::Planting) {
    shop->isOpen = false;
  }

  for (int i = 0; i < 3; i++) {
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

  for (int i = 0; i < 3; i++) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      break;
    }

    if (!hoversButton[i]) {
      continue;
    }

    if (pressedButton[i]) {
      shop->isOpen = false;

      if (game.getState() == GameState::Watering) {
        Vector2 mousePos = GetMousePosition();
        SetMousePosition(mousePos.x + mouseWaterOffset.x, mousePos.y + mouseWaterOffset.y);
        playingAnimation = false;
      }

      std::fill(pressedButton.begin(), pressedButton.end(), false);
      game.setState(GameState::Idle);
    } else {
      shop->isOpen = false;

      std::fill(pressedButton.begin(), pressedButton.end(), false);
      pressedButton[i] = true;

      switch (i) {
        case 0:
          game.setState(GameState::Watering);
          SetMousePosition(mousePos.x - mouseWaterOffset.x, mousePos.y - mouseWaterOffset.y);
          break;
        case 1:
          if (game.getState() == GameState::Watering) {
            Vector2 mousePos = GetMousePosition();
            SetMousePosition(mousePos.x + mouseWaterOffset.x, mousePos.y + mouseWaterOffset.y);
            playingAnimation = false;
          }
          game.setState(GameState::Shopping);
          shop->isOpen = true;
          break;
        case 2:
          if (game.getState() == GameState::Watering) {
            Vector2 mousePos = GetMousePosition();
            SetMousePosition(mousePos.x + mouseWaterOffset.x, mousePos.y + mouseWaterOffset.y);
            playingAnimation = false;
          }
          game.setState(GameState::Menus);
          break;
      }
    }
  }

  if (playingAnimation) {
    waterAnimationTimer += GetFrameTime();

    if (waterAnimationTimer >= waterFrameTime) {
      waterAnimationTimer = 0.0f;
      wateringFrame += 1;
    }

    if (wateringFrame >= 7) {
      wateringFrame = 0;
      playingAnimation = false;
      waterAnimationTimer = 0.0f;
    }
  }

  shop->updateShop();
  menu->updateMenu();

  for (MoneyParticle& particle : moneyParticles) {
    particle.updateParticle();
  }

  for (auto it = moneyParticles.begin(); it != moneyParticles.end();) {
    it->updateParticle();

    if (it->lifetimeOver()) {
      it = moneyParticles.erase(it);
    } else {
      ++it;
    }
  }
}

void UI::drawShop() {
  shop->drawShop();
}

void UI::drawButtons() {
  for (int i = 0; i < 3; i++) {
    if (i == 1 && game.getState() == GameState::Planting) {
      float tileIndexY = pressedButton[i] ? 1.0f : 0.0f;

      Rectangle seedSource = {game.getPlantID() * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
      Rectangle buttonSource = {3 * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
      Vector2 buttonCenter = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
      Rectangle buttonDest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

      Color tint = hoversButton[i] ? Color{230, 230, 230, 255} : WHITE;
      tint = pressedButton[i] ? Color{196, 196, 196, 255} : tint;

      DrawTexturePro(buttonsTexture, buttonSource, buttonDest, buttonCenter, 0.0f, tint);
      DrawTexturePro(seedsTexture, seedSource, buttonDest, buttonCenter, 0.0f, tint);

      continue;
    }

    float tileIndexY = pressedButton[i] ? 1.0f : 0.0f;

    Rectangle source = {i * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    Vector2 center = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
    Rectangle dest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

    Color tint = hoversButton[i] ? Color{230, 230, 230, 255} : WHITE;
    tint = pressedButton[i] ? Color{196, 196, 196, 255} : tint;

    DrawTexturePro(buttonsTexture, source, dest, center, 0.0f, tint);
  }

  menu->drawMenu();
}

void UI::drawSelection() {
  Vector2 mousePos = GetMousePosition();

  int mouseX = mousePos.x / TILE_SIZE;
  int mouseY = mousePos.y / TILE_SIZE;

  Rectangle source = {0 * TILE_SIZE * 3, 0, TILE_SIZE * 3, TILE_SIZE * 3};
  Vector2 position = {mouseX * TILE_SIZE, mouseY * TILE_SIZE};
  Vector2 offset = {-TILE_SIZE, -TILE_SIZE};

  DrawTextureRec(selectionTexture, source, Vector2Add(position, offset), WHITE);
}

void UI::drawWaterIcon(int posX, int posY) {
  float iconX = posX;
  float iconY = posY;

  iconY += -1.0f;
  iconY += 0.1f * std::sin(GetTime());

  DrawTexture(waterIconTexture, iconX * TILE_SIZE, iconY * TILE_SIZE, WHITE);
}

void UI::drawCursor() {
  if (!IsCursorOnScreen()) {
    return;
  }

  Vector2 mousePos = GetMousePosition();

  cursorType = 0;
  Vector2 mouseOffset = mouseNormalOffset;

  if (game.getState() == GameState::Watering) {
    cursorType = 1;
    mouseOffset = mouseWaterOffset;

    Rectangle waterSource = {wateringFrame * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
    Rectangle waterDest = {mousePos.x + waterOffset.x, mousePos.y + waterOffset.y, TILE_SIZE * 2, TILE_SIZE * 2};
    Vector2 waterOrigin = {TILE_SIZE / 2, TILE_SIZE / 2};

    DrawTexturePro(wateringTexture, waterSource, waterDest, waterOrigin, 0.0f, WHITE);
  }

  if (game.getState() == GameState::Planting) {
    cursorType = 1;

    Rectangle seedSource = {game.getPlantID() * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
    Rectangle seedDest = {mousePos.x, mousePos.y, TILE_SIZE * 2, TILE_SIZE * 2};
    Vector2 seedOrigin = {TILE_SIZE, TILE_SIZE};

    DrawTexturePro(seedsTexture, seedSource, seedDest, seedOrigin, 0.0f, WHITE);
  }

  Rectangle mouseSource = {cursorType * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};

  DrawTextureRec(cursorTexture, mouseSource, Vector2Add(mousePos, mouseOffset), WHITE);
}

void UI::drawMoney() {
  Vector2 displayPos = {28 * TILE_SIZE, 7 * TILE_SIZE};

  DrawTexture(moneyDisplayTexture, displayPos.x, displayPos.y, WHITE);

  std::string moneyText = std::to_string(moneySystem.money);

  for (int i = moneyText.length() - 3; i > 0; i -= 3) {
    moneyText.insert(i, ".");
  }

  Vector2 rightPos = displayPos + Vector2(4 * TILE_SIZE, TILE_SIZE / 4);

  Vector2 textSize = MeasureTextEx(moneyDisplayFont, moneyText.c_str(), 20, 0.5f);
  Vector2 textPos = Vector2(rightPos.x - textSize.x - 7, rightPos.y);

  BeginBlendMode(BLEND_ALPHA);
  DrawTextEx(moneyDisplayFont, moneyText.c_str(), textPos, 20, 0.5f, WHITE);
  EndBlendMode();

  for (MoneyParticle& particle : moneyParticles) {
    particle.drawParticle();
  }
}

void UI::playMoneyAnimation(int amount) {
  Vector2 mousePos = GetMousePosition();

  MoneyParticle particle = {moneyTexture, moneyDisplayFont, 3.0f};

  particle.number = amount;
  particle.moneyPos = mousePos + Vector2{0.0f, -36.0f};

  moneyParticles.push_back(particle);
}

void UI::playWaterAnimation() {
  wateringFrame = 0;
  playingAnimation = true;
  waterAnimationTimer = 0.0f;
}

MoneyParticle::MoneyParticle(Texture2D& texture, Font& font, float lifetime)
    : moneyTexture(&texture), moneyFont(&font) {
  totalLifetime = lifetime;
}

void MoneyParticle::drawParticle() {
  std::string moneyText = std::to_string(number);
  Vector2 textSize = MeasureTextEx(*moneyFont, moneyText.c_str(), 12.0f, 1.0f);
  Vector2 textOffset = {-textSize.x / 2.0f, 0.0f};

  Rectangle moneySource = {(currentFrame % 6) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
  Rectangle moneyDest = {moneyPos.x + textSize.x + 4.0f, moneyPos.y + textSize.y / 3.0f, TILE_SIZE, TILE_SIZE};
  Vector2 moneyCenter = {TILE_SIZE / 2, TILE_SIZE / 2};

  BeginBlendMode(BLEND_ALPHA);
  DrawTextEx(*moneyFont, moneyText.c_str(), Vector2Add(moneyPos, textOffset), 12.0f, 1.0f, WHITE);
  EndBlendMode();

  DrawTexturePro(*moneyTexture, moneySource, moneyDest, moneyCenter, 0.0f, WHITE);
}

void MoneyParticle::updateParticle() {
  float delta = GetFrameTime();

  timer += delta;
  frameTimer += delta;

  if (frameTimer >= 0.1f) {
    frameTimer = 0.0f;
    currentFrame += 1;
  }

  moneyPos += Vector2{0.0f, -7.5f} * delta;
}

bool MoneyParticle::lifetimeOver() {
  return timer >= totalLifetime;
}

bool UI::updateFieldButton(int posX, int posY) {
  Vector2 mousePos = GetMousePosition();
  Rectangle bounds = {posX * TILE_SIZE, posY * TILE_SIZE, TILE_SIZE * 3, TILE_SIZE};

  if (CheckCollisionPointRec(mousePos, bounds) && IsCursorOnScreen()) {
    hoversFieldButton = true;
  } else {
    hoversFieldButton = false;
  }

  return hoversFieldButton;
}

void UI::drawFieldButton(int fieldButton, int posX, int posY, bool isActive) {
  Rectangle source = {0, fieldButton * TILE_SIZE, TILE_SIZE * 3, TILE_SIZE};
  Vector2 position = {posX * TILE_SIZE, posY * TILE_SIZE};

  Color tint = hoversFieldButton ? Color{230, 230, 230, 255} : WHITE;
  tint = isActive ? tint : Color{128, 128, 128, 255};

  DrawTextureRec(fieldButtonsTexture, source, position, tint);
}
