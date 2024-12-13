#pragma once
#include "raylib.h"
#include "view.h"
#include <cstdint>
#include <iostream>

// Wrapper for all the rendering code
class RayView final : public IView {
  public:
    constexpr static int SCALE = 10;

    RayView(const int width, const int height) {
        InitWindow(width * SCALE, height * SCALE, "CHIP-8emu");
        SetTargetFPS(60); // TODO
    }

    void draw(const std::array<std::array<bool, 64>, 32> display) override {
        BeginDrawing();
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

    float get_delta_time() { return GetFrameTime(); }

    bool should_end() override { return WindowShouldClose(); }

    ~RayView() override { CloseWindow(); }

    uint8_t get_key() override {
        if (IsKeyDown(49))
            return 1;

        // not used in any app value
        return 255;
    }
};
