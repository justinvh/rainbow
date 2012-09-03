#include <Python.h>
#include <rainbow/python.hpp>

using namespace rb;

Python::Python()
{
    Py_Initialize();
    PyRun_SimpleString("import sys"); 
    PyRun_SimpleString("sys.path.append(\"game/python/\")");
}

Python::~Python()
{
    Py_Finalize();
}
