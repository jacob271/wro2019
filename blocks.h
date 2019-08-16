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
void motorControl(motor_port_t motor, int speed, int maxSpeed);
void resetSpeedControl ();
int getRGB(sensor_port_t port, int color);
int getHTRGB(sensor_port_t sensor, int mode);
int speedLevel(int level);
void waitForButton();
void display(int inhalt);
int colorDetection(sensor_port_t sensor);
int colorDetection_rgb(sensor_port_t sensor, std::string mode);
int findColor(int colors[], std::string mode);
void align(int duration);

//void logColor();
void brake(bool stop, int endSpeed);
//int frequencyDistribution(int colorCounter[]);
int frequencyDistribution(int colorCounter[], std::string mode);

bool lineDetection(std::string mode);
void motorCorrection(double pGain, int cSpeed, int rightreset, int leftreset, double leftRatio, double rightRatio);
double accDec(int togo, double brakeFactor, double accFactor, double zeit, int startSpeed, int maxSpeed, int endSpeed, bool dec);

void initializeSpeeds(int &speed1, int &speed2, int &speed3);
void resetMotors(std::string mode, int leftValue, int rightValue, int maxSpeed);
void resetMotors();
int measureMotorRight();    
int measureMotorLeft();

// ACTION BLOCKS
int moveStraight(int startSpeed, int maxSpeed,  double leftRatio, double rightRatio, std::string mode, double value, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int moveStraight(int startSpeed, int maxSpeed,  double leftRatio, double rightRatio, std::string mode, double wert, int endSpeed, bool stop);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop);
void mediumMotor(motor_port_t motor, int speed, std::string mode, int wert, bool stop);
void turn1(motor_port_t turnMotor, int startSpeed, bool brakeOtherMotor, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);
void turn2(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);
void motorStall(motor_port_t motor, int speed);

// Global Vars
extern motor_port_t motor_left;
extern motor_port_t longMotor;
extern motor_port_t tool2;
extern motor_port_t motor_right;
extern sensor_port_t HTl;
extern sensor_port_t LSr;
extern sensor_port_t LSl;
extern sensor_port_t HTr;

extern motor_port_t temporalMotor;
extern motor_port_t secondTemporalMotor;
extern sensor_port_t temporalSensor;

//Speeds
extern int cSpeed;
extern int speedLevel1;
extern int speedControlD[];
extern int speedControlIndex;
extern double speedControlLastError;

// Variables for Moves
extern double pGain;
extern double pGainTurn2;
extern int resetRightDegree;
extern int resetLeftDegree;
extern const double bfMove;
extern const double bfTurn1;
extern const double bfTurn2;
extern const double bfLine;

extern const double afMove;
extern const double afTurn2;
extern const double afLine;

extern double batteryLevel;
extern double batteryFactor;

extern int LSrMiddle;

// Constants for vertical line alignment
extern const double pi;
extern const double wheelDiameter;
extern const double wheelCircumferance;

extern int positions[];
extern int routerO[];
extern int routerW[];
extern int blue;
extern int red;
extern int green;
extern int yellow;
extern int fall1;
extern int fall2;


extern int miniDistance;