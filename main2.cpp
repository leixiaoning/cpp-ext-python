//
// Created by 山姆 on 2021/3/10.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <python3.9/Python.h>
//#include "/usr/local/Cellar/python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h"
#include <python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>

using namespace std;
using namespace cv;

/*
int main(int argc, char *argv[])
{
    //wchar_t *program = Py_DecodeLocale(argv[0], nullptr);

    //if ( program == nullptr ){
    //    std::cout << "Fatal Error: cannot decode argv[0]!" << std::endl;
    //    return -1;
    //}
    //Py_SetProgramName(program);
    Py_Initialize();

    PyRun_SimpleString("print('hello world!')");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");


    PyObject* pModule = PyImport_ImportModule("hello");
    if ( pModule == nullptr ){
        std::cout << "module not found!" << std::endl;
        return 1;
    }

    Py_Finalize();
    //PyMem_RawFree(program);


    cout << "ok" << endl;
    return 0;
}
*/

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], nullptr);
    if ( program == nullptr ){
        std::cout << "Fatal Error: cannot decode argv[0]!" << std::endl;
        return -1;
    }
    Py_SetProgramName(program);
    Py_SetPythonHome((wchar_t*)L"/usr/local/Cellar/python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9");
    Py_Initialize();
    if ( !Py_IsInitialized() ){
        std::cout << "Python init failed!" << std::endl;
        return 0;
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    PyObject* pModule = PyImport_ImportModule("hello");
    if ( pModule == nullptr ){
        std::cout << "module not found!!!" << std::endl;
        return 1;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, "say");
    if ( !pFunc || !PyCallable_Check(pFunc) ){
        std::cout << "not found function add_num!" << std::endl;
        return 2;
    }
    PyObject_CallObject(pFunc, nullptr);

    if ( Py_FinalizeEx() < 0 ){
        exit(120);
    }
    PyMem_RawFree(program);
    return 0;
}

