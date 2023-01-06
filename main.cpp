#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "raylib.h"
#include <iostream>
#include <stdio.h>
#include <vector>

#define CELL_SIZE 10

#define GUI_FONT_SIZE 35
#define STATUS_HEIGHT 50

#define GRID_HEIGHT 225
#define GRID_WIDTH 150

using namespace std;

void update(vector<vector<bool>> *grid);

int main() {
  InitWindow(GRID_HEIGHT * CELL_SIZE, GRID_WIDTH * CELL_SIZE,
             "Conway's Game of Life");
  SetTargetFPS(30);

  Camera2D camera = {{0, 0}, {0, 0}, 0, 0};
  camera.zoom = 1.0f;

  vector<vector<bool>> grid(GRID_HEIGHT, vector<bool>(GRID_WIDTH));

  auto updating = false;

  auto randomize = [&grid]() {
    for (auto i = 0; i < GRID_HEIGHT; i++) {
      for (auto j = 0; j < GRID_WIDTH; j++) {
        grid[i][j] = GetRandomValue(0, 1);
      }
    }
  };

  auto draw = [&grid]() {
    for (auto i = 0; i < GRID_HEIGHT; i++) {
      for (auto j = 0; j < GRID_WIDTH; j++) {
        if (grid[i][j]) {
          DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                        BLACK);
        }
      }
    }
  };

  auto generation = 0;

  auto reset = [&camera, &generation, &grid]() {
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.zoom = 1.0f;

    for (auto i = 0; i < GRID_HEIGHT; i++) {
      for (auto j = 0; j < GRID_WIDTH; j++) {
        grid[i][j] = false;
      }
    }

    generation = 0;
  };

  while (!WindowShouldClose()) {
    if (IsKeyReleased(KEY_SPACE)) {
      updating = !updating;
    }

    if (IsKeyReleased(KEY_C)) {
      reset();
    }

    if (IsKeyReleased(KEY_R)) {
      reset();
      randomize();
    }

    if (IsKeyDown(KEY_UP)) {
      camera.target.y -= 20;
    } else if (IsKeyDown(KEY_DOWN)) {
      camera.target.y += 20;
    } else if (IsKeyDown(KEY_RIGHT)) {
      camera.target.x += 20;
    } else if (IsKeyDown(KEY_LEFT)) {
      camera.target.x -= 20;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
        IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {

      auto mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
      if (mousePos.x >= 0 && mousePos.x < GRID_HEIGHT * CELL_SIZE &&
          mousePos.y >= 0 && mousePos.y < GRID_WIDTH * CELL_SIZE) {
        grid[mousePos.x / CELL_SIZE][mousePos.y / CELL_SIZE] =
            IsMouseButtonDown(MOUSE_BUTTON_LEFT);
      }
    }

    auto wheel = GetMouseWheelMove();
    if (wheel != 0) {
      auto mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

      camera.offset = GetMousePosition();

      camera.target = mouseWorldPos;

      const auto zoomIncrement = 0.150f;

      camera.zoom += (wheel * zoomIncrement);
      if (camera.zoom < zoomIncrement)
        camera.zoom = zoomIncrement;
    }

    if (IsKeyDown(KEY_COMMA) || IsKeyDown(KEY_PERIOD)) {
      SetTargetFPS(GetFPS() + (IsKeyDown(KEY_COMMA) ? -5 : 5));
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    draw();

    GuiSetStyle(DEFAULT, TEXT_SIZE, GUI_FONT_SIZE);

    Rectangle bounds = {0, 0, (float)GetScreenWidth(), STATUS_HEIGHT};

    auto status =
        "Generation " + to_string(generation) + " | FPS " + to_string(GetFPS());

    EndMode2D();

    GuiStatusBar(bounds, status.c_str());

    EndDrawing();

    if (updating) {
      update(&grid);
      generation++;
    }
  }
}

void update(vector<vector<bool>> *grid) {
  auto prev_grid = *grid;
  for (auto i = 0; i < GRID_HEIGHT; i++) {
    for (auto j = 0; j < GRID_WIDTH; j++) {
      auto neighbors = 0;

      // count neighbors
      for (auto k = -1; k <= 1; k++) {
        for (auto l = -1; l <= 1; l++) {

          // if looking at self, skip
          if (k == 0 && l == 0) {
            continue;
          }

          // if neighbor is out of bounds, skip
          if (i + k < 0 || i + k >= GRID_HEIGHT || j + l < 0 ||
              j + l >= GRID_WIDTH) {
            continue;
          }

          // if neighbor is alive, increment
          if (prev_grid[i + k][j + l]) {
            neighbors++;
          }
        }
      }

      // if fewer than 2 neighbors, cell dies
      if (neighbors < 2) {
        (*grid)[i][j] = false;
      }

      // if more than 3 neighbors, cell dies
      if (neighbors > 3) {
        (*grid)[i][j] = false;
      }

      // if exactly 3 neighbors, cell is born
      if (neighbors == 3) {
        (*grid)[i][j] = true;
      }
    }
  }
}
