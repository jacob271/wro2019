//#pragma once //enabled by default
#include <chrono>
#include <cstdio>

#include <string.h>
#include <stdlib.h>
#include "blocks.h"
#include "timer.h"

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

void test()
{
    line2(1,3,pGL2,dGL2,"degree",400,3,false);
    move(3,3,1,0.42,"degree",800,3,false);
    move(3,3,1,1,"degree",500,1,true);   
    start(); 

    //city(5, 3, -4, 2, true);
}

void main_task(intptr_t unused)
{
    //Log-Datei initialisieren
    std::ofstream out("logFile.txt");
    std::streambuf *coutbuf = cout.rdbuf(); //save old buf
    cout.rdbuf(out.rdbuf());                //redirect cout to out.txt!
    start();
    //Stopwatch run;
    tslp_tsk(200);
    //test();
    //return;
    move(1,3,0.4,1,"degree",220,3,false);
    move(cSpeed,3,1,0.4,"degree",205,3,false);
    move(3,3,1,1,"degree",80,3,false);
    positionenScannen();
    city(9,4,5,3,false);
    routerScannen(HTr,"routerO");
    turn1(motor_left,3,true,3,"degree",490,3,false);
    line2(3,3,pGL2,dGL2,"degree",100,3,false);
    move(3,4,1,0.57,"degree",320,4,false);
    move(4,4,0.57,1,"degree",310,4,false);
    line2(4,4,pGL2,dGL2,"degree",250,2,false);
    move(2,2,1,1,"degree",60,1,true);
    mediumMotor(doubleLever, 70,"degree",leverDistance,true);
    move(-1,-3,1,1,"degree",270,1,true);
    turn2(1,5,"degree",240,1,true);
    line2(1,3,pGL2,dGL2,"degree",100,3,false);
    routerScannen(HTl,"routerW");
    
    int neededTime = run.getTime();

    //int neededTime = run.getTime();
    cout << "Needed time: " << neededTime << endl;
    cout << batteryLevel << " " << batteryFactor << " " << fall1 << " " << fall2 << " " << neededTime << " " << blue << " " << red << " " << green << " " << yellow << " " << routerO[0] << " " << routerO[1] << " " << routerO[2] << " " << routerW[0] << " " << routerW[1] << " " << routerW[2] << endl;
    char msgbuf[10];
    sprintf(msgbuf, "Time: %d", neededTime);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    cout.rdbuf(coutbuf);

    ev3_motor_stop(motor_left, true);
    ev3_motor_stop(motor_right, true);
    ev3_speaker_play_tone(NOTE_E4, 300);
    tslp_tsk(500);
    ev3_motor_stop(motor_left, false);
    ev3_motor_stop(motor_right, false);
    ev3_speaker_play_tone(NOTE_E4, 100);
    //nationalAnthem();
}
//Das Programm braucht noch eine Zeile hintendran, sonst kompiliert es nicht