#include "raylib.h"
#include <iostream>
#include <vector>

#define GRID_HEIGHT 225
#define GRID_WIDTH 150

#define CELL_SIZE 10

using namespace std;

void update(vector<vector<bool>> *grid);

int main() {
  InitWindow(GRID_HEIGHT * CELL_SIZE, GRID_WIDTH * CELL_SIZE,
             "Conway's Game of Life");
  SetTargetFPS(30);

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

  while (!WindowShouldClose()) {
    if (IsKeyReleased(KEY_SPACE)) {
      updating = !updating;
      cout << updating << endl;
    }

    if (IsKeyReleased(KEY_C)) {
      for (auto i = 0; i < GRID_HEIGHT; i++) {
        for (auto j = 0; j < GRID_WIDTH; j++) {
          grid[i][j] = false;
        }
      }
    }

    if (IsKeyReleased(KEY_R)) {
      randomize();
    }

    if (IsMouseButtonDown(0)) {
      Vector2 pos = GetMousePosition();
      int x = (int)pos.x;
      int y = (int)pos.y;
      grid[x / CELL_SIZE][y / CELL_SIZE] = true;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);
    draw();
    EndDrawing();

    if (updating) {
      update(&grid);
    }
  }
}

void update(vector<vector<bool>> *grid) {
  vector<vector<bool>> prev_grid = *grid;
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
