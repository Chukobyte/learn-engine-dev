PROJECT_NAME := learn_engine_dev

# OS Specific Stuff
ifeq ($(OS),Windows_NT)
    OS_TYPE := windows
    BUILD_OBJECT := $(PROJECT_NAME).exe
    L_FLAGS := -lmingw32 -lpython37 -static-libgcc -static-libstdc++
    DELETE_CMD := del
else
    OS_TYPE := linux
    BUILD_OBJECT := $(PROJECT_NAME)
    L_FLAGS := -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm -static-libgcc -static-libstdc++
    DELETE_CMD := rm
endif

CC := gcc # C Compiler
CXX := g++ # C++ compiler
I_FLAGS := -I"${PYTHON_INCLUDE}"
LIBRARIES := -L"${PYTHON_LIBS}"
C_FLAGS := -w -std=c++14 -Wfatal-errors



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
	@$(CXX) -o $(BUILD_OBJECT) $^ $(I_FLAGS) $(L_FLAGS) $(LIBRARIES)

clean:
ifneq ("$(wildcard $(BUILD_OBJECT))","")
	$(DELETE_CMD) $(BUILD_OBJECT)
endif
	$(foreach object, $(OBJ) $(OBJ_C), @$(DELETE_CMD) $(subst /,\, $(object));)

run:
	./$(BUILD_OBJECT)
