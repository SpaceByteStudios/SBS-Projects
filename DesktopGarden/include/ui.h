#pragma once

#include <memory>
#include <vector>

#include "moneySystem.h"
#include "plantsData.h"
#include "raylib.h"
#include "shop.h"

class Game;

struct UI {
  UI(Game& game, MoneySystem& moneySystem, PlantsData& plantsData);

  void updateUI();

  void drawShop();
  void drawButtons();
  void drawSelection();
  void drawCursor();

  void playWaterAnimation();

  float buttonSize = 1.6f;
  float buttonSpacing = 0.25f;

  std::vector<bool> pressedButton;
  std::vector<bool> hoversButton;

  std::vector<Vector2> buttonsPos;
  std::vector<Vector2> buttonsSize;

  Game& game;

  Texture2D selectionTexture;
  Texture2D buttonsTexture;
  Texture2D cursorTexture;
  Texture2D wateringTexture;
  Texture2D seedsTexture;

  Font moneyDisplayFont;
  Texture2D moneyDisplayTexture;

  int cursorType = 0;

  int wateringFrame = 0;
  float waterAnimationTimer = 0.0f;
  float waterFrameTime = 0.05f;
  bool playingAnimation = false;

  const Vector2 mouseNormalOffset = {-6, -4};
  const Vector2 mouseWaterOffset = {16, -32};
  const Vector2 waterOffset = {6, -46};

  MoneySystem& moneySystem;
  std::unique_ptr<Shop> shop;
};
