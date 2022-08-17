cmake_minimum_required(VERSION 3.22.3)
set (CMAKE_CXX_STANDARD 17)

project(chip-8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
    ${PROJECT_NAME} 
    src/main.cpp
    src/display.cpp
    src/chip8.cpp
    src/chip8.Ops.cpp
    src/chip8.Op0.cpp
    src/chip8.Op8XY.cpp
    src/chip8.OpEX.cpp
    src/chip8.OpFX.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})