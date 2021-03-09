#include <Python.h>
//#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
//#include <stdio.h>
#include "someOPs/math2.h"

#include <opencv2/opencv.hpp>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
/*
int main() {

    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.path.append('/Users/shanmu/CLionProjects/project1/cmake-build-debug/project1')");

    PyObject* pModule = PyImport_ImportModule("helloa");
    if (!pModule)
    {
        cout << "Python get module failed." << endl;
        return 0;
    }

    PyObject * pFunc = NULL;
    pFunc = PyObject_GetAttrString(pModule, "Hello");
    PyEval_CallObject(pFunc, NULL);

    PyObject* pv = PyObject_GetAttrString(pModule, "_add");

    PyObject* args = PyTuple_New(2);
    PyObject* arg1 = PyLong_FromLong(4);
    PyObject* arg2 = PyLong_FromLong(3);

    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, arg2);

    PyObject* pRet = PyObject_CallObject(pv, args);

    if (pRet)
    {
        //将返回值转换成long型
        long result = PyLong_AsLong(pRet);
        cout << "result:" << result << endl ;
    }

    PyObject* pModule3 = PyImport_ImportModule("great_module");
    if (!pModule3)
    {
        cout << "Python get module failed." << endl;
        return 0;
    }
    PyObject* args22 = PyTuple_New(1);
    PyObject* arg21 = PyLong_FromLong(99);
    PyTuple_SetItem(args22, 0, arg21);

    PyObject* pRet2 = PyObject_CallObject(pModule3, args22);
    long result2 = PyLong_AsLong(pRet2);


    Py_Finalize();
    return 0;

}
*/

int main(){
    Mat srcImage = imread("/Users/shanmu/Desktop/hh.jpg");
    PyObject* tt = MatToArray(srcImage);
    return 0;
}