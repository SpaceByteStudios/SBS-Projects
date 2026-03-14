#include "audioPlayer.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>

#include "raylib.h"

AudioPlayer::AudioPlayer(const std::string& musicFolder) {
  for (const auto& entry : std::filesystem::directory_iterator(musicFolder)) {
    if (entry.is_regular_file() && entry.path().extension() == ".wav") {
      songFiles.push_back(entry.path().string());
    }
  }

  if (songFiles.empty()) {
    std::cerr << "No .wav files found in " << musicFolder << std::endl;
    return;
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(songFiles.begin(), songFiles.end(), g);

  currentSongIndex = 0;

  currentSong = LoadMusicStream(songFiles[currentSongIndex].c_str());

  currentSong.looping = false;
  SetMusicVolume(currentSong, musicVolume);

  PlayMusicStream(currentSong);
}

void AudioPlayer::updateVolume(float mVolume, float sVolume) {
  musicVolume = mVolume;
  sfxVolume = sVolume;

  SetMusicVolume(currentSong, musicVolume);
}

void AudioPlayer::updateMusic() {
  if (songFiles.empty())
    return;

  UpdateMusicStream(currentSong);

  if (!IsMusicStreamPlaying(currentSong)) {
    UnloadMusicStream(currentSong);

    currentSongIndex++;
    if (currentSongIndex >= songFiles.size()) {
      std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(songFiles.begin(), songFiles.end(), g);
      currentSongIndex = 0;
    }

    currentSong = LoadMusicStream(songFiles[currentSongIndex].c_str());

    currentSong.looping = false;
    SetMusicVolume(currentSong, musicVolume);

    PlayMusicStream(currentSong);
  }
}

void AudioPlayer::playSFX(Sound& sound) {
}
