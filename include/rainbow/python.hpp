#ifndef RAINBOW_PYTHON_HPP
#define RAINBOW_PYTHON_HPP

#include <Python.h>
#include <vector>
#include <iostream>

namespace rb {

// Some basic functions
inline PyObject* py2val(int val) { return PyLong_FromLong(val); }
inline PyObject* py2val(const char* val) { return PyUnicode_FromString(val); }
inline PyObject* py2val(float val) { return PyFloat_FromDouble(val); }

template <class T>
struct Python_result {
    T value;
    bool valid;
    static Python_result<T> invalid()
    {
        Python_result<T> pr;
        pr.valid = false;
        return pr;
    }
};

template <>
struct Python_result<void> {
    bool valid;
    static Python_result<void> invalid()
    {
        Python_result<void> pr;
        pr.valid = false;
        return pr;
    }
};

typedef std::vector<PyObject*> Python_arguments;

class Python_module {
public:
    PyObject* module;

    Python_module() = default;

    Python_module(PyObject* module)
        : module(module) { }

    ~Python_module()
    {
        if (module != nullptr)
            Py_DECREF(module);
    }

    template <class R>
    bool operator()(Python_result<R>* result, PyObject* function, 
        Python_arguments& arguments)
    {
        size_t argc = arguments.size();
        PyObject* args = PyTuple_New(argc);
        PyObject* value;
        for (int i = 0; i < argc; i++) {
            value = arguments[i];
            if (value == nullptr) {
                Py_DECREF(args);
                std::cerr << "Cannot convert argument: " << i
                          << " for function: " << function << std::endl;
                *result = Python_result<R>::invalid();
                return false;
            }
            PyTuple_SetItem(args, i, value);
        }
        Py_DECREF(value);
        PyObject* returned = PyObject_CallObject(function, args);
        Py_DECREF(args);
        Py_DECREF(returned);
        return true;
    }

    template <class R, class T, typename... Args>
    bool operator()(Python_result<R>* result, PyObject* function,
            Python_arguments& arguments, T value, Args... args)
    {
        arguments.push_back(py2val(value));
        return this->operator()(result, function, arguments, args...);
    }

    template <class R, class T, typename... Args>
    bool operator()(Python_result<R>* result, 
            const std::string& function, T value, Args... args)
    {
        bool valid = true;
        PyObject* pyfunc = PyObject_GetAttrString(module, function.c_str());
        if (pyfunc == nullptr || !PyCallable_Check(pyfunc)) {
            std::cerr << "Can not find function: " << function << std::endl;
            if (PyErr_Occurred()) {
                PyErr_Print();
            }
            *result = Python_result<R>::invalid();
            valid = false;
        }

        if (valid) {
            Python_arguments arguments;
            arguments.push_back(py2val(value));
            valid = this->operator()(result, pyfunc, arguments, args...);
        }

        Py_XDECREF(pyfunc);
        return valid;
    }
};

class Python {
public:
    Python();
    ~Python();
    bool operator()(Python_module* save, const std::string& module)
    {
        PyObject* py_module_name = PyUnicode_FromString(module.c_str());
        if (py_module_name == nullptr) {
            std::cerr << "Failed to create unicode string" << std::endl;
            return false;
        }

        PyObject* py_module = PyImport_Import(py_module_name);
        Py_DECREF(py_module_name);

        if (py_module == nullptr) {
            PyErr_Print();
            std::cerr << "Failed to load: " << module.c_str() << std::endl;
            return false;
        }

        *save = Python_module(py_module);
        return true;
    }
};

}

#endif // RAINBOW_PYTHON_HPP
