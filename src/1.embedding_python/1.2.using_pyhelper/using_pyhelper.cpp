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
