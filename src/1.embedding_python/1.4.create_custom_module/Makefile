PROJECT_NAME := learn_engine_dev

# OS Specific Stuff
ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
    BUILD_OBJECT := $(PROJECT_NAME).exe
    L_FLAGS := -lmingw32 -lpython37 -static-libgcc -static-libstdc++
else
    OS_TYPE := linux
    BUILD_OBJECT := $(PROJECT_NAME)
    L_FLAGS := -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm -static-libgcc -static-libstdc++
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
INCLUDE_DIR := ../../../include
GAME_LIB_DIR := $(INCLUDE_DIR)/game_lib
I_FLAGS := -I"${INCLUDE_DIR}" -I"${PYTHON_INCLUDE}"
LIBRARIES := -L"${PYTHON_LIBS}"
CPP_FLAGS := -w -std=c++14 -Wfatal-errors

SRC = $(wildcard src/*.cpp $(GAME_LIB_DIR)/python/*.cpp)

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
	astyle -n --style=google --recursive src/*.cpp src/*.hpp src/*.h
