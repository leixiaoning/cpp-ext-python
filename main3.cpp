//
// Created by 山姆 on 2021/3/10.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <python3.9/Python.h>
//#include "/usr/local/Cellar/python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h"
#include <python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>

#include <stdlib.h>
#include "cpp4pymeanshift/msImageProcessor.h"

using namespace std;
using namespace cv;



int main()
{
    //初始化Python环境
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        cout << "Py_Initialize failed." << endl;
        return false;
    }

    PyRun_SimpleString("import sys");
    //添加Insert模块路径
    //PyRun_SimpleString(chdir_cmd.c_str());
    PyRun_SimpleString("sys.path.append('./')");

    //导入模块
    PyObject* pModule = PyImport_ImportModule("/Users/shanmu/CLionProjects/project7hello");


    import_array();
    clock_t start, finish;
    start = clock();

    Mat img = imread("/Users/shanmu/Desktop/hh.jpg");

    PyObject *PythonArray = PyTuple_New(1);

    auto sz = img.size();
    int x = sz.width;
    int y = sz.height;
    int z = img.channels();
    uchar *CArrays = new uchar[x * y * z];
    int iChannels = img.channels();
    int iRows = img.rows;
    int iCols = img.cols * iChannels;


    uchar *p;
    int id = -1;
    for (int i = 0; i < iRows; i++)
    {
        // get the pointer to the ith row
        p = img.ptr<uchar>(i);
        // operates on each pixel
        for (int j = 0; j < iCols; j++)
        {
            CArrays[++id] = p[j]; //连续空间
        }
    }

    npy_intp Dims[3] = {y, x, z};
    PyObject *PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);
    PyTuple_SetItem(PythonArray, 0, PyArray);

    finish = clock();
    double used_time = finish - start;
    if (!pModule)
    {
        cout << "Python get module failed." << endl;
    }


    PyObject* array = NULL;
    PyObject* inputImage = NULL;
    PyArrayObject* segmentedImage = NULL;
    PyArrayObject* labelImage = NULL;
    int radiusS[1];
    double radiusR[1];
    unsigned int minDensity[1];
    unsigned int speedUp[1] = { HIGH_SPEEDUP };

    msImageProcessor imageSegmenter;
    SpeedUpLevel speedUpLevel;
    int* tmpLabels = NULL;
    float* tmpModes = NULL;
    int* tmpModePointCounts = NULL;
    int nbRegions;
    npy_intp dimensions[3];
    int nbDimensions;

    // this 5 param will get from python api
    radiusS[0] = 4;
    radiusR[0] = 4;
    minDensity[0] = 50;
    speedUp[0] = 2;
    array = PyArray; //array = PythonArray;

    inputImage = PyArray_FROM_OTF(array, NPY_UBYTE, NPY_IN_ARRAY);

    if (PyArray_Check(array)) { // this throws seg fault
        cout << "PyArray_Check Passed" << endl;}

    if(PyArray_NDIM(inputImage) == 2)
    {
        nbDimensions = 2;
        dimensions[0] = PyArray_DIM(inputImage, 0);
        dimensions[1] = PyArray_DIM(inputImage, 1);
        imageSegmenter.DefineImage((unsigned char*)PyArray_DATA(inputImage), GRAYSCALE, dimensions[0], dimensions[1]);
    }
    else if(PyArray_NDIM(inputImage) == 3)
    {
        nbDimensions = 3;
        dimensions[0] = PyArray_DIM(inputImage, 0);
        dimensions[1] = PyArray_DIM(inputImage, 1);
        dimensions[2] = 3;
        cout << typeid(inputImage).name() << endl;
        // PyArray_DATA 返回的是pyarray的字符数组
        // strlen(PyArray_DATA(inputImage)); // 等于44*956*3，下面的参数传输的相当是地址
        imageSegmenter.DefineImage((unsigned char*)PyArray_DATA(inputImage), COLOR, dimensions[0], dimensions[1]);
    }
    else
    {
        Py_DECREF(inputImage);
        PyErr_SetString(PyExc_ValueError, "Array must be 2 dimentional (gray scale image) or 3 dimensional (RGB color image)");
        return NULL;
    }

    // Create output images
    segmentedImage = (PyArrayObject *) PyArray_SimpleNew(nbDimensions, dimensions, PyArray_UBYTE);
    if(!segmentedImage)
    {
        Py_DECREF(inputImage);
        return NULL;
    }

    labelImage = (PyArrayObject *) PyArray_SimpleNew(2, dimensions, PyArray_INT);
    if(!labelImage)
        return NULL;

    // Set speedup level
    switch(speedUp[0])
    {
        case 0:
            speedUpLevel = NO_SPEEDUP;
            break;
        case 1:
            speedUpLevel = MED_SPEEDUP;
            break;
        case 2:
            speedUpLevel = HIGH_SPEEDUP;
            break;
        default:
            speedUpLevel = HIGH_SPEEDUP;
    }

    // Segment image and get segmented image
    imageSegmenter.Segment(radiusS[0], radiusR[0], minDensity[0], speedUpLevel);
    imageSegmenter.GetResults((unsigned char*)PyArray_DATA(segmentedImage));

    // Get labels images and number of regions
    nbRegions = imageSegmenter.GetRegions( &tmpLabels, &tmpModes, &tmpModePointCounts);
    memcpy((int*)PyArray_DATA(labelImage), tmpLabels, dimensions[0]*dimensions[1]*sizeof(int));

    // Cleanup
    Py_DECREF(inputImage);
    delete [] tmpLabels;
    delete [] tmpModes;
    delete [] tmpModePointCounts;if(PyArray_NDIM(inputImage) == 2)
    {
        nbDimensions = 2;
        dimensions[0] = PyArray_DIM(inputImage, 0);
        dimensions[1] = PyArray_DIM(inputImage, 1);
        imageSegmenter.DefineImage((unsigned char*)PyArray_DATA(inputImage), GRAYSCALE, dimensions[0], dimensions[1]);
    }
    else if(PyArray_NDIM(inputImage) == 3)
    {
        nbDimensions = 3;
        dimensions[0] = PyArray_DIM(inputImage, 0);
        dimensions[1] = PyArray_DIM(inputImage, 1);
        dimensions[2] = 3;
        imageSegmenter.DefineImage((unsigned char*)PyArray_DATA(inputImage), COLOR, dimensions[0], dimensions[1]);
    }
    else
    {
        Py_DECREF(inputImage);
        PyErr_SetString(PyExc_ValueError, "Array must be 2 dimentional (gray scale image) or 3 dimensional (RGB color image)");
        return NULL;
    }

    // Create output images
    segmentedImage = (PyArrayObject *) PyArray_SimpleNew(nbDimensions, dimensions, PyArray_UBYTE);
    if(!segmentedImage)
    {
        Py_DECREF(inputImage);
        return NULL;
    }

    labelImage = (PyArrayObject *) PyArray_SimpleNew(2, dimensions, PyArray_INT);
    if(!labelImage)
        return NULL;

    // Set speedup level
    switch(speedUp[0])
    {
        case 0:
            speedUpLevel = NO_SPEEDUP;
            break;
        case 1:
            speedUpLevel = MED_SPEEDUP;
            break;
        case 2:
            speedUpLevel = HIGH_SPEEDUP;
            break;
        default:
            speedUpLevel = HIGH_SPEEDUP;
    }

    // Segment image and get segmented image
    imageSegmenter.Segment(radiusS[0], radiusR[0], minDensity[0], speedUpLevel);
    imageSegmenter.GetResults((unsigned char*)PyArray_DATA(segmentedImage));

    // Get labels images and number of regions
    nbRegions = imageSegmenter.GetRegions( &tmpLabels, &tmpModes, &tmpModePointCounts);
    memcpy((int*)PyArray_DATA(labelImage), tmpLabels, dimensions[0]*dimensions[1]*sizeof(int));

    // Cleanup
    Py_DECREF(inputImage);
    delete [] tmpLabels;
    delete [] tmpModes;
    delete [] tmpModePointCounts;
    // return Py_BuildValue("(NNi)", PyArray_Return(segmentedImage), PyArray_Return(labelImage), nbRegions) ;


    Py_Finalize();



    cout << "done" << endl;
    return 0;
}