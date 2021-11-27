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
