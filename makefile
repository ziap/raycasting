##############################################################################
#
# C++ Raycaster engine's makefile
# 
# Suitable for POSIX platforms (Linux, *BSD, OSX etc.).
# Not tested but should also work with MinGW on Windows.
# 
# WebAssembly support is planned
#
# Requirements:
#  - GNU Make
#  - A C++ Compiler (clang is prefered and is the default)
#  - SDL2 (not required for targetting wasm, use <canvas> image data instead)
#
# Future requirements:
#  - Python and PIP (for loading images at compile time)
# 
# See README.MD for more information about the project.
# See LICENSE file for copyright and license details.
#
##############################################################################

# The name of your program
OUTPUT=main

CXX=clang++
FLAGS=-std=c++20 -Wall
LIBRARIES=$$(sdl2-config --libs)
DEBUG_FLAGS=-g -Wextra
BUILD_FLAGS=-O3 -march=native -mtune=native
WASM_FLAGS=--target=wasm32 -Os -flto -nostdlib -Wl,--no-entry, \
					 -Wl,--export-all, -Wl,-lto-O3 -DUSE_WASM=true \
					 -Wl,--allow-undefined
INPUT_DIR=src
OUTPUT_DIR=obj

# Get list of input and output files
INPUTS=$(wildcard $(INPUT_DIR)/*.cpp)
OUTPUTS=$(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(notdir $(INPUTS)))

build: build/$(OUTPUT)
debug: debug/$(OUTPUT)
web: web/$(OUTPUT).wasm

# Directly build output file with .cpp files
build/$(OUTPUT): $(INPUTS)
	@mkdir -p build
	@echo Compiling $@
	@$(CXX) -o $@ $^ $(FLAGS) $(BUILD_FLAGS) $(LIBRARIES)

# Merging all .o files into output file
debug/$(OUTPUT): $(OUTPUTS)
	@mkdir -p debug
	@echo Compiling $@
	@$(CXX) -o $@ $^ $(FLAGS) $(DEBUG_FLAGS) $(LIBRARIES)

web/$(OUTPUT).wasm: $(INPUTS)
	@echo Compiling $@
	@$(CXX) -o $@ $^ $(FLAGS) $(WASM_FLAGS)

# Incrementally compiling .cpp files into .o files
$(OUTPUTS): $(OUTPUT_DIR)/%.o: $(INPUT_DIR)/%.cpp
	@echo Compiling $(notdir $@)
	@mkdir -p $(OUTPUT_DIR)
	@$(CXX) -c $< -o $@ $(FLAGS) $(DEBUG_FLAGS) 

clean:
	rm -rf $(OUTPUT_DIR)
	rm -rf debug
	rm -rf build
	rm -f web/$(OUTPUT).wasm
