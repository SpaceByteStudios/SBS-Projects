#pragma once

#include <string>
#include <vector>

#include "raylib.h"

struct AudioPlayer {
  AudioPlayer(const std::string& folder);

  void updateMusic();
  void updateVolume(float mVolume, float sVolume);

  void playSFX(Sound& sound);

  float musicVolume = 1.0f;
  float sfxVolume = 1.0f;

  std::vector<std::string> songFiles;
  std::vector<Music> songs;
  Music currentSong;
  int currentSongIndex;
};
