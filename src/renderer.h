#pragma once
#include "raylib.h"

// TODO rethink if needed here?
// Wrapper for all the rendering code
class Renderer {
public:
    constexpr static int SCALE = 10;

    Renderer(int width, int height) {
        InitWindow(width * SCALE, height * SCALE, "CHIP-8emu");
        SetTargetFPS(60);
    }

    void draw(const std::array<std::array<bool, 64>, 32> &display) {
        BeginDrawing();
        // TODO maybe not clear everything if something didn't change? idk if worth it
        ClearBackground(RAYWHITE);

        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 64; x++) {
                if (display[y][x]) {
                    // TODO maybe optimize it by blocks or something
                    DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, BLACK);
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
