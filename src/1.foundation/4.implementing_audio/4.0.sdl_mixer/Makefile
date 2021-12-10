PROJECT_NAME := red_engine

# OS Specific Stuff
ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
    BUILD_OBJECT := $(PROJECT_NAME).exe
    L_FLAGS := -lmingw32 -lSDL2main -lSDL2_mixer -lSDL2 -lpython37 -lfreetype -static-libgcc -static-libstdc++
    DELETE_CMD := del
else
    OS_TYPE := linux
    BUILD_OBJECT := $(PROJECT_NAME)
    L_FLAGS := -lSDL2 -lSDL2_mixer -lGL -lpython3.7m -lfreetype -lcrypt -lpthread -ldl  -lutil -lm -static-libgcc -static-libstdc++
    DELETE_CMD := rm
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
INCLUDE_DIR := ../../../include
GAME_LIB_DIR := $(INCLUDE_DIR)/re
I_FLAGS := -I"$(INCLUDE_DIR)" -I"${SDL2_INCLUDE}" -I"${PYTHON_INCLUDE}" -I"${FREETYPE_INCLUDE}"
LIBRARIES := -L"${SDL2_LIBS}" -L"${PYTHON_LIBS}" -L"${FREETYPE_LIBS}"
C_FLAGS := -w -Wfatal-errors
CPP_FLAGS := -std=c++14 $(C_FLAGS)

SRC = $(wildcard src/*.cpp  $(GAME_LIB_DIR)/python/*.cpp $(GAME_LIB_DIR)/utils/*.cpp $(GAME_LIB_DIR)/rendering/texture.cpp $(GAME_LIB_DIR)/rendering/shader.cpp $(GAME_LIB_DIR)/rendering/render_context.cpp $(GAME_LIB_DIR)/rendering/renderer_batcher.cpp $(GAME_LIB_DIR)/rendering/renderer_2d.cpp $(GAME_LIB_DIR)/rendering/sprite_renderer.cpp $(GAME_LIB_DIR)/rendering/font_renderer.cpp $(INCLUDE_DIR)/stb_image/stb_image.cpp)
SRC_C = $(wildcard $(INCLUDE_DIR)/glad/glad.c)

OBJ = $(SRC:.cpp=.o)
OBJ_C = $(SRC_C:.c=.o)

# MAIN

.PHONY: all build clean

all: clean format build

# Compiles if .o is missing or changes to the .cpp file
%.o: %.cpp
	@echo "Compiling " $< " into " $@
	@$(CXX) -c $(CPP_FLAGS) $< -o $@ $(I_FLAGS)

%.o: %.c
	@echo "Compiling " $< " into " $@
	@$(CC) -c $(C_FLAGS) $< -o $@ $(I_FLAGS)

build: $(OBJ) $(OBJ_C)
	@echo "Linking " $@
	@$(CXX) -o $(BUILD_OBJECT) $^ $(I_FLAGS) $(L_FLAGS) $(LIBRARIES)

clean:
ifneq ("$(wildcard $(BUILD_OBJECT))","")
	@$(DELETE_CMD) $(BUILD_OBJECT)
endif
ifeq ($(OS_TYPE),windows)
	@$(foreach object, $(OBJ), $(DELETE_CMD) $(subst /,\,$(object));)
else
	@$(foreach object, $(OBJ), $(DELETE_CMD) $(object);)
endif

run:
	./$(BUILD_OBJECT)

format:
	astyle -n --style=google --recursive src/*.h src/*.cpp $(GAME_LIB_DIR)/*.cpp $(GAME_LIB_DIR)/*.h $(GAME_LIB_DIR)/*.hpp
