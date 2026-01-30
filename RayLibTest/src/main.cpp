#include "raylib.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void); // Update and Draw one frame

int main() {
  InitWindow(screenWidth, screenHeight, "Raylib Testing");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    UpdateDrawFrame();
  }

  CloseWindow(); // Close window and OpenGL context

  return 0;
}

void UpdateDrawFrame(void) {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);

  EndDrawing();
}
