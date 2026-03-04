#pragma once

#include <optional>
#include <vector>

#include "raylib.h"

struct Cloud {
  int id;

  Vector2 cloudPos;
  float cloudSpeed;

  float shadowOffset;
};

struct Effects {
  Effects();

  void updateEffects();
  void drawClouds();

  void spawnCloud();

  Texture2D cloudsTexture;

  int cloudsAmount = 0;
  const int maxClouds = 100;

  std::vector<std::optional<Cloud>> clouds;
};
