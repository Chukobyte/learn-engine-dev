# Hello Python

## Dependencies

For this tutorial, we'll be using Python 3.7 but feel free to try other versions as well.  Python must be downloaded and installed locally before continuing.  Windows users be sure to include the `python37.dll` in the same directory as the `red_engine` executable.

## Build

For building the engine in a cross platform way we will be using [make](https://www.gnu.org/software/make/).  I won't being going into the specifics of Makefiles in this series, but you can find a pretty solid tutorial [here](https://makefiletutorial.com/).  To make the paths for dependencies configurable we'll set environment variables throughout the tutorial, but feel free to hardcode these values for your convenience.

For python there are two environment variables:

* `PYTHON_INCLUDE` The python installation's include folder.  Should contain `Python.h`.
* `PYTHON_LIBS` The python installation's lib folder.  Should contain the python `a.` file used during linking.

```makefile
PROJECT_NAME := red_engine

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
CPP_FLAGS := -w -std=c++14 -Wfatal-errors

SRC = $(wildcard src/*.cpp)

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
    astyle -n --style=google --recursive src/*.cpp
```

*Note: You can echo the full compile and linking commands to the console by removing `@` on lines 51 and 55.*

## Hello World

It's time to finally write python code within ***Red Engine***!  Before we start diving deeper into creating a scripting system for python, let's first obtain a small win by having out engine print out a statement via python.  Full documentation for embedding python within a c++ application can be found [here](https://docs.python.org/3/extending/embedding.html).

```c++
#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"red_engine");
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

Last but not least we terminate the python interpreter by calling `Py_Finalize()`.  Not too bad of a start but we can definitely do better!  You view the code by looking at the source [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/1.embedding_python/1.0.hello_python).

We will want to execute scripts and python functions from ***Red Engine*** and also be able to call C++ functions from a python script.  Let's work on that next!

## Execute A Python Function From C++

Executing python statements with `PyRun_SimpleString` is simple but it won't scale very well once we start adding more logic in python.  Let's create a new python script and call a function from it!

```py
def play() -> int:
    print("Game played!")
    return 0
```

A simple python function that prints the statement *'Game played!'* to the console.  Next let's call this function in C++.

```c++
#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"red_engine");
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

This is pretty straightforward, the first things to point out are the two `PyRun_SimpleString` calls.  The first python statement:

```py
import sys
```

Simply imports the `sys` module into the interpreter.  The next python statement:
```py
sys.path.append(".")
```

This adds the current path to the system path in order for python modules to be imported based on the current path.  We would have to provide the full path otherwise.

Next we're getting a python string object with `PyUnicode_FromString` as we'll need that to import the python module from our script.  The folder path is `assets/scripts` and the python script is `game.py`.  After getting the module name we'll now import the module it's pointing to with `PyImport_Import`.  `Py_DECREF` is called as we'll need to decrement the reference count of python objects to delete them within python interpreter and prevent memory leaks!

Now that we have imported the module we can get the reference to a function with `PyObject_GetAttrString`.  With this reference we'll call the function with `PyObject_CallObject`.  The function `play` returns an integer and we store this within `pValue`.  After that we just decrement the remaining python objects we no longer need.  The source for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/1.embedding_python/1.1.calling_a_function).

We're able to call a python function from c++ and receive its return value, but incrementing and decrementing the reference count will become tedious.  In the next section we will embed python further into ***Red Engine*** and create classes to generalize some of our usages of the Python API.
