#pragma once

#include <array>
#include <cstdint>
#include <stack>
#include <stdexcept>

class Command {
  public:
    virtual ~Command() = default;

    virtual void execute() = 0;
};

// 0x00E0
class ClearScreen final : public Command {
  public:
    explicit ClearScreen(std::array<std::array<bool, 64>, 32> &display)
        : display(display) {}

    void execute() override {
        for (auto &row : display) {
            for (auto &col : row) {
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
    Return(uint16_t &pc, std::stack<uint16_t> &stack) : pc(pc), stack(stack) {}

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
    Jump(uint16_t &pc, const uint16_t nnn) : nnn(nnn), pc(pc) {}

    void execute() override { pc = nnn; }

  private:
    uint16_t nnn;
    uint16_t &pc;
};

// 0x2NNN
class Call final : public Command {
  public:
    Call(uint16_t &pc, const uint16_t nnn, std::stack<uint16_t> &stack)
        : nnn(nnn), pc(pc), stack(stack) {}

    void execute() override {
        stack.push(pc);
        pc = nnn;
    }

  private:
    uint16_t nnn;
    uint16_t &pc;
    std::stack<uint16_t> &stack;
};

// 0x3XNN
class IsEqual final : public Command {
  public:
    IsEqual(uint16_t &pc, const uint16_t nn, std::array<uint8_t, 16> &v,
            uint8_t x)
        : nn(nn), pc(pc), v(v), x(x) {}

    void execute() override {
        if (v[x] == nn)
            pc += 2;
    }

  private:
    uint16_t nn;
    uint16_t &pc;
    std::array<uint8_t, 16> &v;
    uint8_t x;
};

// 0x4XNN
class IsNotEqual : Command {
  public:
    IsNotEqual(uint16_t &pc, const uint16_t nn, std::array<uint8_t, 16> &v,
               uint8_t x)
        : nn(nn), pc(pc), v(v), x(x) {}

    void execute() override {
        if (v[x] != nn)
            pc += 2;
    }

  private:
    uint16_t nn;
    uint16_t &pc;
    std::array<uint8_t, 16> &v;
    uint8_t x;
};

// 0x5XY0
class IsEqualXY : Command {
  public:
    IsEqualXY(uint16_t &pc, const uint8_t y, std::array<uint8_t, 16> &v,
              uint8_t x)
        : y(y), pc(pc), v(v), x(x) {}

    void execute() override {
        if (v[x] == v[y])
            pc += 2;
    }

  private:
    uint16_t &pc;
    std::array<uint8_t, 16> &v;
    uint8_t x;
    uint8_t y;
};

// 0x6XNN
class AssignX : Command {
  public:
    AssignX(const uint16_t nn, std::array<uint8_t, 16> &v, uint8_t x)
        : nn(nn), v(v), x(x) {}

    void execute() override { v[x] = nn; }

  private:
    uint16_t nn;
    std::array<uint8_t, 16> &v;
    uint8_t x;
};

// 0x7XNN
class AddAssignX : Command {
  public:
    AddAssignX(const uint16_t nn, std::array<uint8_t, 16> &v, uint8_t x)
        : nn(nn), v(v), x(x) {}

    void execute() override { v[x] += nn; }

  private:
    uint16_t nn;
    std::array<uint8_t, 16> &v;
    uint8_t x;
};

// TODO remake as separate commands maybe?
// 0x8XY_
class BitAndMath : Command {
  public:
    BitAndMath(uint8_t n, uint8_t x, uint8_t y, std::array<uint8_t, 16> &v)
        : n(n), x(x), y(y), v(v) {}

    void execute() override {
        if (n == 0)
            v[x] = v[y];
        else if (n == 1)
            v[x] |= v[y];
        else if (n == 2)
            v[x] &= v[y];
        else if (n == 3)
            v[x] ^= v[y];
        else if (n == 4) {
            v[0xf] = v[x] > 255 - v[y] ? 1 : 0;
            v[x] += v[y];
        } else if (n == 5) {
            v[0xf] = v[x] > v[y] ? 1 : 0;
            v[x] -= v[y];
        } else if (n == 6) {
            v[0xf] = v[x] & 0x1;
            v[x] >>= 1;
        } else if (n == 7) {
            v[0xf] = v[y] > v[x] ? 1 : 0;
            v[x] = v[y] - v[x];
        } else if (n == 0xE) {
            v[0xf] = v[x] >> 7;
            v[x] <<= 1;
        }
    }

  private:
    uint8_t n;
    std::array<uint8_t, 16> &v;
    uint8_t x;
    uint8_t y;
};

// 9xXY0
class IsNotEqualXY : Command {
  public:
    IsNotEqualXY(uint8_t x, uint8_t y, uint16_t &pc, std::array<uint8_t, 16> &v)
        : x(x), y(x), pc(pc), v(v) {}

    void execute() override {
        if (v[x] != v[y])
            pc += 2;
    }

  private:
    std::array<uint8_t, 16> &v;
    uint8_t x;
    uint8_t y;
    uint16_t &pc;
};

// 0xANNN
class SetI : Command {
  public:
    SetI(uint16_t nnn, uint16_t &i, uint16_t opcode)
        : nnn(nnn), i(i), opcode(opcode) {}

    void execute() override { i = opcode & 0xFFF; }

  private:
    uint16_t nnn;
    uint16_t &i;
    uint16_t opcode;
};

// 0xBNNN
class JumpPlus : Command {
  public:
    JumpPlus(uint16_t nnn, uint16_t &pc, std::array<uint8_t, 16> &v)
        : nnn(nnn), pc(pc), v(v) {}
    void execute() override { pc = v[0] + nnn; }

  private:
    uint16_t nnn;
    uint16_t &pc;
    std::array<uint8_t, 16> &v;
};

// 0xCXNN
class Rand : Command {
  public:
    Rand(uint8_t nn, uint8_t x, std::array<uint8_t, 16> &v)
        : nn(nn), x(x), v(v) {}
    void execute() override {
        // TODO think about changing to random from std lib
        v[x] = rand() & nn;
    }

  private:
    uint8_t nn;
    uint8_t x;
    std::array<uint8_t, 16> &v;
};

// 0xDXYN
class Draw : Command {
  public:
    Draw(uint16_t &i, uint8_t (&memory)[],
         std::array<std::array<bool, 64>, 32> &display, uint8_t x, uint8_t y,
         uint8_t n, std::array<uint8_t, 16> &v)
        : i(i), memory(memory), display(display), x(x), y(y), n(n), v(v) {}

    void execute() override {
        // TODO FIX AND TIDY MOVE
        v[0xf] = 0;
        // T ODO xors and stop when out of bounds?
        for (auto nth = 0; nth < n; nth++) {
            const auto sprite_byte = memory[i + nth];
            for (int xline = 0; xline < 8; xline++) {
                const auto curr_pixel = sprite_byte >> (7 - xline) & 0x1;
                const auto x_cord = v[x] % 64 + xline;
                const auto y_cord = v[y] % 32 + nth;

                if (curr_pixel & display[y_cord][x_cord]) {
                    v[0xf] = true;
                    display[y_cord][x_cord] = false;
                } else if (curr_pixel) {
                    display[y_cord][x_cord] = true;
                }
            }
        }
    }

  private:
    uint8_t (&memory)[];
    std::array<std::array<bool, 64>, 32> &display;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint16_t &i;
    std::array<uint8_t, 16> &v;
};

// 0xEX9E
class IsDown : Command {
  public:
    IsDown(std::array<uint8_t, 16> &v, uint8_t x, uint16_t &pc, uint8_t key)
        : v(v), x(x), pc(pc), key(key) {}
    void execute() override {
        if (key == v[x])
            pc += 2;
    }

  private:
    std::array<uint8_t, 16> &v;
    uint8_t x;
    uint16_t &pc;
    uint8_t key;
};

// 0xEXA1
class IsNotDown : Command {
  public:
    IsNotDown(std::array<uint8_t, 16> &v, uint8_t x, uint16_t &pc, uint8_t key)
        : v(v), x(x), pc(pc), key(key) {}
    void execute() override {
        if (key != v[x])
            pc += 2;
    }

  private:
    std::array<uint8_t, 16> &v;
    uint8_t x;
    uint16_t &pc;
    uint8_t key;
};

// ---- FX__ ------

// 0xFX07
class GetDelay : Command {
  public:
    GetDelay(uint8_t x, uint8_t &delay_timer, std::array<uint8_t, 16> &v)
        : x(x), delay_timer(delay_timer), v(v) {}
    void execute() override { v[x] = delay_timer; }

  private:
    uint8_t x;
    uint8_t &delay_timer;
    std::array<uint8_t, 16> &v;
};

// 0xFX0A
class AwaitKeyPress : Command {
  public:
    AwaitKeyPress(uint8_t x, std::array<uint8_t, 16> &v, uint8_t key)
        : x(x), v(v), key(key) {}

    void execute() override { v[x] = key; }

  private:
    uint8_t x;
    std::array<uint8_t, 16> &v;
    uint8_t key;
};

// 0xFX1E
class AddVxToI : Command {
  public:
    AddVxToI(std::array<uint8_t, 16> &v, uint16_t &i, uint8_t x)
        : v(v), i(i), x(x) {}

    void execute() override { i += v[x]; }

  private:
    std::array<uint8_t, 16> &v;
    uint16_t &i;
    uint8_t x;
};

// 0xFX33
class SetBCD : Command {
  public:
    SetBCD(uint8_t x, uint16_t &i, uint8_t (&memory)[],
           std::array<uint8_t, 16> &v)
        : x(x), i(i), memory(memory), v(v) {}

    void execute() override {
        // TODO what if error
        memory[i] = v[x] / 100;
        memory[i + 1] = v[x] % 100 / 10;
        memory[i + 2] = v[x] % 10;
    }

  private:
    uint8_t x;
    uint16_t &i;
    uint8_t (&memory)[];

    std::array<uint8_t, 16> &v;
};

// 0xFX55
class RegDump : Command {
  public:
    RegDump(uint8_t x, uint16_t &i, uint8_t (&memory)[],
            std::array<uint8_t, 16> &v)
        : x(x), i(i), memory(memory), v(v) {}

    void execute() override {
        // TODO what if error happens?
        for (int offset = 0; offset <= x; offset++) {
            memory[i + offset] = v[offset];
        }
    }

  private:
    uint8_t x;
    uint16_t &i;
    uint8_t (&memory)[];

    std::array<uint8_t, 16> &v;
};

// 0xFX65
class RegLoad : Command {
  public:
    RegLoad(uint8_t x, uint16_t &i, uint8_t (&memory)[],
            std::array<uint8_t, 16> &v)
        : x(x), i(i), memory(memory), v(v) {}

    void execute() override {
        // TODO what if error
        for (int offset = 0; offset <= x; offset++) {
            v[offset] = memory[i + offset];
        }
    }

  private:
    uint8_t x;
    uint16_t &i;
    uint8_t (&memory)[];

    std::array<uint8_t, 16> &v;
};
