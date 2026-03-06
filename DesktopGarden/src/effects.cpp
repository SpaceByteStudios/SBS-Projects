#include "effects.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

#include "constants.h"
#include "raylib.h"
#include "raymath.h"

Effects::Effects() {
  cloudsTexture = LoadTexture("assets/sprites/Clouds.png");

  clouds.resize(maxClouds);

  for (int i = 0; i < 10; i++) {
    Vector2 pos = {(float)(rand() % 3000), (float)rand() / RAND_MAX * TILE_SIZE * 2.0f};
    float speed = 5.0f + rand() % 15;

    float shadowOffset = 1.5f + (float)rand() / RAND_MAX * 6.0f * TILE_SIZE;

    Cloud cloud = {rand() % 3, pos, speed, shadowOffset};
    clouds[i] = cloud;
    cloudsAmount += 1;
  }
}

void Effects::updateEffects() {
  for (std::optional<Cloud>& cloud : clouds) {
    if (cloud.has_value()) {
      cloud->cloudPos = Vector2Add(cloud->cloudPos, {-cloud->cloudSpeed * GetFrameTime(), 0.0f});
    }
  }

  for (std::optional<Cloud>& cloud : clouds) {
    if (cloud.has_value()) {
      if (cloud->cloudPos.x >= -300.0f) {
        continue;
      }

      cloud = std::nullopt;
      cloudsAmount -= 1;

      int numNewClouds = rand() % 3;

      for (int i = 0; i < numNewClouds; i++) {
        float spawnChance = 1.0f - ((float)cloudsAmount / maxClouds);
        float roll = (float)rand() / RAND_MAX;

        if (roll < spawnChance) {
          spawnCloud();
        }
      }
    }
  }

  bool anyCloud = false;
  for (const auto& cloud : clouds) {
    if (cloud.has_value()) {
      anyCloud = true;
      break;
    }
  }
  if (!anyCloud) {
    spawnCloud();
  }
}

void Effects::spawnCloud() {
  for (auto& slot : clouds) {
    if (!slot.has_value()) {
      Vector2 pos = {2000.0f + rand() % 2000, (float)rand() / RAND_MAX * TILE_SIZE * 2.0f};
      float speed = 10.0f + rand() % 25;

      float shadowOffset = 1.5f + (float)rand() / RAND_MAX * 6.0f * TILE_SIZE;

      slot = {rand() % 3, pos, speed, shadowOffset};
      cloudsAmount += 1;

      break;
    }
  }

  std::sort(clouds.begin(), clouds.end(), [](const std::optional<Cloud>& a, const std::optional<Cloud>& b) {
    if (!a.has_value() && !b.has_value())
      return false;
    if (!a.has_value())
      return false;
    if (!b.has_value())
      return true;
    return a->shadowOffset < b->shadowOffset;
  });
}

void Effects::drawClouds() {
  for (std::optional<Cloud>& cloud : clouds) {
    if (cloud.has_value()) {
      Rectangle shadowSource = {cloud->id * TILE_SIZE * 3, 2 * TILE_SIZE, TILE_SIZE * 3, TILE_SIZE * 2};
      Vector2 shadowPos = Vector2Add(cloud->cloudPos, {0.0f, cloud->shadowOffset});

      DrawTextureRec(cloudsTexture, shadowSource, shadowPos, WHITE);
    }
  }

  for (std::optional<Cloud>& cloud : clouds) {
    if (cloud.has_value()) {
      Rectangle cloudSource = {cloud->id * TILE_SIZE * 3, 0.0f, TILE_SIZE * 3, TILE_SIZE * 2};

      DrawTextureRec(cloudsTexture, cloudSource, cloud->cloudPos, WHITE);
    }
  }
}
