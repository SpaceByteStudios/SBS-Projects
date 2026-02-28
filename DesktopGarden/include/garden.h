#pragma once

#include <memory>
#include <vector>

#include "field.h"
#include "plantsData.h"
#include "raylib.h"
#include "ui.h"

struct Garden {
  void init(PlantsData plantsData, UI ui);
  void drawGarden();

  int tilesRows = 0;
  int tilesColumns = 0;

  Texture2D grassTileset;
  Texture2D fenceTileset;
  Texture2D propsAtlas;

  const int fenceOffset = 24;
  const int propsOffset = 48;

  std::vector<int> propsMap;
  std::vector<int> propsFlipsMap;

  std::vector<std::unique_ptr<Field>> fields;
  UI ui;
};
