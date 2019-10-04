# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -std=c++11 -Wall
CVFLAG = `pkg-config --libs --cflags opencv`

# ****************************************************
# Targets needed to bring the executable up to date

cvSorts: main.cpp status.h status.cpp heapsort.h heapsort.cpp
	$(CXX) $(CXXFLAGS) main.cpp status.cpp heapsort.cpp $(CVFLAG) -o cvSorts
