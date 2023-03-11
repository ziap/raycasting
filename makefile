##############################################################################
#
# C++ Raycaster engine's makefile
# 
# Suitable for POSIX platforms (Linux, *BSD, OSX etc.).
# Not tested but should also work with MinGW on Windows.
# 
# Requirements:
#  - GNU Make
#  - A C++ Compiler (clang is prefered and is the default)
#  - SDL2 (not required for targetting wasm, use <canvas> image data instead)
#  - Python, PIP and python-venv (you don't need to do anything)
# 
# See README.MD for more information about the project.
# See LICENSE file for copyright and license details.
#
##############################################################################

# The name of your program
OUTPUT=raycaster

CXX=clang++
FLAGS=-std=c++17 -Wall -Wextra -pedantic
LIBRARIES=$$(sdl2-config --libs)
DEBUG_FLAGS=-ggdb
BUILD_FLAGS=-Ofast -march=native -mtune=native
WASM_FLAGS=--target=wasm32 -Ofast -flto -fno-builtin -nostdlib \
	-fvisibility=hidden -DUSE_WASM=true -Wl,--no-entry, -Wl,--strip-debug, \
	-Wl,--lto-O3 -Wl,--allow-undefined -Wl,--export-dynamic -msimd128
INPUT_DIR=src
OUTPUT_DIR=obj
TEXTURE_DIR=assets

# Get list of input and output files
INPUTS=$(wildcard $(INPUT_DIR)/*.cpp)
OUTPUTS=$(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(notdir $(INPUTS)))

TEXTURE_INPUTS=$(wildcard $(TEXTURE_DIR)/*.png)
TEXTURE_OUTPUTS=$(patsubst %.png, $(INPUT_DIR)/texture_%.h, \
	$(notdir $(TEXTURE_INPUTS)))

VENV_NAME=.penv
ACTIVATE_VENV=. $(VENV_NAME)/bin/activate

build: assets build/$(OUTPUT)
debug: assets debug/$(OUTPUT)
web: assets $(OUTPUT).wasm

# Always recompile unless in debug mode
.PHONY: build/$(OUTPUT) $(OUTPUT).wasm clean_assets clean 

# Directly build output file with .cpp files
build/$(OUTPUT): $(INPUTS)
	@mkdir -p build
	@echo Compiling $@
	@$(CXX) -o $@ $(INPUTS) $(FLAGS) $(BUILD_FLAGS) $(LIBRARIES)

# Merging all .o files into output file
debug/$(OUTPUT): $(OUTPUTS)
	@mkdir -p debug
	@echo Compiling $@
	@$(CXX) -o $@ $^ $(FLAGS) $(DEBUG_FLAGS) $(LIBRARIES)

$(OUTPUT).wasm: $(INPUTS)
	@echo Compiling $@
	@$(CXX) -o $@ $^ $(FLAGS) $(WASM_FLAGS)

# Incrementally compiling .cpp files into .o files
$(OUTPUTS): $(OUTPUT_DIR)/%.o: $(INPUT_DIR)/%.cpp
	@echo Compiling $(notdir $@)
	@mkdir -p $(OUTPUT_DIR)
	@$(CXX) -c $< -o $@ $(FLAGS) $(DEBUG_FLAGS) 

assets: clean_assets $(VENV_NAME) $(TEXTURE_OUTPUTS)

$(TEXTURE_OUTPUTS): $(INPUT_DIR)/texture_%.h: $(TEXTURE_DIR)/%.png
	@echo Generating $(notdir $@)
	@$(ACTIVATE_VENV) && scripts/loadassets.py $< $(notdir $@) > $@

clean_assets:
	rm -f src/texture_*

clean: clean_assets
	rm -rf $(OUTPUT_DIR)
	rm -rf debug
	rm -rf build
	rm -f $(OUTPUT).wasm

$(VENV_NAME):
	@echo Creating the virtual environment
	@python -m venv $(VENV_NAME)
	@echo Installing packages
	@$(ACTIVATE_VENV) && pip install opencv-python numpy
