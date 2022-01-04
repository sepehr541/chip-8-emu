SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SCR_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = play
INCLUDE_PATHS = -Iinclude -I/usr/local/Cellar/sdl2/2.0.18/include/SDL2
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
LINKER_FLAGS = -lsdl2

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(SCR_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)