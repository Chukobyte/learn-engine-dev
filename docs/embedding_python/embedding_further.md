# Embedding Further

## PyHelper Class

So far we need to:

* Initialize the python interpreter.
* Create python objects.
* Increment and decrement the reference count for python objects as needed.
* Close the python interpreter once we're finished with it.

With that said, let's create `PyHelper.hpp` which encapsulates this functionality.

```c++
#ifndef PYHELPER_HPP
#define PYHELPER_HPP
#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>

class CPyInstance {
  public:
    CPyInstance() {
        Py_SetProgramName(L"red_engine");
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
    }

    ~CPyInstance() {
        Py_Finalize();
    }
};

class CPyObject {
  private:
    PyObject* pyObj;
  public:
    CPyObject(): pyObj(nullptr) {}

    CPyObject(PyObject* p) : pyObj(p) {}

    ~CPyObject() {
        Release();
    }

    PyObject* GetObj() {
        return pyObj;
    }

    PyObject* SetObj(PyObject* p) {
        return (pyObj=p);
    }

    PyObject* AddRef() {
        if(pyObj) {
            Py_INCREF(pyObj);
        }
        return pyObj;
    }

    void Release() {
        if(pyObj) {
            Py_DECREF(pyObj);
        }
        pyObj = nullptr;
    }

    PyObject* operator->() {
        return pyObj;
    }

    bool Is() const {
        return pyObj ? true : false;
    }

    operator PyObject*() {
        return pyObj;
    }

    PyObject* operator=(PyObject* p) {
        pyObj = p;
        return pyObj;
    }
};

#endif //PYHELPER_HPP

```

There are two classes created in `PyHelper.hpp`.  `CPyInstance` responsibility is to initialize the python interpreter, perform any additional setup, and shutdown the intepreter once finished.  `CPyObject` is a wrapper class for `PyObject` which is a python object.  Instead of having to explicitly decrement with `Py_DECREF` we instead use this `CPyObject` which decrements the reference count once the object is out of scope.



Now that we have a helper class, let's put it to use.

```py
def play(message : str) -> int:
    print(f"{message} (from python)!")
    return 0
```

We have updated our python function to now accept an argument.

```c++
#include "./scripting/pyhelper.hpp"

int main(int argv, char** args) {
    CPyInstance pyInstance;

    // Load Module
    CPyObject pModuleName = PyUnicode_FromString("assets.scripts.game");
    CPyObject pModule = PyImport_Import(pModuleName);
    assert(pModule != nullptr && "Not able to load python module!");

    // Function
    CPyObject pFunc = PyObject_GetAttrString(pModule, "play");
    assert(pFunc != nullptr && "Not able to find function named 'play'!");
    CPyObject pArgs = Py_BuildValue("(s)", "hello world!");
    assert(pArgs != nullptr);
    CPyObject pValue = PyObject_CallObject(pFunc, pArgs);

    return 0;
}
```

This is similar to the code snippet we've created in the previous section, but instead of using `PyObject` we are using `CPyObject`.  `Py_BuildValue` builds a tuple of arguements that we can then pass to a python function.  With the argument defined, we can now call `PyObject_CallObject` with an argument.  If you would like to double check your code you can view the source code for this section [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.embedding_python/1.2.using_pyhelper).

## Creating A Python Instance in C++

We're able to import modules and call functions, but there may be times where we'll want to interact with an instance of a python class.  Furthermore, we don't want to have to import a module and query its attributes each time we want to use a function as that will affect performance.  In this section we'll create a class to manage active python objects.

```c++
#ifndef PYTHON_OBJECT_MANAGER_H
#define PYTHON_OBJECT_MANAGER_H

#include <string>
#include <unordered_map>

#include "./pyhelper.hpp"

struct PythonModuleObject {
    CPyObject module;
    std::unordered_map<std::string, CPyObject> classes;
};

class PythonObjectManager {
  private:
    std::unordered_map<std::string, PythonModuleObject> modules;

    CPyObject GetClass(const std::string &classPath, const std::string &className) {
        if (modules.find(classPath) == modules.end()) {
            CPyObject pModuleName = PyUnicode_FromString(classPath.c_str());
            CPyObject pModule = PyImport_Import(pModuleName);
            assert(pModule != nullptr && "Python module is NULL!");
            modules.emplace(classPath, PythonModuleObject{
                .module = pModule,
                .classes = {}
            });
        }
        if (modules[classPath].classes.find(className) == modules[classPath].classes.end()) {
            CPyObject pModuleDict = PyModule_GetDict(modules[classPath].module);
            CPyObject pClass = PyDict_GetItemString(pModuleDict, className.c_str());
            assert(pClass != nullptr && "Python class is NULL!");
            modules[classPath].classes.emplace(className, pClass);
        }
        return modules[classPath].classes[className];
    }
  public:
    CPyObject CreateClassInstance(const std::string &classPath, const std::string &className) {
        CPyObject pClass = GetClass(classPath, className);
        CPyObject pClassInstance = PyObject_CallObject(pClass, nullptr);
        assert(pClassInstance != nullptr && "Class instance is NULL!");
        pClassInstance.AddRef();
        return pClassInstance;
    }
};

#endif // PYTHON_OBJECT_MANAGER_H
```

Within `python_object_manager.h` we first create `PythonModuleObject` which is a struct to hold a python module's object data.  It will hold all the python class objects loaded for the module.

Next is the main class which is `PythonObjectManager`.  The `GetClass` function will return a class object for a module.  We use this within `CreateClassInstance` to create a python instance of a class.

```py
class Player:
    def talk(self, message: str) -> None:
        print(f"Player says '{message}'!")
```

The script `game.py` has been updated to include a class named `Player`.  We can finally create an instance of this class and call a function from this instance from c++.

```c++
#include "./scripting/python_object_manager.h"

int main(int argv, char** args) {
    CPyInstance pyInstance;
    PythonObjectManager pObjectManager;

    CPyObject pClassInstance = pObjectManager.CreateClassInstance("assets.scripts.game", "Player");
    PyObject_CallMethod(pClassInstance, "talk", "(s)", "Hello!");

    return 0;
}
```

There is even less code in `main` even though we're creating an instance!  The only thing to really point out is `PyObject_CallMethod` which calls a function on an instance of a class.  Source code for this section can be viewed [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.embedding_python/1.3.create_python_instance).  Now that we have a solid grasp on how to create python instances and call functions on them it's time to create our own modules in C++ in order to call the ***Red Engine*** api from within python scripts.
