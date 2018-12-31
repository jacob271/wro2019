
#include "blocks.h"
#include "timer.h"
double Stopwatch::getTime()
{
    get_tim(&now);
    int timestamp = now-start;
    return timestamp;
    /*current = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current - start);
    return elapsed.count();*/
}
void Stopwatch::reset()
{
    get_tim(&start);
    //start = std::chrono::high_resolution_clock::now();
}
Stopwatch::Stopwatch()
{
    reset();
}