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
