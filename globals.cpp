#include "blocks.h"

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
int miniDistance1 = 125;      //55  //Distanz um direkt wieder perfekt auf Linie zu stehen
int miniDistance2 = 130;
int miniDistanceShort = 40; //rückwärts an Linie herangefahren
int moveBackDistance = 193; //Distanz vor einer Drehung zum Router
int leverDistance = 150;
int leverUpTime = 400;
int longMotorUpSpeed = 75;
int longMotorDownSpeed = -80;
int longMotorDistance = 310;
int spin180 = 500;
int spin90 =240;
int turn90 = 470;//435 //470
int goTurn90 = 510; //490

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

int routerDelivered = 0; //Um zu wissen, ob gerade gelb gesucht wird

int entscheidung; //ob oben oder unten rum zum wegbringen // 1 == obenrum, 0 == untenrum
int currentPosition;
int endPosition;
bool manualSetDriveDirection = false;
int currentColor;
bool liftParallel = false;
int liftParallelSpeed = 70;
Stopwatch run;

bool surprise1 = false;
//StallDetection mediumMotor