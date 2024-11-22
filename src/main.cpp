#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "chip8.h"
#include "rayview.h"

std::vector<char> load_program_to_buffer(const char *path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not read given program.");
    }

    file.seekg(0, std::ios::end);
    const std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(file_size);
    if (!file.read(buffer.data(), file_size)) {
        throw std::runtime_error("Error reading file.\n");
    }
    file.close();

    return buffer;
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Path to a program has to be given as an argument." <<
                std::endl;
        return 1;
    }

    std::vector<char> buffer;
    try {
        buffer = load_program_to_buffer(argv[1]);
    } catch (std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    std::unique_ptr<IView> view = std::make_unique<RayView>(64, 32);

    Chip8 chip8(std::move(view));
    chip8.load(buffer);
    chip8.loop();

    return 0;
}
