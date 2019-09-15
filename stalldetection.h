#pragma once
class StallDetection
{
    int arrayLength = 100;
    int stall [100];
    int index;
    bool array_full;

  public:
    void init(int length);
    bool detectStall();
    void measure(int value);
    void resetStall();
    StallDetection();
};
