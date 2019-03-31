#pragma once

#include "ev3api.h"
#include "app.h"

//#include <thread>
//#include <assert.h>
#include <string.h>
#include <vector>
//#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <fstream> //logfile
//#include <stdio.h> //date
//#include <stdlib.h> //date
#include "timer.h" //custom timer
#include "stalldetection.h"
#include <iostream>


//-----------------------------------------------------------------------------

// SUPPORT BLOCKS
int getRGB(sensor_port_t port, int color);
int getHTRGB(sensor_port_t sensor, int mode);
int speedLevel(int level);
void waitForButton();
void display(int inhalt);
int colorDetection(sensor_port_t sensor);
int colorDetection_rgb(sensor_port_t sensor, std::string mode);
int findColor(int colors[], std::string mode);

//void logColor();
void brake(bool stop, int endSpeed);
int frequencyDistribution(int colorCounter[]);
bool lineDetection(std::string mode);
void motorCorrection(double pGain, double cSpeed, int rightreset, int leftreset);
double accDec(int togo, double brakeFactor, double accFactor, Stopwatch move, double startSpeed, int maxSpeed, int endSpeed, bool dec);

void initializeSpeeds(int &speed1, int &speed2, int &speed3);
void resetMotors(std::string mode, int leftValue, int rightValue, int maxSpeed);
void resetMotors();
int measureMotorRight();    
int measureMotorLeft();

// ACTION BLOCKS
int moveStraight(int startSpeed, int maxSpeed, std::string mode, double value, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int moveStraight(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop);
void mediumMotor(motor_port_t motor, int speed, std::string mode, int wert, bool stop);
void turn1(motor_port_t turnMotor, int startSpeed, bool brakeOtherMotor, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);
void turn2(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);

// Global Vars
extern motor_port_t motor_left;
extern motor_port_t longMotor;
extern motor_port_t tool2;
extern motor_port_t motor_right;
extern sensor_port_t HTl;
extern sensor_port_t LSr;
extern sensor_port_t LSl;
extern sensor_port_t HTr;

//Speeds
extern double cSpeed;

// Variables for Moves
extern double pGain;
extern double pGainTurn2;
extern int resetRightDegree;
extern int resetLeftDegree;
extern const double bfMove;
extern const double bfTurn1;
extern const double bfTurn2;
extern const double afMove;
extern const double afLine1;

extern double batteryFactor;

extern int LSrMiddle;

// Constants for vertical line alignment
extern const double pi;
extern const double wheelDiameter;
extern const double wheelCircumferance;

extern int positions[];
extern int routerO[];
extern int routerW[];
extern int router[];

extern int alignDuration;
extern int miniDistance;
 
extern int wallDistanceE;