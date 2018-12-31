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
//void setMotorDirection(int v);
//int measureGyro();
//void resetGyrosensor(int resetvalue);
void waitForButton();
int colorDetection(sensor_port_t sensor);
int colorDetection_rgb(sensor_port_t sensor);
int lightSensor(sensor_port_t sensor, std::string mode);

//int colorDetection_adv();
//void showColor(int color);
void logColor();
void brakeAtEnd(int endSpeed);
int frequencyDistribution(int colorCounter[]);
bool lineDetection(std::string mode);
//void gyroCorrection(double _pGain, double _iGain, double &iCorrection, double cSpeed);
void motorCorrection(double pGain, double cSpeed, int rightreset, int leftreset);
double accDec(int togo, double brakeFactor, int accFactor, Stopwatch move, double startSpeed, int maxSpeed, int endSpeed, bool dec);
void initializeSpeeds(int &speed1, int &speed2, int &speed3);
void resetMotors(std::string mode, int leftValue, int rightValue, int maxSpeed);
int measureMotorRight();
int measureMotorLeft();

// ACTION BLOCKS
void turn1_rot(motor_port_t turnMotor, int startSpeed, int maxSpeed, double angle, int endSpeed);
int move(int startSpeed, int maxSpeed, std::string mode, double value, int endSpeed, bool stop, bool colorSearch);
/*
void turn2_rot(int startSpeed, int maxSpeed, double angle, int endSpeed);
void paulturn_rot(int startSpeed, int maxSpeed, double angle, double leftratio, double rightratio, int endSpeed);
void moveRotDegNoSync(int startSpeed, int maxSpeed, int distance, int endSpeed);
int moveRotDegRatio(int startSpeed, int maxSpeed, int distance, double rratio, double lratio, int endSpeed, bool colorSearch);
int moveRotDegRatioNoSync(int startSpeed, int maxSpeed, int distance, double rratio, double lratio, int endSpeed, bool colorSearch);
int moveRotDegMediumMotor(int startSpeed, int maxSpeed, int distance, int endSpeed, bool colorSearch, motor_port_t motor, int speed, int degree, bool stop);
int moveRotDegMediumMotorTime(int startSpeed, int maxSpeed, int distance, int endSpeed, bool colorSearch, motor_port_t motor, int speed, int time, bool stop);
int moveRotLineMediumMotor(int startSpeed, int maxSpeed, std::string mode, int endSpeed, bool colorSearch, motor_port_t motor, int speed, int degree, bool stop);
int alignmentangle(int startSpeed, int maxSpeed, std::string color, int endSpeed);
void old_align(int cSpeed, std::string mode);
void mediumMotorDeg(motor_port_t motor, int speed, int degree, bool stop);
void mediumMotorTime(motor_port_t motor, int speed, int time, bool stop);
void twoMediumMotorsTime(int speed_kuhl, int time_kuhl, int speed_greif, int time_greif);
int brake();

*/

// Global Vars
extern motor_port_t motor_left;
extern motor_port_t tool1;
extern motor_port_t tool2;
extern motor_port_t motor_right;
extern sensor_port_t LSinnen;
extern sensor_port_t LSr;
extern sensor_port_t LSl;
extern sensor_port_t LSaussen;

//Speeds
extern double cSpeed;
extern const int minSpeed;

// Variables for Moves
extern double pGain;
extern int resetRightDegree;
extern int resetLeftDegree;
extern const double bfMove;
extern const double bfTurn1;
extern const double bfTurn2;
extern const double afMove;
extern const int averageBlueLeft;
extern const int averageBlueRight;

// Constants for vertical line alignment
extern const double lsDistance;
extern const double pi;
extern const double wheelDiameter;
extern const double wheelConverter;
extern const double wheelLSdist;

extern double batteryFactor;