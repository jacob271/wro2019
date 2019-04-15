#pragma once
class StallDetection
{
    int stall [21];
    int index;
    bool array_full;

  public:
    bool detectStall();
    void measure(int value);
    void resetStall();
    StallDetection();
};
