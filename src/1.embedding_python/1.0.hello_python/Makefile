ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
else
    OS_TYPE := linux
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
#I_FLAGS := -I"${PYTHON_HOME}/include"
I_FLAGS := -I"/usr/include/python3.7"
#L_FLAGS := -lmingw32 -lpython37 -static-libgcc -static-libstdc++
L_FLAGS := -static-libgcc -static-libstdc++
C_FLAGS := -w -std=c++14 -Wfatal-errors
LIBRARIES := -L"${PYTHON_HOME}/libs"

PROJECT_NAME := learn_engine_dev
BUILD_OBJECT := $(PROJECT_NAME).exe

SRC = $(wildcard *.cpp)

OBJ = $(SRC:.cpp=.o)

# MAIN

.PHONY: all build clean

all: clean format build

# Compiles if .o is missing or changes to the .cpp file
%.o: %.cpp
	@echo "Compiling " $< " into " $@
	@$(CXX) -c $(C_FLAGS) $< -o $@ $(I_FLAGS)


build: $(OBJ) $(OBJ_C)
	@echo "Linking " $@
	@echo "OS Type = $(OS_TYPE)"
	@$(CXX) -o $(BUILD_OBJECT) $^ $(I_FLAGS) $(L_FLAGS) $(LIBRARIES)

format:
	astyle -n --style=google --recursive *.cpp

clean:
ifneq ("$(wildcard $(BUILD_OBJECT))","")
	del $(BUILD_OBJECT)
endif
	$(foreach object, $(OBJ) $(OBJ_C), @del $(subst /,\, $(object));)

run:
	./$(BUILD_OBJECT)
