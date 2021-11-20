PROJECT_NAME := learn_engine_dev

# OS Specific Stuff
ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
    BUILD_OBJECT := $(PROJECT_NAME).exe
    L_FLAGS := -lmingw32 -lSDL2main -lSDL2 -lpython37 -static-libgcc -static-libstdc++
    DELETE_CMD := del
else
    OS_TYPE := linux
    BUILD_OBJECT := $(PROJECT_NAME)
    L_FLAGS := -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm -static-libgcc -static-libstdc++
    DELETE_CMD := rm
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
INCLUDE_DIR := ../../../include
GAME_LIB_DIR := $(INCLUDE_DIR)/game_lib
I_FLAGS := -I"$(INCLUDE_DIR)" -I"${SDL2_HOME}/include" -I"${PYTHON_INCLUDE}"
LIBRARIES := -L"${SDL2_HOME}/lib" -L"${PYTHON_LIBS}"
C_FLAGS := -w -Wfatal-errors
CPP_FLAGS := $(CPP_FLAGS) -std=c++14

SRC = $(wildcard src/*.cpp  $(GAME_LIB_DIR)/python/*.cpp $(GAME_LIB_DIR)/math/*.cpp $(GAME_LIB_DIR)/rendering/*.cpp $(GAME_LIB_DIR)/utils/*.cpp $(INCLUDE_DIR)/stb_image/stb_image.cpp)
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
	@$(CC) -c $(CPP_FLAGS) $< -o $@ $(I_FLAGS)

build: $(OBJ) $(OBJ_C)
	@echo "Linking " $@
	@$(CXX) -o $(BUILD_OBJECT) $^ $(I_FLAGS) $(L_FLAGS) $(LIBRARIES)

clean:
ifneq ("$(wildcard $(BUILD_OBJECT))","")
	$(DELETE_CMD) $(BUILD_OBJECT)
endif
ifeq ($(OS_TYPE),windows)
	$(foreach object, $(OBJ), del $(subst /,\,$(object));)
else
	$(foreach object, $(OBJ), rm $(object);)
endif

run:
	./$(BUILD_OBJECT)

format:
	astyle -n --style=google --recursive src/*.h src/*.cpp $(GAME_LIB_DIR)/*.cpp $(GAME_LIB_DIR)/*.h $(GAME_LIB_DIR)/*.hpp
