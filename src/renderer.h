#pragma once
#include "raylib.h"

// Wrapper for all the rendering code
class Renderer {
public:
    Renderer(int width, int height) {
        InitWindow(width, height, "CHIP-8emu");
        SetTargetFPS(60);
    }

    void draw() {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    bool should_end() {
        return WindowShouldClose();
    }

    ~Renderer() {
        CloseWindow();
    }
};
