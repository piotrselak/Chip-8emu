#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <stack>
#include <vector>
#include "view.h"

// TODO add font?
class Chip8 {
public:
    constexpr static uint16_t ROM_START = 0x200;

    explicit Chip8(std::unique_ptr<IView> view) {
        clear_screen();
        this->view = std::move(view);
    }

    void loop() {
        while (!view->should_end()) {
            const auto opcode = fetch();

            // TODO debug remove
            // if (!stack.empty())
            //     std::cout << pc << " " << opcode << " " << stack.top() <<
            //             std::endl;
            // else
            //     std::cout << pc << " " << opcode << std::endl;

            execute(opcode);
            // Cant move to DXYN as raylib relies on drawing for many things
            view->draw(get_display());
        }
    }

    [[nodiscard]] std::array<std::array<bool, 64>, 32> get_display() const {
        return display;
    }

    void load(const std::vector<char> &buffer);

    uint16_t fetch();

    void execute(uint16_t opcode);

private:
    void clear_screen();

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
