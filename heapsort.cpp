/*
 *      Matthew Sanders
 * ============================================================================
 *  cvSorts -- heapsort.h
 *      Define the methods of the Heapsort class
 */

#include <cmath>
#include <functional>

#include "heapsort.h"

/* ========================================================
 *  Heapsort::siftDown
 *      purpose: sift a value down in the heap
 *      input: start - where value to sift down is
 *              end - effective end of heap
 *              comp - comparator to use for sifting
 *      output: none
 */
void Heapsort::siftDown(const int start, const int end,
    std::function<bool(const double, const double)> comp)
{
    int root = start;

    while(left(root) < end)
    {
        int child = left(root);
        int move = root;

        if( comp( (*this)[move], (*this)[child]) )
            move = child;
        if(child+1 < end)
            if( comp( (*this)[move], (*this)[child+1]) )
                move = child+1;
        if(move == root)
            return;

        swap(root, move);
        root = move;
    }
}

/* ========================================================
 *  Heapsort::heapify
 *      purpose: Perform the heapify opeartion on the heap, ie. perform
 *          siftDown on each item from the middle to the start
 *      input: none
 *      output: none
 */
void Heapsort::heapify()
{
    for(int i = parent(len-1); i >= 0; i--)
        siftDown(i, len);
}

/* ========================================================
 *  Heapsort::sort
 *      purpose: sift a value down in the heap
 *      input: none
 *      output: none
 */
void Heapsort::sort()
{
    this->heapify();
    for(int i = len-1; i > 0; i--)
    {
        swap(i, 0);
        siftDown(0, i-1);
    }
}

/* ========================================================
 *  Heapsort::left/right/parent
 *      purpose: Return the left/right child or parent node of the passed node
 *      input: i - node to find child or parent of
 *      output: left/right child or parent
 */
int Heapsort::left(const int i) const
{
    return (i * 2) + 1;
}
int Heapsort::right(const int i) const
{
    return (i * 2) + 2;
}
int Heapsort::parent(const int i) const
{
    return static_cast<int>(floor((i - 1) / 2));
}

/* ========================================================
 *  Heapsort::report
 *      purpose: Print a report of the heap's constants
 *      input: none
 *      output: none
 */
void Heapsort::report()
{
  std::cout << "edgeLen: " << edgeLen << " - delay: " << delay
        << " - len: " << len << '\n';
}
