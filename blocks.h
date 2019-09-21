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
using std::cout;
using std::endl;

//-----------------------------------------------------------------------------

//Methods
void start();
void nationalAnthem();
void updateLogDatei();
void positionenScannen();
void kabelSammeln(bool south);
void routerScannen(sensor_port_t searchSensor, std::string mode);
void routerEinsammeln(bool directionEast, int mode, bool crossline);
void kabelAbladen(bool stop, bool red);
void routerAbladen(sensor_port_t followSensor, bool lang);
void fallunterscheidung();
bool direction(int position, int direction, int nPositions);
int getDistance(int startPosition, int startDirection, int endPosition, int nPositions);
void router(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop);
void city(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop);

// SUPPORT BLOCKS
void motorControl(motor_port_t motor, int speed, int maxSpeed);
void resetSpeedControl();
int getRGB(sensor_port_t port, int color);
int getHTRGB(sensor_port_t sensor, int mode);
int speedLevel(int level);
void waitForButton();
void display(int inhalt);
bool colorDetection_rgb_ev3(sensor_port_t sensor);
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
int move(int startSpeed, int maxSpeed, double leftRatio, double rightRatio, std::string mode, double value, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int move(int startSpeed, int maxSpeed, double leftRatio, double rightRatio, std::string mode, double wert, int endSpeed, bool stop);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop);

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode);
int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop);
void mediumMotor(motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop);
void turn1(motor_port_t turnMotor, int startSpeed, bool brakeOtherMotor, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);
void turn2(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop);

// Global Vars
extern motor_port_t motor_left;
extern motor_port_t longMotor;
extern motor_port_t doubleLever;
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

extern double pGL1;
extern double dGL1;

extern double pGL2;
extern double dGL2;

// Constants for vertical line alignment
extern const double pi;
extern const double wheelDiameter;
extern const double wheelCircumferance;

//Distances for main
extern int miniDistance1;      //55  //Distanz um direkt wieder perfekt auf Linie zu stehen
extern int miniDistance2;
extern int miniDistanceShort; //rückwärts an Linie herangefahren
extern int moveBackDistance;  //Distanz vor einer Drehung zum Router
extern int leverDistance;
extern int leverUpTime;
extern int longMotorUpSpeed;
extern int longMotorDownSpeed;
extern int longMotorDistance;
extern int spin180;
extern int spin90;
extern int turn90;
extern int goTurn90;

extern int entscheidung; //ob oben oder unten rum zum wegbringen // 1 == obenrum, 0 == untenrum
extern int currentPosition;
extern int endPosition;
extern bool manualSetDriveDirection;
extern int currentColor;
extern Stopwatch run;