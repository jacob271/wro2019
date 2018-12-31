
#include "blocks.h"
#include "stalldetection.h"

void StallDetection::measure(int value)
{
    index++;
    if(index == 20){
        index = 0;
        array_full = true;
    }
    stall[index] = value;

}
bool StallDetection::detectStall()
{
    // Returns true if stall is detected
    int current;
    int next;
    if (array_full){
        current = stall[index];
        if (index == 19)
            next = stall[0];
        else
            next = stall[index+1];
        if (current == next){
            //std::cout << "stall detected " << current << " " << next << std::endl;
            //for(int i = 0; i < 20; i++);
                //std::cout << stall[i] << " ";
            //std::cout << std::endl;
            ev3_speaker_play_tone(NOTE_C4, 500);
            return true;
        }

    }
    return false;
}
void StallDetection::resetStall(){
    index = 0;
}
StallDetection::StallDetection()
{
    stall[20] = {0};
    array_full = false;
    index = 0;
    for(int i = 0; i < 20; i++)
        stall[i] = 0;
}
