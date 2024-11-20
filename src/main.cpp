#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "chip8.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Path to a program has to be given as an argument." <<
                std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cerr << "Could not read given program." << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    const std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer;
    if (!file.read(buffer.data(), file_size)) {
        std::cerr << "Error reading file.\n";
        return 1;
    }
    file.close();

    Chip8 chip8;
    chip8.load(buffer);


    return 0;
}
