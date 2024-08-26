#include <Python.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <cstdlib>

const std::string D_INTERPRETER_PATH = std::getenv("D_INTERPRETER_PATH");
const std::string D_SOURCE_PATH = std::getenv("D_SOURCE_PATH");

int main(int argc, char *argv[])
{

    // Set Environment Variables
    Py_Initialize();
    PyObject *sys_path = PySys_GetObject("path");
    if(sys_path == nullptr)  {
        std::cerr << "Failed to get sys.path" << std::endl;
        PyErr_Print();
        return 1;
    }
    PyList_Append(sys_path, PyUnicode_FromString(D_SOURCE_PATH.c_str()));
    PyList_Append(sys_path, PyUnicode_FromString(D_INTERPRETER_PATH.c_str()));

    /*
    // Print sys.path
    std::cout << "sys.path:" << std::endl;
    for (Py_ssize_t i = 0; i < PyList_Size(sys_path); ++i) {
        PyObject *path_item = PyList_GetItem(sys_path, i);
        std::cout << "  " << PyUnicode_AsUTF8(path_item) << std::endl;
    }
    */
    
    // Load module
    PyObject *name, *load_module, *func, *callfunc, *args;
    name = PyUnicode_FromString((char*)"script");
    load_module = PyImport_Import(name);
    
    if (load_module == nullptr) {
        PyErr_Print(); 
        Py_Finalize();
        return 1;
    }
    
    // Call function
    func = PyObject_GetAttrString(load_module, (char*)"testFunction");
    callfunc = PyObject_CallObject(func, NULL); 
    double result = PyFloat_AsDouble(callfunc);
    std::cout << "Result: " << result << std::endl;
    Py_Finalize();

    return 0;
}

