#pragma once

#include <vector>

#include "raylib.h"
struct Garden {
  Garden(float tilesWidth, float tilesHeight, int potsRows, int potsColumns);

  void drawGarden();

  float tilesWidth = 0;
  float tilesHeight = 0;

  int potsRows = 0;
  int potsColumns = 0;

  int tilesRows = 0;
  int tilesColumns = 0;

  Texture2D grass_tileset;
  Texture2D fence_tileset;
  Texture2D props_atlas;

  std::vector<int> props_map;
  std::vector<int> props_flips_map;
};
