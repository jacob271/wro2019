#pragma once
#include "blocks.h"


//#include "ev3dev.h"
class Stopwatch
{
    /*std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point current;*/
    SYSTIM start, now;

  public:
    double getTime();
    void reset();
    Stopwatch();
};