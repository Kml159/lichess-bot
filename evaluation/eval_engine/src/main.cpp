#include <Python.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <cstdlib>
#include <vector>

const char *D_INTERPRETER_PATH = std::getenv("D_INTERPRETER_PATH");
const char *D_SOURCE_PATH = std::getenv("D_SOURCE_PATH");

namespace py
{

    PyObject *name = nullptr;
    PyObject *load_module = nullptr;

    void initializePython()
    {
        // Check if env. vars valid
        if (D_INTERPRETER_PATH == nullptr || D_SOURCE_PATH == nullptr)
        {
            throw std::runtime_error("---> Environment variables D_INTERPRETER_PATH or D_SOURCE_PATH are not set");
        }

        // Set env. vars
        Py_Initialize();
        PyObject *sys_path = PySys_GetObject("path");
        if (sys_path == nullptr)
        {
            PyErr_Print();
            throw std::runtime_error("---> Failed to get sys.path");
        }
        PyList_Append(sys_path, PyUnicode_FromString(D_SOURCE_PATH));
        PyList_Append(sys_path, PyUnicode_FromString(D_INTERPRETER_PATH));

        // Load module
        name = PyUnicode_FromString((char *)"script");
        load_module = PyImport_Import(name);

        if (name == nullptr || load_module == nullptr)
        {
            PyErr_Print();
            throw std::runtime_error("---> Failed to load module");
        }
    }

    void finalizePython()
    {
        // Set reference counts to 0 for python gc
        Py_SET_REFCNT(name, 0);
        Py_SET_REFCNT(load_module, 0);
        Py_Finalize();
    }

    void testFunction()
    {
        // Call function
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"testFunction");
        PyObject *callfunc = PyObject_CallObject(func, NULL);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in testFunction");
        }
        double result = PyFloat_AsDouble(callfunc);
        std::cout << "Result: " << result << std::endl;

        // Set reference counts to 0 for python gc,
        Py_SET_REFCNT(callfunc, 0);
        Py_SET_REFCNT(func, 0);
    }

    bool isCheckmate(const std::string fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_checkmate");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in isCheckmate");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_SET_REFCNT(callfunc, 0);
        Py_SET_REFCNT(args, 0);
        Py_SET_REFCNT(func, 0);

        return res;
    }

    // Unchecked !!!
    bool isStalemate(const std::string fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_stalemate");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in is_stalemate");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_SET_REFCNT(callfunc, 0);
        Py_SET_REFCNT(args, 0);
        Py_SET_REFCNT(func, 0);

        return res;
    }

    std::vector<std::string> generateLegalMoves(const std::string fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"generate_legal_moves");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        PyObject *tupleMoves = PyList_AsTuple(callfunc);

        // Convert python tuple to vector
        std::vector<std::string> res;
        
        for (Py_ssize_t i = 0; i < PyTuple_GET_SIZE(tupleMoves); i++)
        {
            PyObject *item = PyTuple_GetItem(tupleMoves, i);
            std::string move(PyUnicode_AsUTF8(item));
            res.push_back(move);

            // Set reference counts
            Py_SET_REFCNT(item, 0);
        }

        // Check for errors
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in generateLegalMoves");
        }

        // Set reference counts
        Py_SET_REFCNT(callfunc, 0);
        Py_SET_REFCNT(args, 0);
        Py_SET_REFCNT(func, 0);
        Py_SET_REFCNT(tupleMoves, 0);

        return res;
    }

    std::string makeMove(const std::string fenBoard, const std::string move)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"make_move");
        PyObject *args = PyTuple_Pack(2, PyUnicode_FromString(fenBoard.c_str()), PyUnicode_FromString(move.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in makeMove");
        }
        std::string res = PyUnicode_AsUTF8(callfunc);

        // Set reference counts
        Py_SET_REFCNT(callfunc, 0);
        Py_SET_REFCNT(args, 0);
        Py_SET_REFCNT(func, 0);

        return res;
    }

    void printAllMoves(const std::vector<std::string> &moves)
    {
        for (int i = 0; i < moves.size(); i++)
        {
            std::cout << moves[i] << std::endl;
        }
    }

}

int main(int argc, char *argv[])
{
    try
    {
        py::initializePython();
        std::string fenBoard = "8/1Bpn2K1/p2Q4/1P2P2q/5p2/1R6/1rp3P1/k5N1 w - - 0 1";

        std::vector<std::string> moves = py::generateLegalMoves(fenBoard);
        py::printAllMoves(moves);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        py::finalizePython();
    }

    py::finalizePython();
    return 0;
}
