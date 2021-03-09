#include <iostream>
#include "math1.h"
#include "someOPs/math2.h"
#include <fstream>
#include <opencv2/opencv.hpp>

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <Python.h>
//#include </System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/Python.h>
//#include </usr/local/Cellar//python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9/include/python3.9/Python.h>

//#include <numpy/arrayobject.h>
#include </usr/local/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>
#include <cstring>
#include "cpp4pymeanshift/msImageProcessor.h"

using namespace std;
using namespace cv;
/*
int main() {
    cout << "Hello, World!" << endl;

    int num;

    f1();
    f2();

    num = f3();

    char data[100];
    // 以写模式打开文件
    ofstream outfile;
    outfile.open("afile.dat");
    cout << "Writing to the file" << endl;
    cout << "Enter your name: ";
    cin.getline(data, 100);
    // 向文件写入用户输入的数据
    outfile << data << endl;
    cout << "Enter your age: ";
    cin >> data;
    cin.ignore();
    outfile << data << endl;
    // 关闭打开的文件
    outfile.close();

    // 以读模式打开文件
    ifstream infile;
    infile.open("afile.dat");

    cout << "Reading from the file" << endl;
    infile >> data;

    // 在屏幕上写入数据
    cout << data << endl;

    // 再次从文件读取数据，并显示它
    infile >> data;
    cout << data << endl;

    // 关闭打开的文件
    infile.close();

    cout << "done for executing code" << endl;
    return 0;
}
*/

/*
int main()
{
    Mat srcImage = imread("/Users/shanmu/Desktop/hh.jpg");
    if (!srcImage.data) {
        std::cout << "Image not loaded";
        return -1;
    }

    cout << "show" << endl;

    imshow("[img]", srcImage);
    waitKey(0);

    cout << "ok" << endl;
    return 0;
}
 */

int main()
{
    // c++ 让 python 运行相应的语句
    //PyRun_SimpleString("print 'hello'");
    //PyRun_SimpleString("import sys");


    Mat srcImage = imread("/Users/shanmu/Desktop/hh.jpg");
    cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);

    int row = srcImage.rows;
    int col = srcImage.cols;

    //动态创建二维数组，row行col列
    int **La = new int *[row];
    for (int i = 0; i < row; i ++){
        La[i] = new int[col];
    }
    // 循环二维数组和mat，并将mat对应值赋给二维数组对应值，
    for (int i = 0; i < row; i ++){
        for (int j = 0; j < col; j ++){
            La[i][j] = srcImage.at<uchar>(i, j);
        }
    }

    PyObject* return2py = NULL;
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

    //if (!PyArg_ParseTuple(args, "OidI|I", &array, &radiusS, &radiusR, &minDensity, &speedUp))
    //   return NULL;


    //array = PyArray_Return(srcImage);
    radiusS[0] = 4;
    radiusR[0] = 4;
    minDensity[0] = 50;
    speedUp[0] = 2;
    inputImage = PyArray_FROM_OTF(array, NPY_UBYTE, NPY_IN_ARRAY);

    if(inputImage == NULL)
        cout << "null, should return something" << endl;

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

    // Return a tuple with the segmented image, the label image, and the number of regions
    return2py = Py_BuildValue("(NNi)", PyArray_Return(segmentedImage), PyArray_Return(labelImage), nbRegions);



    cout << "done" << endl;
    return 0;
}