#include <iostream>
#include <opencv2/opencv.hpp>
#include <python3.9/Python.h>
//#include "/usr/local/Cellar/python@3.9/3.9.2_1/Frameworks/Python.framework/Versions/3.9/lib/python3.9/site-packages/numpy/core/include/numpy/arrayobject.h"
#include <python3.9/site-packages/numpy/core/include/numpy/arrayobject.h>

using namespace std;
using namespace cv;


int main() {
    cout << "Hello, World!" << endl;
    Mat srcImage = imread("/Users/shanmu/Desktop/hh.jpg");

    PyObject* return2py = NULL;
    PyObject* array = NULL;
    PyObject* inputImage = NULL;
    PyArrayObject* segmentedImage = NULL;
    PyArrayObject* labelImage = NULL;
    int radiusS[1];
    double radiusR[1];
    unsigned int minDensity[1];
    /*
    unsigned int speedUp[1] = { HIGH_SPEEDUP };
    msImageProcessor imageSegmenter;
    SpeedUpLevel speedUpLevel;
    */

    int* tmpLabels = NULL;
    float* tmpModes = NULL;
    int* tmpModePointCounts = NULL;
    int nbRegions;
    npy_intp dimensions[3];
    int nbDimensions;

    radiusS[0] = 4;
    radiusR[0] = 4;
    minDensity[0] = 50;


    cout << "ok!" << endl;
    return 0;
}
