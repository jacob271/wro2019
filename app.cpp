/**
 * This sample program balances a two-wheeled Segway type robot such as Gyroboy in EV3 core set.
 *
 * References:
 * http://www.hitechnic.com/blog/gyro-sensor/htway/
 * http://www.cs.bgu.ac.il/~ami/teaching/Lejos-2013/classes/src/lejos/robotics/navigation/Segoway.java
 */

#pragma once
//#include <thread>
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

//Ports
motor_port_t motor_left = EV3_PORT_A;
motor_port_t tool1 = EV3_PORT_B;
motor_port_t tool2 = EV3_PORT_C;
motor_port_t motor_right = EV3_PORT_D;
sensor_port_t LSinnen = EV3_PORT_4;
sensor_port_t LSr = EV3_PORT_2;
sensor_port_t LSl = EV3_PORT_1;
sensor_port_t LSaussen = EV3_PORT_3;

//Speeds
double cSpeed = 0;
const int minSpeed = 25;

// Variables for Moves
double pGain = 0.02; //0.017
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.2;                  // 0.1   6
const double bfTurn1 = 0.5;                 // 9
const double bfTurn2 = 0.4;                // 0.3
const double afMove = 4.0;
const int averageBlueLeft = 1;
const int averageBlueRight = 1;

// Constants for vertical line alignment
const double lsDistance = 11.3; //11.7;
const double pi = 3.14159265358979324;
const double wheelDiameter = 6.24; //6.24;
const double wheelConverter = 6.24 / wheelDiameter;
const double wheelLSdist = 3.6 * 2 * (360 / (6.24 * pi));

double batteryFactor = 1;

void resetMotors()
{
    resetRightDegree = ev3_motor_get_counts(motor_right);
    resetLeftDegree = ev3_motor_get_counts(motor_left);
}

void test()
{
    //testgit
}

void main_task(intptr_t unused)
{
    Stopwatch run;
    std::ofstream out("AA_LOG_.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_motor_config(motor_left, UNREGULATED_MOTOR);
    ev3_motor_config(motor_right, UNREGULATED_MOTOR);
    ev3_motor_config(tool1, UNREGULATED_MOTOR);
    ev3_motor_config(tool2, UNREGULATED_MOTOR);
    ev3_sensor_config(LSinnen, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSaussen, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSl, COLOR_SENSOR);
    ev3_sensor_config(LSr, COLOR_SENSOR);
    ev3_speaker_play_tone(NOTE_C4, 500);
    ev3_speaker_play_tone(NOTE_E4, 500);
    tslp_tsk(500);
    test();
    return;
    std::cout << "Battery at: " << ev3_battery_voltage_mV() << "Volt" << std::endl;
    char buffer[10];
    itoa(ev3_battery_voltage_mV(), buffer, 10);
    ev3_lcd_draw_string(buffer, 20, 50);
    waitForButton();
    




    int neededTime = run.getTime();
    std::cout << "Needed time: " << neededTime << std::endl;
    char msgbuf[10];
    sprintf(msgbuf, "Time: %d", neededTime);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    ev3_motor_stop(motor_left, true);
    ev3_motor_stop(motor_right, true);
    tslp_tsk(1000);
    ev3_motor_stop(motor_left, false);
    ev3_motor_stop(motor_right, false);
    std::cout.rdbuf(coutbuf);}
}