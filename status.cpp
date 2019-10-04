/*
 *      Matthew Sanders
 * ============================================================================
 *  cvSorts -- status.cpp
 *      Define the methods of the Status class
 */


#include <utility>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "status.h"

using namespace std;
using namespace cv;

/* ========================================================
 * Status::Status
 *  purpose: Default constructor
 *  input:  edge - edge length, defaulted to 40
 *          del - delay between cycles in ms, defaulted to 100
 *  output: none
 */
Status::Status(int edge, int del)   // edge defaulted to 40, del to 100
{
    edgeLen = edge;
    delay = del;

    data = new Mat(edgeLen, edgeLen, CV_64FC1);
    img  = new Mat(edgeLen, edgeLen, CV_64FC3);

    for(int i = 0; i < edgeLen; i++)
    {
        for(int j = 0; j < edgeLen; j++)
        {
            data->at<double>(i, j) = (rand() % 1000) / 1000.0;
            colorPoint(i, j, data->at<double>(i, j));
        }
    }
}

/* ========================================================
 *  Status::visualSwap
 *      purpose: Conduct the visual movement of swapping two blocks
 *          on the data grid
 *      input: x1/y1 - coords of first block
 *              x2/y2
 *      output: none
 */
void Status::visualSwap(int x1, int y1, int x2, int y2)
{
    // Find travel distance in either dirction (double to avoid later casting)
    double xTravel = x2 - x1, yTravel = y2 - y1;
    // Take the larger of the two for performing the swap so we do not travel
    // more than once block at a time
    double steps = (xTravel > yTravel) ? xTravel : yTravel; 
    int prevX1 = 0, prevY1 = 0, prevX2 = 0, prevY2 = 0;
    // Grab color data for coloring new position
    double color1 = data->at<double>(x1, y1);
    double color2 = data->at<double>(x2, y2);

    for(int i = 0; i < steps; i++)
    {
        // Two blocks will be in motion, for each one calculate its new
        // x and y positions in terms of motion from initial position
        int xMotion1 = static_cast<int>((static_cast<double>(i)/steps) * xTravel) + x1;
        int yMotion1 = static_cast<int>((static_cast<double>(i)/steps) * yTravel) + y1;
        int xMotion2 = x2 - static_cast<int>((static_cast<double>(i)/steps) * xTravel);
        int yMotion2 = y2 - static_cast<int>((static_cast<double>(i)/steps) * yTravel);

        // Recolor positions that in motion blocks are leaving
        resetPoint(prevX1, prevY1);
        resetPoint(prevX2, prevY2);

        // Record current position for later recoloring
        prevX1 = xMotion1;
        prevY1 = yMotion1;
        prevX2 = xMotion2;
        prevY2 = yMotion2;

        // Move blocks to new position
        colorPoint(xMotion1, yMotion1, color1);
        colorPoint(xMotion2, yMotion2, color2);

        // Update the image
        imshow("Data", *img);
		    processInput(waitKeyEx(delay));
    }

    // Movement completed, reset previous position and update image
    resetPoint(prevX1, prevY1);
    resetPoint(prevX2, prevY2);
    imshow("Data", *img);
    processInput(waitKey(delay));
}


/* ========================================================
 * Status::resetPoint
 *  purpose: Return a block which has been altered to its correct color
 *  input:  x, y - x and y positon of point to be reset
 *  output: none
 */
void Status::resetPoint(int x, int y)
{
    colorPoint(x, y, data->at<double>(x, y));
}


/* ========================================================
 * Status::colorPoint
 *  purpose: Color a point based on its value
 *  input:  x, y - x and y positon of point to be colored
 *          color - floating point value of block from which
 *                  color is to be determined
 *  output: none
 */
void Status::colorPoint(int x, int y, double color)
{
    Vec3d &element = img->at<Vec3d>(x, y);
    Vec3d toColor = chromatic(color);
    element[0] = toColor[0];
    element[1] = toColor[1];
    element[2] = toColor[2];
}


/* ========================================================
 * Status::chromatic
 *  purpose: Create a three dimensional color vector from a floating
 *            point value
 *  input:  color - floating point value from which color is to be determined
 *  output: RETURNED - 3d vector with established color
 */
Vec3d Status::chromatic(double color)
{
	if (color < (1.0 / 6.0))
		return Vec3d(1.0, color / (1.0 / 6.0), 0);

	else if (color >= (1.0 / 6.0) && color < (2.0 / 6.0))
		return Vec3d(1.0 - ((color - (1.0 / 6.0)) / (1.0 / 6.0)), 1.0, 0);

	else if (color >= (2.0 / 6.0) && color < (3.0 / 6.0))
		return Vec3d(0, 1.0, (color - (2.0 / 6.0)) / (1.0 / 6.0));

	else if (color >= (3.0 / 6.0) && color < (4.0 / 6.0))
		return Vec3d(0, 1.0 - ((color - (3.0 / 6.0)) / (1.0 / 6.0)), 1.0);

	else if (color >= (4.0 / 6.0) && color < (5.0 / 6.0))
		return Vec3d((color - (4.0 / 6.0)) / (1.0 / 6.0), 0, 1.0);

	else
		return Vec3d(1.0, 0.0, 1.0 - ((color - (5.0 / 6.0)) / (1.0 / 6.0)));
}


