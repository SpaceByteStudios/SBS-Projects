#pragma once

#include <vector>

#include "field.h"
#include "raylib.h"

struct Garden {
  Garden(float tilesWidth, float tilesHeight);

  void drawGarden();

  float tilesWidth = 0;
  float tilesHeight = 0;

  int tilesRows = 0;
  int tilesColumns = 0;

  Texture2D grassTileset;
  Texture2D fenceTileset;
  Texture2D propsAtlas;

  const int fenceOffset = 24;
  const int propsOffset = 48;

  std::vector<int> propsMap;
  std::vector<int> propsFlipsMap;

  std::vector<Field> fields;
};
