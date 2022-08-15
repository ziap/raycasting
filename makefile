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
#  - SDL2
#
# Future requirements:
#  - Python and PIP (for loading images at compile time)
#  - Emscripten (for targetting WebAssembly)
# 
# See README.MD for more information about the project.
# See LICENSE file for copyright and license details.
#
##############################################################################

# The name of your program
OUTPUT=main

CXX=clang++
FLAGS=-std=c++20 -Wall -pthread
LIBRARIES=-lSDL2
DEBUG_FLAGS=-g -Wextra
BUILD_FLAGS=-O3 -march=native -mtune=native

INPUT_DIR=src
OUTPUT_DIR=obj

# Get list of input and output files
INPUTS=$(wildcard $(INPUT_DIR)/*.cpp)
OUTPUTS=$(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(notdir $(INPUTS)))

build: build/$(OUTPUT)
debug: debug/$(OUTPUT)

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

# Incrementally compiling .cpp files into .o files
$(OUTPUTS): $(OUTPUT_DIR)/%.o: $(INPUT_DIR)/%.cpp
	@echo Compiling $(notdir $@)
	@mkdir -p $(OUTPUT_DIR)
	@$(CXX) -c $< -o $@ $(FLAGS) $(DEBUG_FLAGS) 

clean:
	rm -rf $(OUTPUT_DIR)
	rm -rf debug
	rm -rf build
