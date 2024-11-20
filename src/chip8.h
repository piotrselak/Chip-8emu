#pragma once
#include <array>
#include <cstdint>
#include <stack>

class Chip8 {
public:
    constexpr static uint16_t ROM_START = 0x200;

    Chip8() {
        clear_screen();
    }

    [[nodiscard]] std::array<std::array<bool, 64>, 32> get_display() const {
        return display;
    }

    void load(const std::vector<char> &buffer) {
        if (constexpr size_t MAX_ROM_SIZE = sizeof(memory) - ROM_START;
            buffer.size() > MAX_ROM_SIZE) {
            throw std::runtime_error("Program size exceeds available memory.");
        }

        for (size_t i = 0; i < buffer.size(); ++i) {
            memory[ROM_START + i] = static_cast<unsigned char>(buffer[i]);
        }
    }

    uint16_t fetch() {
        const uint8_t byte1 = memory[pc];
        const uint8_t byte2 = memory[pc + 1];
        pc += 2;

        const uint16_t opcode = (byte1 << 8) | byte2;
        return opcode;
    }

    void execute(const uint16_t opcode) {
        const uint8_t x = opcode >> 8 & 0xF;
        const uint8_t y = opcode >> 4 & 0xF;

        switch (uint8_t first_nible = opcode >> 12) {
            case 0x0:
                clear_screen();
                break;
            case 0x1:
                pc = opcode & 0xFFF;
                break;
            case 0x6: {
                const uint8_t nn = opcode & 0xFF;
                v[x] = nn;
                break;
            }
            case 0x7: {
                const uint8_t nn = opcode & 0xFF;
                v[x] += nn;
                break;
            }
            case 0xA:
                i = opcode & 0xFFF;
                break;
            case 0xD: {
                // TODO FIX AND TIDY
                const uint8_t n = opcode & 0xF;

                v[0xf] = 0;
                // TODO MODULO and xors and stop when out of bounds
                for (auto nth = 0; nth < n; nth++) {
                    auto sprite_byte = memory[i + nth];
                    for (int xline = 0; xline < 8; xline++) {
                        auto curr_pixel = sprite_byte >> (7 - xline) & 0x1;
                        auto x_cord = v[x] % 64 + xline;
                        auto y_cord = v[y] % 32 + nth;

                        if (curr_pixel & display[y_cord][x_cord]) {
                            v[0xf] = true;
                            display[y_cord][x_cord] = false;
                        } else if (curr_pixel) {
                            display[y_cord][x_cord] = true;
                        }
                    }
                }
                // TODO VF flag
                break;
            }
            default:
                throw std::runtime_error("Unknown opcode.");
        }
    }

private:
    void clear_screen() {
        for (auto &row: display) {
            for (auto &col: row) {
                col = false;
            }
        }
    }

    uint8_t memory[4096] = {};
    // TODO change inside array to 8 8bit uint
    std::array<std::array<bool, 64>, 32> display{};
    uint16_t pc = ROM_START;
    uint16_t i = 0;
    std::array<uint8_t, 16> v{};
    std::stack<uint8_t> stack;
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
};
