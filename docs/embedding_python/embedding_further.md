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

More coming soon.
