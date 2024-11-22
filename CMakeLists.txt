cmake_minimum_required(VERSION 3.29)
project(Chip_8emu)

set(CMAKE_CXX_STANDARD 20)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif ()
endif ()


add_executable(Chip_8emu src/main.cpp
        src/chip8.h
        src/rayview.h
        src/chip8.cpp
        src/view.h
        src/command.h
)

target_link_libraries(${PROJECT_NAME} raylib)