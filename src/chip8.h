#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "command.h"
#include "view.h"

// TODO add font?
class Chip8 {
  public:
    constexpr static uint16_t ROM_START = 0x200;

    explicit Chip8(std::unique_ptr<IView> view) {
        ClearScreen(display).execute();
        this->view = std::move(view);
    }

    void loop() {
        const int CPU_HZ = 700; // Instruction cycles per second
        const float CPU_CYCLE_TIME = 1.0f / CPU_HZ;
        float cpuAccumulator = 0.0f;

        while (!view->should_end()) {
            float deltaTime = view->get_delta_time();
            cpuAccumulator += deltaTime;

            while (cpuAccumulator >= CPU_CYCLE_TIME) {
                const auto opcode = fetch();
                execute(opcode);
                cpuAccumulator -= CPU_CYCLE_TIME;
            }
            // Cant move to DXYN as raylib relies on drawing for many things
            view->draw(get_display());
        }
    }

    [[nodiscard]] std::array<std::array<bool, 64>, 32> get_display() const {
        return display;
    }

    void load(const std::vector<char> &buffer);

    uint16_t fetch();

    // std::unique_ptr<Command> decode(uint16_t opcode);

    void execute(uint16_t opcode);

  private:
    std::unique_ptr<IView> view;

    uint8_t memory[4096] = {};
    // TODO change inside array to 8 8bit uint?
    std::array<std::array<bool, 64>, 32> display{};
    uint16_t pc = ROM_START;
    uint16_t i = 0;
    std::array<uint8_t, 16> v{};
    std::stack<uint16_t> stack{};
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
};
