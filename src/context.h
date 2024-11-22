#pragma once

struct Context {
    std::array<std::array<bool, 64>, 32> &display;
    uint16_t &pc;
    std::stack<uint16_t> &stack;

    explicit Context(std::array<std::array<bool, 64>, 32> &display,
                     uint16_t &pc, std::stack<uint16_t> &stack

    )
        : display(
              display), pc(pc), stack(stack) {
    }
};
