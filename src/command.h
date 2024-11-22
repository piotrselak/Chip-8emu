#pragma once
#include "context.h"

class Command {
public:
    explicit Command(const Context &context) : context(context) {
    };

    virtual ~Command() = default;

    virtual void execute() = 0;

protected:
    Context context;
};

// 0x00E0
class ClearScreen final : public Command {
public:
    using Command::Command;

    void execute() override {
        for (auto &row: context.display) {
            for (auto &col: row) {
                col = false;
            }
        }
    }
};

// 0x00EE
class Return final : public Command {
public:
    using Command::Command;

    void execute() override {
        if (context.stack.empty())
            throw std::runtime_error("Can't pop from empty stack.");
        context.pc = context.stack.top();
        context.stack.pop();
    }
};

// 0x1NNN
class Jump final : public Command {
public:
    Jump(const Context &context, const uint16_t nnn) : Command(context),
        nnn(nnn) {
    }

    void execute() override {
        context.pc = nnn;
    }

private:
    uint16_t nnn;
};
