#include <cstdlib>
#include <ctime>

#include "constants.h"
#include "game.h"
#include "raylib.h"
// #include "win_overlay.h"

int main() {
  srand(time(nullptr));

  SetTraceLogLevel(LOG_NONE);

  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  InitWindow(1, 1, "Desktop Garden");
  SetWindowState(FLAG_WINDOW_UNDECORATED);
  SetWindowState(FLAG_WINDOW_TOPMOST);

  SetExitKey(KEY_NULL);

  int screenWidth = GetMonitorWidth(0);
  int screenHeight = GetMonitorHeight(0);

  int windowHeight = SCREEN_HEIGHT;
  int windowPosHeight = screenHeight - windowHeight;

  SetWindowSize(screenWidth, windowHeight);
  SetWindowPosition(0, windowPosHeight);

  // Doesn't make sense if Window is not Transparent
  //  MakeWindowClickThrough(GetWindowHandle());
  SetTargetFPS(60);

  InitAudioDevice();

  Game game;

  game.init();
  game.run();

  CloseAudioDevice();

  CloseWindow();
  return 0;
}
