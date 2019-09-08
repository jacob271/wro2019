#pragma once
class StallDetection
{
    int arrayLength = 21;
    int stall [150];
    int index;
    bool array_full;

  public:
    void init(int length);
    bool detectStall();
    void measure(int value);
    void resetStall();
    StallDetection();
};
