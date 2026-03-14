#include "settings.h"

#include "constants.h"
#include "game.h"
#include "raylib.h"

Settings::Settings(Game& game) : game(game) {
  settingsTexture = LoadTexture("assets/sprites/ui/Settings.png");
  settingsSliders = LoadTexture("assets/sprites/ui/SettingsSliders.png");

  volume.resize(2);
  volume[0] = sfxVolume;
  volume[1] = musicVolume;

  game.updateVolume(volume[1], volume[0]);
}

void Settings::updateSettings() {
  Vector2 mousePos = GetMousePosition();

  for (int i = 0; i < 2; i++) {
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      break;
    }

    float yPos = (3.65f + 1.25f * i) * TILE_SIZE;
    Rectangle sliderBounds = {53.75f * TILE_SIZE, yPos, 3 * TILE_SIZE, TILE_SIZE};

    if (CheckCollisionPointRec(mousePos, sliderBounds)) {
      int relativeX = mousePos.x - sliderBounds.x;
      volume[i] = relativeX / sliderBounds.width;
    }
  }

  game.updateVolume(volume[1], volume[0]);
}

void Settings::drawSettings() {
  DrawTexture(settingsTexture, 51.5f * TILE_SIZE, 3.25f * TILE_SIZE, WHITE);

  Rectangle sliderSource = {0.0f, 0.0f, TILE_SIZE, TILE_SIZE};
  Rectangle emptySliderSource = {0.0f, TILE_SIZE, 3 * TILE_SIZE, TILE_SIZE};

  for (int i = 0; i < 2; i++) {
    float yPos = (3.65f + 1.25f * i) * TILE_SIZE;

    Rectangle iconSource = {(i + 1) * TILE_SIZE, 0.0f, TILE_SIZE, TILE_SIZE};
    Vector2 iconPos = {52.5f * TILE_SIZE, yPos};

    DrawTextureRec(settingsSliders, iconSource, iconPos, WHITE);

    Rectangle fullSliderSource = {0.0f, 2 * TILE_SIZE, 3 * TILE_SIZE * volume[i], TILE_SIZE};

    Vector2 SliderPos = {53.25f * TILE_SIZE + 3 * TILE_SIZE * volume[i], yPos};
    Vector2 SlidersPos = {53.75 * TILE_SIZE, yPos};

    DrawTextureRec(settingsSliders, emptySliderSource, SlidersPos, WHITE);
    DrawTextureRec(settingsSliders, fullSliderSource, SlidersPos, WHITE);
    DrawTextureRec(settingsSliders, sliderSource, SliderPos, WHITE);
  }
}