/* ========================================================
 * Status::reduceRank / Status::inflateRank
 *  purpose: data is sorted as if it is in a linear array but is displayed in a
 *            two dimensional array, these methods allow for easy conversion
 *            between the two coordinate systems
 *
 * reduceRank
 *  input:  x, y - 2d position to be downranked to 1d
 *  output: RETURNED - 1d position
 */
int Status::reduceRank(int x, int y)
{
    return (x * edgeLen) + y;
}
/*
 * inflateRank
 *  input: rank - 1d position to be converted to 2d
 *  output: RETURNED - std::pair with 2d position
 */
std::pair<int, int> Status::inflateRank(int rank)
{
    return std::pair<int, int>( (rank - (rank % edgeLen) ) / edgeLen ,
                          rank % edgeLen);
}

/* ========================================================
 * Status::swap
 *  purpose: Swap two points in the linear data array and call the visual
 *            swap for the image of the array
 *  input:  a, b - two linear positions to be swapped
 *  output: none
 */
void Status::swap(int a, int b)
{
    int x1 = inflateRank(a).first;
    int y1 = inflateRank(a).second;
    int x2 = inflateRank(b).first;
    int y2 = inflateRank(b).second;

    visualSwap(x1, y1, x2, y2);

    double temp = data->at<double>(x1, y1);
    data->at<double>(x1, y1) = data->at<double>(x2, y2);
    data->at<double>(x2, y2) = temp;

    resetPoint(x1,y1);
    resetPoint(x2,y2);
}

/* ========================================================
 * Status::getValAt
 *  purpose: Return the value for sorting at a given 1d location
 *  input:  loc - location to find value at
 *  output: RETURNED - value from location
 */
double Status::getValAt(int loc)
{
    // Ensure location is in bounds, if not return 0
    if(loc >= (edgeLen * edgeLen))
        return 0.0;

    return data->at<double>(inflateRank(loc).first, inflateRank(loc).second);
}

/* ========================================================
 * Status::operator []
 *  purpose: Return the value for sorting at a given 1d location
 *           (operator to simplify calls)
 *  input:  loc - location to find value at
 *  output: RETURNED - value from location
 */
double Status::operator [] (int loc)
{
    return getValAt(loc);
}

/* ========================================================
 * Status::processInput
 *  purpose: process keyboard input from user
 *  input:  in - integer value of key pressed
 *  output: RETURNED - boolean false if execution should continue
 */
bool Status::processInput(int in)
{
	switch (in)
	{
	case 'q':   // q for quit
	case 'Q':
		exit(0);
	case 's':   // s for slower
  case 'S':
		delay += 2;
		cout << "Delay: " << delay << '\n';
		return false;
	case 'f':   // f for faster
  case 'F':
		delay -= 2;
		if (delay < 2) // Delay less than 2ms reset to 2ms
			delay = 2;
		cout << "Delay: " << delay << '\n';
		return false;
	case 32:    // 32 == SPACE, pause and resume on space
		while (waitKey(40) != 32);
		return false;
	}
	return false;
}
  
/* ========================================================
 * Status::getImg
 *  purpose: Get function for img
 *  input:  none
 *  output: RETURNED - img pointer
 */
Mat* Status::getImg()
{
    return img;
}
  
/* ========================================================
 * Status::getLen
 *  purpose: Get function for edgeLen
 *  input:  none
 *  output: RETURNED - value of edgeLen
 */
int Status::getLen()
{
    return edgeLen;
}

/* ========================================================
 * Status::bubbleSort()
 *  purpose: Perform the bubble sort alogrithm on encapsulated data
 *  input:  none
 *  output: none
 */
void Status::bubbleSort()
{
    int len = edgeLen*edgeLen;
    bool swapped = true;

    while(swapped)
    {
        swapped = false;
        for(int i = 0; i < len-1; i++)
        {
            if(getValAt(i) < getValAt(i+1))
            {
                swap(i, i+1);
                swapped = true;
            }
        }
    }

}

/* ========================================================
 * Status::qspartition
 *  purpose: Partition helper function for quicksort algorithm
 *  input:  low, high - range of data to be partitioned
 *  output: RETURNED - pivot point of partition
 */
int Status::qspartition(int low, int high)
{
  double pivot = getValAt(high);
  int i = (low - 1);

  for(int j = low; j <= high-1; j++)
  {
    if(getValAt(j) <= pivot)
    {
      i++;
      swap(i, j);
    }
  }
  swap(i+1, high);

  return i+1;
}

/* ========================================================
 * Status::quicksort()
 *  purpose: Perform the quick sort alogrithm on encapsulated data
 *  input:  none
 *  output: none
 */
void Status::quicksort(int low, int high)
{
  if(low < high)
  {
    int pi = qspartition(low, high);

    quicksort(low, pi - 1);
    quicksort(pi + 1, high);
  }
}
