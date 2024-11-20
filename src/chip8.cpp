#include "chip8.h"

void Chip8::load(const std::vector<char> &buffer) {
    if (constexpr size_t MAX_ROM_SIZE = sizeof(memory) - ROM_START;
        buffer.size() > MAX_ROM_SIZE) {
        throw std::runtime_error("Program size exceeds available memory.");
    }

    for (size_t i = 0; i < buffer.size(); ++i) {
        memory[ROM_START + i] = static_cast<unsigned char>(buffer[i]);
    }
}

uint16_t Chip8::fetch() {
    const uint8_t byte1 = memory[pc];
    const uint8_t byte2 = memory[pc + 1];
    pc += 2;

    const uint16_t opcode = (byte1 << 8) | byte2;
    return opcode;
}

void Chip8::clear_screen() {
    for (auto &row: display) {
        for (auto &col: row) {
            col = false;
        }
    }
}

void Chip8::execute(const uint16_t opcode) {
    const uint8_t x = opcode >> 8 & 0xF;
    const uint8_t y = opcode >> 4 & 0xF;
    const uint16_t nnn = opcode & 0xFFF;
    const uint8_t nn = opcode & 0xFF;
    const uint8_t n = opcode & 0xF;

    // TODO maybe tidy and move it somewhere else if needed
    switch (uint8_t first_nible = opcode >> 12) {
        case 0x0:
            if (nnn == 0x0EE) {
                pc = stack.top();
                stack.pop();
            } else if (nnn == 0x0E0)
                clear_screen();
            break;
        case 0x1:
            pc = opcode & 0xFFF;
            break;
        case 0x2:
            stack.push(pc);
            pc = nnn;
            break;
        case 0x3:
            if (v[x] == nn)
                i += 2;
            break;
        case 0x4:
            if (v[x] != nn)
                i += 2;
        case 0x5:
            if (v[x] == v[y])
                i += 2;
        case 0x6:
            v[x] = nn;
            break;
        case 0x7:
            v[x] += nn;
            break;
        case 0x8: // TODO use something to make it nicer than ifs ladder
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
            break;
        case 0x9:
            if (v[x] != v[y])
                pc += 2;
            break;
        case 0xA:
            i = opcode & 0xFFF;
            break;
        case 0xB:
            pc = v[0] + nnn;
            break;
        case 0xC:
            // TODO think about changing to random from std lib
            v[x] = rand() & nn; // NOLINT(cert-msc30-c, cert-msc50-cpp)
            break;
        case 0xD:
            // TODO FIX AND TIDY
            v[0xf] = 0;
        // TODO xors and stop when out of bounds?
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
            break;
        // TODO
        case 0xE:
            break;
        // TODO
        case 0xF:
            break;
        default:
            throw std::runtime_error("Unknown opcode.");
    }
}

