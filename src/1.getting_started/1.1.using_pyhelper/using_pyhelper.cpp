#include "./pyhelper.hpp"

int main(int argv, char** args) {
    CPyInstance pyInstance;
    PyRun_SimpleString("print('hello world (from python)')");

    return 0;
}
