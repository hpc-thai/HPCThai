#include "python.h"

auto main() -> int
{
    Py_Initialize();
    PyRun_SimpleString("print('Hello, World')");
    Py_Exit(0);
    return 0;
}
