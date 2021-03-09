//
// Created by 山姆 on 2021/3/7.
//
#include <opencv2/opencv.hpp>
#include <Python.h>

#include "opencv2/imgproc/imgproc.hpp"
#include </usr/local/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>

using namespace cv;

int f3()
{

    int a = 3;

    return a;
}

PyObject* MatToArray(Mat src) {
    import_array();
    clock_t start, finish;
    start = clock();

    Mat img = src;
    //imread("1.bmp"); // CV_LOAD_IMAGE_COLOR

    PyObject *PythonArray = PyTuple_New(1);

    auto sz = img.size();
    int x = sz.width;
    int y = sz.height;
    int z = img.channels();
    uchar *CArrays = new uchar[x * y * z];
    int iChannels = img.channels();
    int iRows = img.rows;
    int iCols = img.cols * iChannels;

    // if (img.isContinuous())
    // {
    //     iCols *= iRows;
    //     iRows = 1;
    // }

    uchar *p;
    int id = -1;
    for (int i = 0; i < iRows; i++) {
        // get the pointer to the ith row
        p = img.ptr<uchar>(i);
        // operates on each pixel
        for (int j = 0; j < iCols; j++) {
            CArrays[++id] = p[j]; //连续空间
        }
    }

    npy_intp Dims[3] = {y, x, z};
    PyObject *PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);
    PyTuple_SetItem(PythonArray, 0, PyArray);

    finish = clock();
    //cout << "\n赋值为" << (double)(finish - start) / CLOCKS_PER_SEC << "秒！" << endl;

    return PythonArray;

}