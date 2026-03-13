#pragma once

#include <string>
#include <vector>

#include "raylib.h"

struct AudioPlayer {
  AudioPlayer(const std::string& folder);

  void updateMusic();
  void updateVolume(float mVolume, float sVolume);

  void playSFX(Sound& sound);

  float musicVolume = 0.2f;
  float sfxVolume = 0.2f;

  std::vector<std::string> songFiles;
  std::vector<Music> songs;
  Music currentSong;
  int currentSongIndex;
};
