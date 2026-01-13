######################################################################
# Makefile variables
######################################################################
PROJECT_NAME = 2dgameengine
CC = g++
LANG_STD = -std=c++17
OBJECT_NAME = $(PROJECT_NAME)

# Homebrew paths (for MacOS)
INCLUDE_PATH = -I"./libs" -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
LIBRARY_PATH = -L/opt/homebrew/lib

# Hide warnings
COMPILER_FLAGS = -Wall -Wfatal-errors -w

SRC_FILES = $(shell find src/ -name "*.cpp") \
            $(shell find libs/imgui/ -name "*.cpp")

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

######################################################################
# Makefile rules
######################################################################
build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LIBRARY_PATH) $(LINKER_FLAGS) -o $(OBJECT_NAME)

run:
	./$(OBJECT_NAME)

clean:
	rm -f ./$(OBJECT_NAME)