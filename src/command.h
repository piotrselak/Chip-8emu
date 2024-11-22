#pragma once

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

// 0x00E0
class ClearScreen final : public Command {
public:
    explicit
    ClearScreen(std::array<std::array<bool, 64>, 32> &display) : display(
        display) {
    }

    void execute() override {
        for (auto &row: display) {
            for (auto &col: row) {
                col = false;
            }
        }
    }

private:
    std::array<std::array<bool, 64>, 32> &display;
};

// 0x00EE
class Return final : public Command {
public:
    Return(uint16_t &pc,
           std::stack<uint16_t> &stack) : pc(pc), stack(stack) {
    }

    void execute() override {
        if (stack.empty())
            throw std::runtime_error("Can't pop from empty stack.");
        pc = stack.top();
        stack.pop();
    }

private:
    uint16_t &pc;
    std::stack<uint16_t> &stack;
};

// 0x1NNN
class Jump final : public Command {
public:
    Jump(uint16_t &pc, const uint16_t nnn) : nnn(nnn),
                                             pc(pc) {
    }

    void execute() override {
        pc = nnn;
    }

private:
    uint16_t nnn;
    uint16_t &pc;
};
