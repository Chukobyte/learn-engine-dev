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
        Py_SetProgramName(L"learn_engine_dev");
        Py_Initialize();
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

There are two classes created in `PyHelper.hpp`.  `CPyInstance` responsibility is to initialize the python interpreter, perform any additional setup, and shutdown the intepreter once finished.  `CPyObject` is wrapper class for `PyObject` which is a python object.  Instead of having to explicitly decrement with `Py_DECREF` we instead use this `CPyObject` which decrements the reference count once the object is out of scope.



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

This is similar to the code snippet we've created in the previous section, but instead of using `PyObject` we are using `CPyObject`.  `Py_BuildValue` builds a tuple of arguements that we can then pass to a python function.  With the argument defined, we can now call `PyObject_CallObject` with an argument.