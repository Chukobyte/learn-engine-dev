#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argv, char** args) {
    Py_SetProgramName(L"learn_engine_dev");
    Py_Initialize();

    PyRun_SimpleString("print('hello world (from python)')");

    Py_Finalize();

    return 0;
}
