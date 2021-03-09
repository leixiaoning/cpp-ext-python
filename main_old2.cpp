#include <iostream>
#include "math1.h"
#include "someOPs/math2.h"
#include <fstream>
#include <opencv2/opencv.hpp>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <Python.h>

#include </usr/local/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>
#include <cstring>
#include "cpp4pymeanshift/msImageProcessor.h"
#include "object.h"

using namespace std;
using namespace cv;



int main() {
// 初始化Python
    //在使用Python系统前，必须使用Py_Initialize对其
    //进行初始化。它会载入Python的内建模块并添加系统路
    //径到模块搜索路径中。这个函数没有返回值，检查系统
    //是否初始化成功需要使用Py_IsInitialized。
    Py_Initialize();

    // 检查初始化是否成功
    if (!Py_IsInitialized()) {
        return -1;
    }

    PyRun_SimpleString("print('hello')");
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs;
    // 载入名为pytest的脚本
    //pName = PyString_FromString("testvideo");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/home/vetec-p/Pan/project/run-maskrcnn')");
    PyRun_SimpleString("sys.path.append('/home/vetec-p/Pan/project/run-maskrcnn/build')");
    PyRun_SimpleString("sys.path.append('/home/vetec-p/Pan/Detectron-master')");
    //    PyRun_SimpleString("print sys.path");

    pModule = PyImport_ImportModule("infer_one_pic");
    if (!pModule) {
        printf("can't find testvideo.py");
        //getchar();
        return -1;
    }
    pDict = PyModule_GetDict(pModule);
    if (!pDict) {
        return -1;
    }
    pFunc = PyDict_GetItemString(pDict, "run");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        printf("can't find function [run]");
        getchar();
        return -1;
    }
    for (int i = 1; i < 200; i++) {
        clock_t start, finish;
        double totaltime;
        start = clock();
        Mat img = imread(
                "/media/vetec-p/Data/Rubbish/maskrcnn_dataset/0803_mask/train_all/pic/" + to_string(i) + ".png");
        if (img.empty())
            return -1;
        //        imshow("img",img);
        //        waitKey(0);

        clock_t s1;
        s1 = clock();
        //PyObject *PyList  = PyList_New(data_size);//定义一个与数组等长的PyList对象数组
        PyObject *ArgList = PyTuple_New(1);


        auto sz = img.size();
        int x = sz.width;
        int y = sz.height;
        int z = img.channels();
        uchar *CArrays = new uchar[x * y * z];//这一行申请的内存需要释放指针，否则存在内存泄漏的问题
        int iChannels = img.channels();
        int iRows = img.rows;
        int iCols = img.cols * iChannels;
        if (img.isContinuous()) {
            iCols *= iRows;
            iRows = 1;
        }

        uchar *p;
        int id = -1;
        for (int i = 0; i < iRows; i++) {
            // get the pointer to the ith row
            p = img.ptr<uchar>(i);
            // operates on each pixel
            for (int j = 0; j < iCols; j++) {
                CArrays[++id] = p[j];//连续空间
            }
        }

        npy_intp Dims[3] = {y, x, z}; //注意这个维度数据！
        PyObject *PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);
        PyTuple_SetItem(ArgList, 0, PyArray);
        clock_t e1 = clock();

        cout << "\n赋值为" << (double) (e1 - s1) / CLOCKS_PER_SEC << "秒！" << endl;

        //PyTuple_SetItem(ArgList, 0, PyList);//将PyList对象放入PyTuple对象中
        PyObject *pReturn = PyObject_CallObject(pFunc, ArgList);
        clock_t e2 = clock();
        //cout<<"\n detect为"<<(double)(e2-e1)/CLOCKS_PER_SEC<<"秒！"<<endl;
        delete[]CArrays;
        CArrays = nullptr;
    }


    Py_DECREF(pModule);

    // 关闭Python
    Py_Finalize();


}