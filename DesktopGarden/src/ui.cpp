#include "ui.h"

#include "constants.h"
#include "raylib.h"
#include "raymath.h"

UI::UI() {
  selectionTexture = LoadTexture("assets/sprites/Selection.png");
}

void UI::drawButtons() {
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
