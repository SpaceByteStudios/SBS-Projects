#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "field.h"
#include "plantsData.h"
#include "raylib.h"
#include "ui.h"

struct Garden {
  Garden(Game& game, UI& ui, PlantsData& plantsData);

  void drawGarden();
  void drawGardenIcons();

  void updateGarden();

  bool canUpgrade(int level);

  int tilesRows = 0;
  int tilesColumns = 0;

  int fieldsAmount = 1;

  Vector2 fieldButtonPos;

  Texture2D grassTileset;
  Texture2D fenceTileset;
  Texture2D propsAtlas;
  Texture2D treeAtlas;

  const int fenceOffset = 24;
  const int propsOffset = 48;

  std::vector<int> propsMap;
  std::vector<int> propsFlipsMap;

  std::vector<std::unique_ptr<Field>> fields;

  std::unordered_map<int, std::unordered_map<int, int>> upgradeCosts;

  Game& game;
  UI& ui;
  PlantsData& plantsData;
};
