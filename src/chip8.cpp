#include "chip8.h"
#include "command.h"
#include "raylib.h"
#include <cstdint>

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
        Rand(nn, x, v).execute();
        break;
    case 0xD:
        Draw(i, memory, display, x, y, n, v).execute();
        break;
    case 0xE: {
        auto keypad_key = view->get_key();
        if (nn == 0x9E) {
            IsDown(v, x, pc, keypad_key).execute();
        } else if (nn == 0xA1) {
            IsNotDown(v, x, pc, keypad_key).execute();
        }
        break;
    }
    case 0xF: // TODO refactor to else ifs
        if (y == 0x0 && n == 0x7)
            GetDelay(x, delay_timer, v).execute();
        else if (nn == 0x0A) {
            std::cout << "Here2" << std::endl;
            auto key = 0; // TODO await_keypad_key();
            AwaitKeyPress(x, v, key).execute();
        }
        if (y == 0x1 && n == 0xE)
            AddVxToI(v, i, x).execute();
        if (y == 0x3 && n == 0x3)
            SetBCD(x, i, memory, v).execute();
        if (y == 0x5 && n == 0x5)
            RegDump(x, i, memory, v).execute();
        if (y == 0x6 && n == 0x5)
            RegLoad(x, i, memory, v).execute();
        break;
    default:
        throw std::runtime_error("Unknown opcode.");
    }
}
