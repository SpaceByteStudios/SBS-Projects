#include "ui.h"

#include <algorithm>
#include <iostream>

#include "constants.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"

UI::UI(Game& game) : game(game) {
  selectionTexture = LoadTexture("assets/sprites/ui/Selection.png");
  buttonsTexture = LoadTexture("assets/sprites/ui/Buttons.png");
  cursorTexture = LoadTexture("assets/sprites/ui/Cursor.png");
  wateringTexture = LoadTexture("assets/sprites/ui/Watering.png");

  pressedButton.resize(3, false);
  hoversButton.resize(3, false);

  for (int i = 0; i < 3; i++) {
    Vector2 position = {59 * TILE_SIZE, (3.0f + i * (buttonSize + buttonSpacing)) * TILE_SIZE};
    buttonsPos.push_back(position);
    buttonsSize.push_back(Vector2(buttonSize * TILE_SIZE, buttonSize * TILE_SIZE));
  }

  // HideCursor();
}

void UI::updateUI() {
  Vector2 mousePos = GetMousePosition();
  if (game.getState() == GameState::Watering) {
    mousePos += mouseWaterOffset;
  }

  for (int i = 0; i < 3; i++) {
    float sizeX = buttonsSize[i].x;
    float sizeY = buttonsSize[i].y;
    Rectangle bounds = {buttonsPos[i].x - sizeX / 2, buttonsPos[i].y - sizeY / 2, sizeX, sizeY};

    if (CheckCollisionPointRec(mousePos, bounds)) {
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
      if (game.getState() == GameState::Watering) {
        Vector2 mousePos = GetMousePosition();
        SetMousePosition(mousePos.x + mouseWaterOffset.x, mousePos.y + mouseWaterOffset.y);
        playingAnimation = false;
      }

      std::fill(pressedButton.begin(), pressedButton.end(), false);
      game.setState(GameState::Idle);
    } else {
      std::fill(pressedButton.begin(), pressedButton.end(), false);
      pressedButton[i] = true;

      switch (i) {
        case 0:
          game.setState(GameState::Watering);
          SetMousePosition(mousePos.x - mouseWaterOffset.x, mousePos.y - mouseWaterOffset.y);
          break;
        case 1:
          game.setState(GameState::Shopping);
          break;
        case 2:
          game.setState(GameState::Settings);
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
}

void UI::drawButtons() {
  for (int i = 0; i < 3; i++) {
    float tileIndexY = pressedButton[i] ? 1.0f : 0.0f;

    Rectangle source = {i * TILE_SIZE, tileIndexY * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    Vector2 center = {buttonsSize[i].x / 2, buttonsSize[i].y / 2};
    Rectangle dest = {buttonsPos[i].x, buttonsPos[i].y, buttonsSize[i].x, buttonsSize[i].y};

    Color tint = hoversButton[i] ? Color{240, 240, 240, 255} : WHITE;
    tint = pressedButton[i] ? Color{196, 196, 196, 255} : tint;

    DrawTexturePro(buttonsTexture, source, dest, center, 0.0f, tint);
  }
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

  Rectangle mouseSource = {cursorType * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};

  DrawTextureRec(cursorTexture, mouseSource, Vector2Add(mousePos, mouseOffset), WHITE);
}

void UI::playWaterAnimation() {
  wateringFrame = 0;
  playingAnimation = true;
  waterAnimationTimer = 0.0f;
}
