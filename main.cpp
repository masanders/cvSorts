/*
 *      Matthew Sanders
 * ============================================================================
 *  cvSorts -- main.cpp
 *      - Main entry point of cvSorts program
 *      - Create visual display of sorting alorithms
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <string>

#include <opencv2/opencv.hpp>

#include "status.h"
#include "heapsort.h"

using namespace cv;

constexpr auto WINDOW_X = 800;
constexpr auto WINDOW_Y = 800;
constexpr auto WINDOW_NAME = "Data";

// Define sort types
enum SortTypes {
  QUICK, BUBBLE, HEAP};

// Struct to encapsulate user input
typedef struct InputData_ {
  int edge;
  int delay;
  SortTypes type;
} InputData;

// Function prototypes -- See functions for full description
bool procInput(int c, char** v, InputData &input);
void usage(const std::string arg0);
void runSort(InputData &input);

int main(int argc, char** argv)
{
    // Seed rand, explicit cast to suppress warning
    srand(static_cast<unsigned int>(time(NULL)));

    InputData userIn;
    if(!procInput(argc, argv, userIn))
      return 0;

    namedWindow(WINDOW_NAME, WINDOW_NORMAL);
    resizeWindow(WINDOW_NAME, WINDOW_X, WINDOW_Y); 

    runSort(userIn);
    waitKey(0);
}

/* ========================================================
 *  procInput
 *      purpose: Process command line arguments
 *      input: c - argc
 *             v - argv
 *      output: REFERENCE - input - wrapper structure for input
 *              RETURNED  - true if valid input, else false
 */
bool procInput(int c, char** v, InputData &input)
{
  if(c != 4)
  {
    usage(v[0]);
    return false;
  }

  input.edge = atoi(v[1]);
  input.delay = atoi(v[2]);

  if(v[3][0] == 'q' || v[3][0] == 'Q')
    input.type = QUICK;
  else if(v[3][0] == 'b' || v[3][0] == 'B')
    input.type = BUBBLE;
  else if(v[3][0] == 'h' || v[3][0] == 'H')
    input.type = HEAP;
  else
  {
    std::cout << "Sort type " << v[3] << " not recognized, defaulting to quicksort\n\n";
    input.type = QUICK;
  }
  
  return true;
}

/* ========================================================
 *  usage
 *      purpose: Display proper usage to stdout
 *      input: arg0 - the name of this executable as called
 *      output: none
 */
void usage(const std::string arg0)
{
  std::cout << "\nUsage: " << arg0 << " <edge length> <delay> <sort>\n";
  std::cout << "Sorts: quick, bubble, heap\n\n";
}

/* ========================================================
 *  runSort
 *      purpose: Run the sorting algorithm as called
 *      input: input - input data from user
 *      output: none
 */
void runSort(InputData &input)
{
  switch(input.type)
  {
    case QUICK:
      {
        Status status(input.edge, input.delay);
        imshow(WINDOW_NAME, *(status.getImg()));
        status.quicksort(0, (input.edge*input.edge)-1);
        break;
      }
    case BUBBLE:
      {
        Status status(input.edge, input.delay);
        imshow(WINDOW_NAME, *(status.getImg()));
        status.bubbleSort();
        break;
      }
    case HEAP:
      {
        Heapsort status(input.edge, input.delay);
        imshow(WINDOW_NAME, *(status.getImg()));
        status.sort();
        break;
      }
   }
}
