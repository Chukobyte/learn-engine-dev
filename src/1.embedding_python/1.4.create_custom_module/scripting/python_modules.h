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
