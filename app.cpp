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

//Ports
//Motoren von links (A) nach rechts (D)
motor_port_t motor_left = EV3_PORT_A;
motor_port_t longMotor = EV3_PORT_C;   //Motor für Router
motor_port_t doubleLever = EV3_PORT_B; //Motor für Kabel
motor_port_t motor_right = EV3_PORT_D;
//Sensoren von links (1) nach rechts (4)
sensor_port_t HTl = EV3_PORT_1;
sensor_port_t LSl = EV3_PORT_2;
sensor_port_t LSr = EV3_PORT_3;
sensor_port_t HTr = EV3_PORT_4;

//Ports für main
motor_port_t temporalMotor;
motor_port_t secondTemporalMotor;
sensor_port_t temporalSensor;

//Speeds
int cSpeed = 0;
int speedLevel1 = 20;

int speedControlD[4] = {0};
int speedControlIndex = 0;
double speedControlLastError = 0.0;

// Variables for Moves
double pGain = 0.02; //0.017
double pGainTurn2 = 1.5;
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.4; // Je höher, desto früher wird gebremst
const double bfTurn1 = 0.6;
const double bfTurn2 = 1.3; //1.1
const double bfLine = 0.8;  //1.0
const double afMove = 0.2;  //Beschleunigung in Einheiten pro Millisekunde
const double afTurn2 = 0.3;
const double afLine = 0.4; //0.2

double pGL1 = 0.43;
double dGL1 = 2.1;

double pGL2 = 0.35;
double dGL2 = 0.9;

// Constants for vertical line alignment
const double pi = 3.14159265358979323846264338;
const double wheelDiameter = 6.24; //6.24;
const double wheelCircumferance = 17.6;

//Distances for main
int miniDistance = 50;      //55  //Distanz um direkt wieder perfekt auf Linie zu stehen
int miniDistanceShort = 40; //rückwärts an Linie herangefahren
int moveBackDistance = 193; //Distanz vor einer Drehung zum Router
int leverDistance = 230;
int leverUpTime = 400;
int longMotorUpSpeed = 75;
int longMotorDownSpeed = -80;
int longMotorDistance = 310;
int spin180 = 480;

//Values for Line1
int LSrMiddle = (92 + 9) / 2;
double batteryLevel = 1;
double batteryFactor = 0.016; //speed - (batteryLevel - 7700) * batteryFactor

//Variablen zur Fallunterscheidung
int positions[4] = {0};
int routerO[3] = {0};
int routerW[3] = {0};
int blue;
int red;
int green;
int yellow;
int fall1;
int fall2;

int entscheidung; //ob oben oder unten rum zum wegbringen // 1 == obenrum, 0 == untenrum
int currentPosition;
int endPosition;
int currentColor;
Stopwatch run;

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