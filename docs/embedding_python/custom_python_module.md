# Custom Python Module

## Creating a Game Library

There will be code shared between each lesson and we wouldn't want to copy and paste the same code again and again as that goes against the [DRY](https://en.wikipedia.org/wiki/Don%27t_repeat_yourself) principle.  Moving forward, we will keep all code files that will be shared within the folder `./include/re`.  With that being said we will move our python code (including from the previous section) into `./include/re/python`.

## Python Modules Class

We have successfully called functions from python modules and class instances but we have yet to call C++ code from python.  The best way to do that is to start defining our engine api.  To keep things simple and concise for now we'll just define one python module named `engine` which contains two functions.  The first function `get_version` doesn't take any parameters and will just return a hard coded string that represents our engine version.  Our second function `print_log` will simply take a string as a parameter and print it out to the console.  Now that we have an idea of what to do let's create our class for defining and implementing custom python modules.

```c++
#ifndef PYTHON_MODULES_H
#define PYTHON_MODULES_H

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <string>

class EnginePythonModule {
  public:
    static PyObject* get_version(PyObject* self, PyObject* args);
    static PyObject* print_log(PyObject* self, PyObject* args, PyObject* kwargs);
};

static struct PyMethodDef engineModuleMethods[] = {
    {
        "get_version", EnginePythonModule::get_version,
        METH_VARARGS, "Gets version of the engine."
    },
    {
        "print_log", (PyCFunction) EnginePythonModule::print_log,
        METH_VARARGS | METH_KEYWORDS, "Logs a message to the console."
    },

    {nullptr, nullptr, 0,nullptr },
};

static struct PyModuleDef engineModuleDefinition = {
    PyModuleDef_HEAD_INIT, "engine", nullptr, -1, engineModuleMethods,
    nullptr, nullptr, nullptr, nullptr
};

static char *enginePrintLogKWList[] = {"message", nullptr};

static PyObject* PyInit_engine(void) {
    return PyModule_Create(&engineModuleDefinition);
}

#endif //PYTHON_MODULES_H
```

The `EnginePythonModule` class defines the two functions that we went over earlier.  Notice that `print_log` has an extra parameter for `kwargs` as the parameter will have a keyword named `message`.  After that we  define a struct which contains our module function definitions.  Take note how `print_log` has to cast it's function to `PyCFuntion` to support keyword arguments.  Next we define our module definition for `engine` which we passed in our previously created `engineModuleMethods` struct.  `enginePrintLogKWList` contains the keywords for our `print_log` function.  `message` is passed in by reference so we can set the value to what's passed in as the `message` keyword argument.  Lastly `PyInit_engine` will be used to create our module.  We will return to `PyInit_engine` later as we'll need to import the `engine` module before initializing the python interpreter.

With the header out of the way let's write the implementation of our two functions.

```c++
#include "python_modules.h"
#include <iostream>

PyObject* EnginePythonModule::get_version(PyObject *self, PyObject *args) {
    return Py_BuildValue("s", "v0.0.1");
}

PyObject* EnginePythonModule::print_log(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *message;
    if (PyArg_ParseTupleAndKeywords(args, kwargs, "s", enginePrintLogKWList, &message)) {
        std::cout << "[INFO] " << message << std::endl;
        Py_RETURN_NONE;
    }
    return nullptr;
}
```

`get_version` is really simple as it is just returning a string which contain 'v0.0.1' with `Py_BuildValue`.  The next function `print_log` is slightly more complicated but not by much. We use `PyArg_ParseTupleKeywords` to get the values of the argument passed in.  `enginePrintLogKWList` is passed in as we have the defined `message` as a keyword argument.

With our new module defined and implemented, we have to now import it into the python interpreter.  Update the construtor in `pyhelper.hpp` to add our newly created module with `PyImport_AppendInittab`.  This should be called before `Py_Initialize`.

```c++
CPyInstance() {
	Py_SetProgramName(L"learn_engine_dev");
    PyImport_AppendInittab("engine", &PyInit_engine);
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\".\")");
}
```

*Don't forget to include the header for `python_modules.h`!*

We can finally call C++ code from python. Let's update our `game.py` script to import the `engine` module and call the two functions we defined in C++ earlier.

```py
import engine

class Player:
    def talk(self, message: str) -> None:
        engine_version = engine.get_version()
        engine.print_log(message=f"Engine version = {engine_version}")
```

We're going to keep things easy and just use the same function `talk` that was used in the previous section.  There are a few differences now, the first line imports our `engine` module to be used by the python script.  We're going to ignore the `message` parameter of the `talk` function.  Next we call `engine.get_version()` which returns from our C++ function we defined early a hard coded version string.  We then call `engine.print_log()` to print a log statement to the console.

We will keep our `main` function defined in our C++ code the same as changes aren't needed except to update the path of `PythonObjectManager` header:

```cpp
#include "./re/python/python_object_manager.h"
```

The Final output when we run the engine will be `[INFO] Engine version = v0.0.1`.  All the code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.embedding_python/1.4.create_custom_module).  Now that we have a good foundation for the scripting system, it's time to focus next on creating the game loop.
