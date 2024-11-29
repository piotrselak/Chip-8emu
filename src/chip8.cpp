#include "chip8.h"
#include "command.h"

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

// std::unique_ptr<Command> Chip8::decode(uint16_t opcode) {
// }

void Chip8::execute(const uint16_t opcode) {
    // TODO idk might be responsible for bugs
    //  if (opcode == 0x0) {
    //      pc = ROM_START;
    //      return;
    //  }

    const uint8_t x = opcode >> 8 & 0xF;
    const uint8_t y = opcode >> 4 & 0xF;
    const uint16_t nnn = opcode & 0xFFF;
    const uint8_t nn = opcode & 0xFF;
    const uint8_t n = opcode & 0xF;

    // TODO maybe tidy and move it somewhere else if needed
    switch (uint8_t first_nible = opcode >> 12) {
    case 0x0:
        if (nnn == 0x0EE)
            Return(pc, stack).execute();
        else if (nnn == 0x0E0)
            ClearScreen(display).execute();
        break;
    case 0x1:
        Jump(pc, nnn).execute();
        break;
    case 0x2:
        Call(pc, nnn, stack).execute();
        break;
    case 0x3:
        IsEqual(pc, nn, v, x).execute();
        break;
    case 0x4:
        IsNotEqual(pc, nn, v, x).execute();
        break;
    case 0x5:
        IsEqualXY(pc, y, v, x).execute();
        break;
    case 0x6:
        AssignX(nn, v, x).execute();
        break;
    case 0x7:
        AddAssignX(nn, v, x).execute();
        break;
    case 0x8:
        BitAndMath(n, x, y, v).execute();
        break;
    case 0x9:
        IsNotEqualXY(x, y, pc, v).execute();
        break;
    case 0xA:
        SetI(nnn, i, opcode).execute();
        break;
    case 0xB:
        JumpPlus(nnn, pc, v).execute();
        break;
    case 0xC:
        // TODO think about changing to random from std lib
        // v[x] = rand() & nn; // NOLINT(cert-msc30-c, cert-msc50-cpp)
        break;
    case 0xD:
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
        break;
    case 0xF:
        if (y == 0x0 && n == 0xA)
            GetDelay(x, delay_timer, v).execute();
        break;
    default:
        throw std::runtime_error("Unknown opcode.");
    }
}
