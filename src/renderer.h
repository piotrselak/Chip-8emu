#pragma once
#include "raylib.h"

// TODO rethink if needed here?
// Wrapper for all the rendering code
class Renderer {
public:
    Renderer(int width, int height) {
        InitWindow(width, height, "CHIP-8emu");
        SetTargetFPS(60);
    }

    void draw(const std::array<std::array<uint8_t, 8>, 32> &display) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 8; x++) {
                if (display[y][x]) {
                    DrawRectangle(x, y, 8, 1, BLACK);
                }
            }
        }

        EndDrawing();
    }

    bool should_end() {
        return WindowShouldClose();
    }

    ~Renderer() {
        CloseWindow();
    }
};
