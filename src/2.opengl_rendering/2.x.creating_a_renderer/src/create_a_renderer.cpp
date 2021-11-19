#include "./game_lib/python/python_object_manager.h"
#include "./game_lib/rendering/texture.h"

int main(int argv, char** args) {
    CPyInstance pyInstance;
    PythonObjectManager pObjectManager;

    CPyObject pClassInstance = pObjectManager.CreateClassInstance("assets.scripts.game", "Player");
    PyObject_CallMethod(pClassInstance, "talk", "(s)", "Hello!");

    return 0;
}
