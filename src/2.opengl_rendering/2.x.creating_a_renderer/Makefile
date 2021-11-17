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
I_FLAGS := -I"./include" -I"${PYTHON_INCLUDE}"
LIBRARIES := -L"${PYTHON_LIBS}"
C_FLAGS := -w -Wfatal-errors
CPP_FLAGS := $(C_FLAGS) -std=c++14

SRC = $(wildcard src/*.cpp  src/core/utils/*.cpp src/core/math/*.cpp src/core/rendering/*.cpp include/stb_image/stb_image.cpp)
SRC_C = $(wildcard include/glad/glad.c)

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
	astyle -n --style=google --recursive src/*.cpp src/*.h
