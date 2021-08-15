# Hello Python

## Dependencies

For this tutorial, we'll be using Python 3.7 but feel free to try other versions as well.  Python must be downloaded and installed locally before continuing.  Windows users be sure to include the `python37.dll`  within root directory of each chapter.

## Build

For building the application we will be using [make](https://www.gnu.org/software/make/).  To make the paths for dependencies configurable we'll set environment variables.  `PYTHON_INCLUDE` should be set to the path of the python 3.7 installation's include folder.  `PYTHON_LIBS` should be set to the python3.7 installation's lib folder.  The Makefile should look like the following:

```makefile
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
```

*You can remove the `@` from in fron of the compile and build target commands to view the full command.*

## Hello World

It's time to finally write python code within our engine!  Before we start diving deeper into creating a scripting system for python, let's first obtain a small win by having out engine print out a statement via python.  Full documentation for embedding python within a c++ application can be found [here](https://docs.python.org/3/extending/embedding.html).

```c++
#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"learn_engine_dev");
    Py_Initialize();

    PyRun_SimpleString("print('hello world (from python)')");

    Py_Finalize();

    return 0;
}
```

This should print out *'hello world (from python)'*.  There's other things we can setup before initializing the python interpreter but for now we'll just set the python application name with `Py_SetProgramName`. `Py_Initialize` initializes the python interpreter.

Next we call `PyRun_SimpleString` to execute this python statement:
```py
print('hello world (from python)')
```

Last but not least we terminate the python interpreter by calling `Py_Finalize()`.  Not too bad of a start but we can definitely do better.  You view the code by looking at the source [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.embedding_python/1.0.hello_python).

We will want to execute scripts and python functions from our engine and also be able to call c++ functions from a python script.  Let's work on that next!

## Execute A Python Function From C++

Executing python statements with `PyRun_SimpleString` is simple but it won't scale well once we start adding more logic in python.  Let's create a new python script and call a function from it!

```py
def play() -> int:
    print("Game played!")
    return 0
```

A simple python function that prints the statement *'Game played!'* to the console.  Next let's call this function in c++.

```c++
#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"learn_engine_dev");
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");

    // Load Module
    PyObject* pModuleName = PyUnicode_FromString("assets.scripts.game");
    PyObject* pModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);
    assert(pModule != nullptr && "Not able to load python module!");

    // Function
    PyObject* pFunc = PyObject_GetAttrString(pModule, "play");
    assert(pFunc != nullptr && "Not able to find function named 'play'!");
    PyObject* pValue = PyObject_CallObject(pFunc, nullptr);
    std::cout << "Python script exit code = " << PyLong_AsLong(pValue) << std::endl;

    Py_DECREF(pValue);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    Py_Finalize();

    return 0;
}
```

This is pretty straightforward, the first things to point out are the two `PyRun_SimpleString` calls.  This is to add the current path to the system path in order for the python module paths to be relative to the current path.  Next we're getting a python string object with `PyUnicode_FromString` as we'll need that to import the python module from our script.  The folder path is `assets/scripts` and the python script is `game.py`.  Next we'll import the module with `PyImport_Import`, which thanks to the appending the current path to the system path we're able to find without including the full path.  `Py_DECREF` is called as we'll need to decrement the reference count of python objects to delete the objects in the python interpreter and prevent memory leaks!

Now that we have imported the module we can get the reference to a function with `PyObject_GetAttrString`.  With this reference we'll call the function with `PyObject_CallObject`.  The function `play` returns an integer and we store this within `pValue`.  After that we just decrement the remaining python objects we no longer need.  The source for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.embedding_python/1.1.calling_a_function).

We're able to call a python function from c++ and receive it's return value, but incrementing and decrementing the reference count will become tedious.  Next we will embed python further into our engine and create classes to generalize some of our usages of the Python API.
