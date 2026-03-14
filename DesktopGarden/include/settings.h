#pragma once

#include <vector>

#include "raylib.h"
class Game;

struct Settings {
  Settings(Game& game);

  void updateSettings();
  void drawSettings();

  // SFX and Music Volume
  std::vector<float> volume;

  float musicVolume = 0.2f;
  float sfxVolume = 0.2f;

  Texture2D settingsTexture;
  Texture2D settingsSliders;

  Game& game;
};
