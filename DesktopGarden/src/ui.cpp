#include "ui.h"

#include <iostream>

#include "constants.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"

UI::UI(Game& game) : game(game) {
  selectionTexture = LoadTexture("assets/sprites/Selection.png");
  buttonsTexture = LoadTexture("assets/sprites/Buttons.png");
  cursorTexture = LoadTexture("assets/sprites/Cursor.png");

  for (int i = 0; i < 3; i++) {
    Vector2 position = {59 * TILE_SIZE, (3.0f + i * (buttonSize + buttonSpacing)) * TILE_SIZE};
    buttonPositions.push_back(position);
    buttonSizes.push_back(Vector2(buttonSize * TILE_SIZE, buttonSize * TILE_SIZE));
  }

  HideCursor();
}

void UI::drawButtons() {
  for (int i = 0; i < 3; i++) {
    Rectangle source = {i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
    Vector2 center = {buttonSizes[i].x / 2, buttonSizes[i].y / 2};
    Rectangle dest = {buttonPositions[i].x, buttonPositions[i].y, buttonSizes[i].x, buttonSizes[i].y};

    DrawTexturePro(buttonsTexture, source, dest, center, 0.0f, WHITE);
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
  Vector2 offset = {-6, -4};
  Rectangle source = {cursorType * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};

  DrawTextureRec(cursorTexture, source, Vector2Add(mousePos, offset), WHITE);
}
