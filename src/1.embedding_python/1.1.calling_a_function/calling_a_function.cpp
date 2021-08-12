#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"learn_engine_dev");
    Py_Initialize();

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
