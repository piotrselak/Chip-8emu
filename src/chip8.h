#pragma once
#include <cstdint>

class Chip8 {
public:
    void load(const std::vector<char> &buffer) {
        constexpr unsigned short ROM_START = 0x200;

        if (constexpr size_t MAX_ROM_SIZE = sizeof(memory) - ROM_START;
            buffer.size() > MAX_ROM_SIZE) {
            throw std::runtime_error("Program size exceeds available memory.");
        }

        for (size_t i = 0; i < buffer.size(); ++i) {
            memory[ROM_START + i] = static_cast<unsigned char>(buffer[i]);
        }
    }

private:
    unsigned short int memory[4096] = {};
    unsigned short int pc = 0;
    unsigned short int i = 0;
    uint8_t registers[16] = {};
    // TODO: stack here
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;
};
