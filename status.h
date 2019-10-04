/*
 *      Matthew Sanders
 * ============================================================================
 *  cvSorts -- status.h
 *      Declare the Status class which will encapsulate data to be sorted and
 *      it's associated image data
 */

#ifndef STATUS_H
#define STATUS_H

#include <utility>

#include <opencv2/opencv.hpp>

using namespace cv;

constexpr auto UP_ARROW = 2490368;
constexpr auto DOWN_ARROW = 2621440;

class Status {
public:
    Status(int edge = 40, int del = 100);

    void visualSwap(int x1, int y1, int x2, int y2);
    void resetPoint(int x, int y);
    void colorPoint(int x, int y, double color);
    Vec3d chromatic(double color);
    int reduceRank(int x, int y);
    std::pair<int, int> inflateRank(int rank);
    void swap(int a, int b);
    double getValAt(int loc);
    double operator [] (int loc);
  	bool processInput(int in);

    Mat* getImg();
    int  getLen();

    void bubbleSort();
    void quicksort(int low, int high);
    int qspartition(int low, int high);

protected:
    int edgeLen;
    int delay;

    Mat *img;
    Mat *data;
};

#endif /* STATUS_H */
