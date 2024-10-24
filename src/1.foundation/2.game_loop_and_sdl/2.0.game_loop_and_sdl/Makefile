PROJECT_NAME := red_engine

# OS Specific Stuff
ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
    BUILD_OBJECT := $(PROJECT_NAME).exe
    L_FLAGS := -lmingw32 -lSDL2main -lSDL2 -lpython37 -static-libgcc -static-libstdc++
    DELETE_CMD := del
else
    OS_TYPE := linux
    BUILD_OBJECT := $(PROJECT_NAME)
    L_FLAGS := -lSDL2 -lGL -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm -static-libgcc -static-libstdc++
    DELETE_CMD := rm
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
INCLUDE_DIR := ../../../../include
GAME_LIB_DIR := $(INCLUDE_DIR)/re
I_FLAGS := -I"$(INCLUDE_DIR)" -I"${SDL2_INCLUDE}" -I"${PYTHON_INCLUDE}"
LIBRARIES := -L"${SDL2_LIBS}" -L"${PYTHON_LIBS}"
CPP_FLAGS := -w -Wfatal-errors -std=c++14

SRC = $(wildcard src/*.cpp $(GAME_LIB_DIR)/game_engine_context.cpp $(GAME_LIB_DIR)/python/*.cpp $(GAME_LIB_DIR)/utils/logger.cpp $(GAME_LIB_DIR)/utils/timer.cpp)

OBJ = $(SRC:.cpp=.o)

# MAIN

.PHONY: all build clean

all: clean format build

# Compiles if .o is missing or changes to the .cpp file
%.o: %.cpp
	@echo "Compiling " $< " into " $@
	@$(CXX) -c $(CPP_FLAGS) $< -o $@ $(I_FLAGS)

build: $(OBJ)
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
	@./$(BUILD_OBJECT)

format:
	@astyle -n --style=google --recursive src/*.h src/*.cpp $(GAME_LIB_DIR)/*.cpp $(GAME_LIB_DIR)/*.h $(GAME_LIB_DIR)/*.hpp

# Adds debug symbols for debugger
debug-build: C_FLAGS +=  -g
debug-build: build
