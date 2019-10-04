/*
 *      Matthew Sanders
 * ============================================================================
 *  cvSorts -- heapsort.h
 *      Declare the heapsort class which inherits the Status class and allows
 *      for performance of the heap sort alorithm on the encapsulated data
 */

#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "status.h"

// Comparator for sorting
struct defaultCompare {
    bool operator() (const double a, const double b) const
    {
        return b > a;
    }
};

class Heapsort: public Status {
public:
    Heapsort(int edge, int del) : Status(edge, del), len(edge*edge) {};

    void siftDown(const int start, const int end,
        std::function<bool(const double, const double)> comp = defaultCompare());
    void heapify();
    void report();
    void sort();

    int left(const int i) const;
    int right(const int i) const;
    int parent(const int i) const;

private:
    int len;
};

#endif /* HEAPSORT_H */
