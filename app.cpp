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
const double bfTurn1 = 0.8;
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
int leverDistance = 110;
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
int currentColor;
Stopwatch run;

void start() //Konfiguration für den Start
{
    //Konfiguration der Hardware
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_motor_config(motor_left, UNREGULATED_MOTOR); //MEDIUM_MOTOR
    ev3_motor_config(motor_right, UNREGULATED_MOTOR);
    ev3_motor_config(longMotor, UNREGULATED_MOTOR);
    ev3_motor_config(doubleLever, UNREGULATED_MOTOR);
    ev3_sensor_config(HTl, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(HTr, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSl, COLOR_SENSOR);
    ev3_sensor_config(LSr, COLOR_SENSOR);
    ev3_speaker_play_tone(NOTE_E4, 300);
    tslp_tsk(4000);
    ev3_speaker_play_tone(NOTE_G4, 300);

    batteryLevel = ev3_battery_voltage_mV();

    batteryFactor = (7950 / batteryLevel); //8045
    if (batteryLevel >= 7900)
    {
        batteryFactor = batteryFactor * 1.02;
    }

    std::cout << batteryFactor << std::endl;

    pGL1 = pGL1 * batteryFactor;
    dGL1 = dGL1 * batteryFactor;

    pGL2 = pGL2 * batteryFactor;
    dGL2 = dGL2 * batteryFactor;

    std::cout << "Battery at: " << batteryLevel << "Volt" << std::endl;
    std::cout << "speedsLevel:" << std::endl;
    std::cout << "1 " << speedLevel(1) << std::endl;
    std::cout << "2 " << speedLevel(2) << std::endl;
    std::cout << "3 " << speedLevel(3) << std::endl;
    std::cout << "4 " << speedLevel(4) << std::endl;
    std::cout << "5 " << speedLevel(5) << std::endl;
    std::cout << "6 " << speedLevel(6) << std::endl;
    display(batteryLevel);

    //Berechnet minSpeed mit batteryVoltage
    //speedLevel1 = (int)(25 - (batteryVoltage - 7500) * (16 / 1000));
    //std::cout << "speedLevel1: " << speedLevel1 << std::endl;

    waitForButton();
    run.reset();
    resetMotors();
}

void nationalAnthem()
{
    for (int i = 0; i < 2; i++)
    {
        ev3_speaker_play_tone(NOTE_F4, 750);
        tslp_tsk(750);
        ev3_speaker_play_tone(NOTE_G4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_A4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_G4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_AS4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_A4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_G4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_E4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_F4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_D5, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_C5, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_AS4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_A4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_G4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_A4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_F4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_C5, 1000);
        tslp_tsk(1000);
    }
    ev3_speaker_play_tone(NOTE_G4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_A4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_G4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_E4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_C4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_AS4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_A4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_G4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_E4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_C4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_C5, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_AS4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_A4, 750);
    tslp_tsk(750);
    ev3_speaker_play_tone(NOTE_A4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_B4, 500);
    tslp_tsk(500);
    ev3_speaker_play_tone(NOTE_B4, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_C5, 250);
    tslp_tsk(250);
    ev3_speaker_play_tone(NOTE_C5, 1000);
    tslp_tsk(1000);
    for (int i = 0; i < 2; i++)
    {
        ev3_speaker_play_tone(NOTE_F5, 750);
        tslp_tsk(750);
        ev3_speaker_play_tone(NOTE_E5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_E5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_D5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_C5, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_D5, 750);
        tslp_tsk(750);
        ev3_speaker_play_tone(NOTE_C5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_C5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_AS4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_A4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_G4, 750);
        tslp_tsk(750);
        ev3_speaker_play_tone(NOTE_A4, 125);
        tslp_tsk(125);
        ev3_speaker_play_tone(NOTE_AS4, 125);
        tslp_tsk(125);
        ev3_speaker_play_tone(NOTE_C5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_D5, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_AS4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_G4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_F4, 500);
        tslp_tsk(500);
        ev3_speaker_play_tone(NOTE_A4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_G4, 250);
        tslp_tsk(250);
        ev3_speaker_play_tone(NOTE_F4, 1000);
        tslp_tsk(1000);
    }
}

void updateLogDatei()
{
    std::cout << "-" << std::endl;
    std::cout << "time " << run.getTime() << std::endl;
    std::cout << "fall1 " << fall1 << " fall2 " << fall2 << std::endl;
    std::cout << "blue " << blue << " red " << red << " green " << green << " yellow " << yellow << std::endl;
    std::cout << "routerO " << routerO[0] << " " << routerO[1] << " " << routerO[2] << std::endl;
    std::cout << "routerW " << routerW[0] << " " << routerW[1] << " " << routerW[2] << std::endl;
    std::cout << "-" << std::endl;
}

void crossline(int startSpeed, int anzahl)
{
    line2(startSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
    for (int i = 1; i < anzahl; i++)
    {
        line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
        line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
    }
    //moveStraight(cSpeed, 2, "degree", miniDistance, 1, true);
}

void positionenScannen()
{
    line1(1, 90, pGL1, dGL1, LSr, false, "degree", 315, 90, false);
    for (int i = 0; i < 3; i++)
    {
        positions[i] = line1(cSpeed, 90, pGL1, dGL1 * 0.6, LSr, false, "degree", 147, 60, false, true, HTr, "color");
    }
    positions[3] = findColor(positions, "positions");
    line1(cSpeed, 90, pGL1, dGL1, LSr, false, "degree", 360, 1, true);
}

void kabelSammeln(bool south)
{
    int richtung = 1;
    if (south)
    {
        richtung = -1;
    }
    line2(1, 3, pGL2, dGL2, "degree", 100, 3, false);
    crossline(cSpeed, 1);
    turn2(1, 5, "degree", -90 * richtung, 1, true);
    //moveStraight(1, 3, "degree", 115, 1, true);
    turn2(1, 5, "degree", 90 * richtung, 1, true);
    mediumMotor(doubleLever, -80, "degree", leverDistance, false); //down
    line2(1, 40, pGL2, dGL2, "degree", 280, 1, true);              //275
    mediumMotor(doubleLever, 80, "time", 630, true);               //up
    //moveStraight(-1, -3, "crossline", 0, -1, true);
    //moveStraight(1, 2, "degree", miniDistanceShort, 1, true);
    if (fall2 != 2 && fall2 != 3 && fall2 != 10)
    {
        turn2(1, 5, "degree", -90 * richtung, 1, true);
        line2(1, 3, pGL2, dGL2, "degree", 70, 1, true);
    }
}

void routerScannen(sensor_port_t searchSensor, std::string mode)
{
    //direkt nach der Drehung
    int router[3] = {0};
    int i = 0;
    if (mode == "routerO")
    {
        line2(1, 4, pGL2, dGL2, "degree", 165, 4, false);
    }
    else
    {
        line2(1, 4, pGL2, dGL2, "degree", 40, 4, false);
    }
    router[i] = line2(cSpeed, 4, pGL2, dGL2 * 0.6, "degree", 349, 4, false, true, searchSensor, "bw");
    i++;
    if (mode == "routerO")
    {
        router[i] = line2(cSpeed, 4, pGL2, dGL2 * 0.6, "degree", 349, 3, false, true, searchSensor, "bw");
        line2(cSpeed, 3, pGL2, dGL2, "degree", 360, 3, false);
        crossline(cSpeed, 1);
    }
    else
    {
        router[i] = line2(cSpeed, 4, pGL2, dGL2 * 0.6, "degree", 349, 1, true, true, searchSensor, "bw");
    }
    i++;
    router[i] = findColor(router, "router");

    if (mode == "routerO")
    {
        for (int x = 0; x < 3; x++)
        {
            routerO[2 - x] = router[x];
        }
    }
    else
    {
        for (int x = 0; x < 3; x++)
        {
            routerW[x] = router[x];
        }
    }
}

void routerEinsammeln(bool directionEast, bool fromSouth)
{
    if (directionEast == true)
    {
        motor_port_t turnMotor = motor_right;
        if (fromSouth == true)
        {
            turnMotor = motor_left;
        }
        line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false);
        line2(cSpeed, 4, pGL2, dGL2, "degree", 250, 1, true);
        turn1(turnMotor, -1, false, -4, "degree", 435, -1, true);
        mediumMotor(longMotor, -100, "degree", 210, true);
        line2(1, 4, pGL2, dGL2, "degree", 200, 1, true);
        mediumMotor(longMotor, 60, "degree", 250, true);
    }
    else
    {
        if (fromSouth == true)
        {
            turn2(1, 5, "degree", -240, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 5, "degree", 240, 1, true);
        }
        else
        {
            turn2(1, 5, "degree", 240, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 5, "degree", -240, 1, true);
        }
    }
}

void kabelAbladen()
{
    line2(1, 3, 0.7, dGL2, "degree", 300, 6, false);
    line2(cSpeed, 6, 0.7, dGL2, "degree", 140, 40, false);
    //moveStraight(cSpeed, 35, "degree", 160, 1, true);
    //tslp_tsk(300);
    mediumMotor(doubleLever, -50, "time", 350, false);
    //moveStraight(-1, -6, "degree", 250, 1, true);
}

void routerAbladen()
{
    line2(cSpeed, 30, pGL2, dGL2, "crossline", 0, 1, true);
    mediumMotor(longMotor, -30, "degree", 230, true);
    waitForButton();
    moveStraight(-1,-3,1,1,"degree",350,-1,true);
    turn2(1,5,"degree",480,1,true);
}

void test()
{
    turn1(motor_left, -1, false, -4, "degree", 435, -1, true);
    turn1(motor_right, -1, false, -4, "degree", 435, -1, true);

    mediumMotor(doubleLever, 80, "time", 630, true); //up
    waitForButton();
    tslp_tsk(340);

    line2(50, 90, pGL2, dGL2, "crossline", 0, 90, false);
    line2(cSpeed, 90, pGL2, dGL2, "degree", 310, 1, true);
    turn2(1, 5, "degree", 90, 1, true);
    mediumMotor(longMotor, -100, "degree", 210, true);
    turn2(1, 5, "degree", -90, 1, true);
    line2(50, 90, pGL2, dGL2, "crossline", 0, 90, false);
    line2(cSpeed, 90, pGL2, dGL2, "degree", 312, 1, true);
    turn1(motor_right, 1, false, 90, "degree", 510, 1, true);
    mediumMotor(longMotor, 60, "degree", 250, true);
    line2(1, 60, pGL2, dGL2, "degree", 100, 30, true);
    line2(cSpeed, 30, pGL2, dGL2, "crossline", 0, 1, true);
    mediumMotor(longMotor, -30, "degree", 210, true);

    /*
    while (true)
    {
        line2(50, 60, pGL2, dGL2, "degree", 250, 1, true);
        mediumMotor(longMotor, 70, "degree", 250, true);
        waitForButton();
        line2(50, 60, pGL2, dGL2, "degree", 250, 50, false);
        line2(cSpeed, 30, pGL2, dGL2, "crossline", 0, 1, true);
        mediumMotor(longMotor, -30, "degree", 210, true);
        waitForButton();
    }*/

    /*
    while(true){
        line2(cSpeed, 60, pGL2, dGL2, "degree", 330, 60,false);
        moveStraight(60,60,"degree",295,1,true);
        mediumMotor(doubleLever, -30, "degree", 210, true);
    }
    */

    /*
    while(true){
        tslp_tsk(2000);
        line1(50,100,0.43,2.1,LSr,true,"degree",800,70,true);
        waitForButton();
    }   
    */
}

void fallunterscheidung()
{

    //Schreibt Werte aus Positionsarray in passende Variablen
    for (int i = 0; i < 4; i++)
    {
        switch (positions[i])
        {
        case 2:
            blue = i + 1;
            break;
        case 3:
            green = i + 1;
            break;
        case 4:
            yellow = i + 1;
            break;
        case 5:
            red = i + 1;
            break;
        default:
            break;
        }
    }

    int tempred = red;
    int tempblue = blue;
    int tempgreen = green;
    int tempyellow = yellow;

    red = tempyellow;
    blue = tempgreen;
    yellow = tempred;
    green = tempblue;

    //Weist den Kombinationen die passenden Fälle zu
    if ((blue == 4 && red == 1 && green == 2 && yellow == 3) || (blue == 1 && red == 2 && green == 3 && yellow == 4))
    {
        fall1 = 1;
        fall2 = 1;
    }
    else if ((blue == 4 && red == 1 && green == 3 && yellow == 2) || (blue == 4 && red == 2 && green == 3 && yellow == 1))
    {
        fall1 = 1;
        fall2 = 2;
    }
    else if ((blue == 1 && red == 2 && green == 4 && yellow == 3) || (blue == 4 && red == 2 && green == 1 && yellow == 3))
    {
        fall1 = 1;
        fall2 = 3;
    }
    else if ((blue == 1 && red == 3 && green == 2 && yellow == 4) || (blue == 1 && red == 4 && green == 2 && yellow == 3))
    {
        fall1 = 2;
        fall2 = 4;
    }
    else if ((blue == 2 && red == 1 && green == 3 && yellow == 4) || (blue == 3 && red == 1 && green == 2 && yellow == 4))
    {
        fall1 = 3;
        fall2 = 4;
    }
    else if ((blue == 1 && red == 4 && green == 3 && yellow == 2) || (blue == 4 && red == 3 && green == 2 && yellow == 1))
    {
        fall1 = 2;
        fall2 = 5;
    }
    else if ((blue == 2 && red == 1 && green == 4 && yellow == 3) || (blue == 3 && red == 2 && green == 1 && yellow == 4))
    {
        fall1 = 3;
        fall2 = 6;
    }
    else if ((blue == 1 && red == 3 && green == 4 && yellow == 2) || (blue == 4 && red == 3 && green == 1 && yellow == 2))
    {
        fall1 = 2;
        fall2 = 7;
    }
    else if ((blue == 3 && red == 1 && green == 4 && yellow == 2) || (blue == 3 && red == 2 && green == 4 && yellow == 1))
    {
        fall1 = 3;
        fall2 = 7;
    }
    else if ((blue == 2 && red == 4 && green == 3 && yellow == 1) || (blue == 3 && red == 4 && green == 2 && yellow == 1))
    {
        fall1 = 4;
        fall2 = 8;
    }
    else if ((blue == 2 && red == 3 && green == 1 && yellow == 4) || (blue == 2 && red == 4 && green == 1 && yellow == 3))
    {
        fall1 = 4;
        fall2 = 9;
    }
    else //if ((blue == 3 && red == 4 && green == 1 && yellow == 2) || (blue == 2 && red == 3 && green == 4 && yellow == 1))
    {
        fall1 = 4;
        fall2 = 10;
    }
}

//Gibt die Fahrtrichtung im Uhrzeigersinn anhand der Position auf dem Router-Kreis und der Himmelsrichtung (1-4) zurück
bool directionRouter(int position, int direction)
{
    if ((direction == 1) && (position == 2 || position == 3 || position == 4 || position == 5))
    {
        return true;
    }
    else if ((direction == 2) && (position == 6))
    {
        return true;
    }
    else if ((direction == 3) && (position == 7 || position == 8 || position == 9 || position == 10))
    {
        return true;
    }
    else if ((direction == 4) && (position == 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Gibt die Fahrtrichtung im Uhrzeigersinn anhand der Position auf dem Router-Kreis und der Himmelsrichtung (1-4) zurück
bool directionCity(int position, int direction)
{
    if ((direction == 1) && (position == 2 || position == 3 || position == 4 || position == 5))
    {
        return true;
    }
    else if ((direction == 2) && (position == 6 || position == 7 || position == 8 || position == 9))
    {
        return true;
    }
    else if ((direction == 3) && (position == 10 || position == 11 || position == 12 || position == 13))
    {
        return true;
    }
    else if ((direction == 4) && (position == 14 ||position == 15 || position == 16 || position == 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Der Roboter kann anhand der Start- und Endposition auf dem Router-Kreis den kürzesten Weg erkennen und fährt diesen anschließend
void router(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop)
{
    //1 = kurze Seite im Süden, dann im Uhrezeigersinn
    std::cout << "Enter Router-Kreis at: " << currentPosition << " " << currentDirection << std::endl;

    //Berechnet die Abstände im und gegen den Uhrzeigersinn auf dem Router-Kreis
    int distanceForw = 0;
    int distanceBack = 0;
    for (int i = currentPosition; i != endPosition; i++)
    {
        if (i == 11)
        {
            i = 1;
        }
        if (i != endPosition)
        {
            distanceForw++;
        }
        else
        {
            break;
        }
    }

    for (int i = currentPosition; i != endPosition; i--)
    {
        if (i == 0)
        {
            i = 10;
        }
        if (i != endPosition)
        {
            distanceBack++;
        }
        else
        {
            break;
        }
    }

    std::cout << "distanceForw: " << distanceForw << " distanceBack: " << distanceBack << std ::endl;

    //Legt anhand der kürzeren Strecke die Fahrtrichtung fest. Falls es keine kürzere gibt entscheidet die aktuelle Orientierung des Roboters
    bool driveDirection;
    if (distanceForw < distanceBack)
    {
        driveDirection = true;
    }
    else if (distanceForw > distanceBack)
    {
        driveDirection = false;
    }
    else
    {
        driveDirection = directionRouter(currentPosition, currentDirection);
    }
    std::cout << "driveDirection: " << driveDirection << std::endl;

    //Dreht den Roboter gegebenenfalls in Fahrtrichtung
    if (directionRouter(currentPosition, currentDirection) != driveDirection)
    {
        turn2(1, 5, "degree", 480, 1, true);
    }

    //übernimmt Fahrtgeschwindigkeit oder setzt Startspeed
    if (cSpeed < speedLevel(1))
        cSpeed = 1;

    //Strecken
    int toMiddle = 310;      //Von Crossline bis der Roboter zwischen den Linien steht
    int degT1continue = 495; //Strecke für 90 Grad turn1 ohne abbremsen
    motor_port_t turnMotor;

    int endSpeed;
    bool stopNow;

    int nextPosition;

    //Zählt je nach Fahrtrichtung die Positionen hoch oder runter und fährt von einer zur nächsten
    if (driveDirection == true)
    {
        turnMotor = motor_left;
        for (int i = currentPosition; i != endPosition; i++)
        {
             if (i == 11)
                i = 1;

            nextPosition = i + 1;
            if (i == 10)
                nextPosition = 1;

            if (i != endPosition)
            {
                endSpeed = 3;
                stopNow = false;
                if (nextPosition == endPosition && stop == true)
                {
                    endSpeed = 1;
                    stopNow = true;
                }
                std::cout<< "position: " << i << std::endl;
                switch (i)
                {
                case 1:
                case 6:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 145, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, endSpeed, stopNow);
                    break;
                case 2:
                case 3:
                case 4:
                case 7:
                case 8:
                case 9:
                    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", toMiddle, endSpeed, stopNow);
                    break;
                case 5:
                case 10:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 60, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 205, endSpeed, stopNow);
                    break;
                default:
                    break;
                }
            }
            else
                break;
        }
    }
    else
    {
        turnMotor = motor_right;
        for (int i = currentPosition; i != endPosition; i--)
        {
            if (i == 0)
                i = 10;

            nextPosition = i - 1;
            if (i == 1)
                nextPosition = 10;

            if (i != endPosition)
            {
                endSpeed = 3;
                stopNow = false;
                if (nextPosition == endPosition && stop == true)
                {
                    endSpeed = 1;
                    stopNow = true;
                }
                std::cout<< "position: " << i << std::endl;
                switch (i)
                {
                case 1:
                case 6:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 145, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, endSpeed, stopNow);
                    break;
                case 10:
                case 9:
                case 8:
                case 5:
                case 4:
                case 3:
                    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", toMiddle, endSpeed, stopNow);
                    break;
                case 2:
                case 7:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 60, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 205, endSpeed, stopNow);
                    break;
                default:
                    break;
                }
            }
            else
                break;
        }
    }

    //Dreht am Ende, falls die Orientierung noch nicht übereinstimmt
    if (directionRouter(endPosition, endDirection) != driveDirection)
    {
        turn2(1, 5, "degree", 480, 1, true);
    }

    std::cout << "Exit Router-Kreis at: " << endPosition << " " << endDirection << std::endl;
}

void city(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop)
{
    
    //1 = westlichste Seite im Süden, dann im Uhrzeigersinn
    std::cout << "Enter City-Kreis at: " << currentPosition << " " << currentDirection << std::endl;


    //Berechnen wo der Roboter hinfährt - Strecken aufteilen
    //Auf den Kreis fahren

    //Berechnet die Abstände im und gegen den Uhrzeigersinn auf dem City-Kreis 
    int distanceForw = 0;
    int distanceBack = 0;
    for (int i = currentPosition; i != endPosition; i++)
    {
        if (i == 17)
        {
            i = 1;
        }
        if (i != endPosition)
        {
            distanceForw++;
        }
        else
        {
            break;
        }
    }

    for (int i = currentPosition; i != endPosition; i--)
    {
        if (i == 0)
        {
            i = 16;
        }
        if (i != endPosition)
        {
            distanceBack++;
        }
        else
        {
            break;
        }
    }

    std::cout << "distanceForw: " << distanceForw << " distanceBack: " << distanceBack << std ::endl;

    //Legt anhand der kürzeren Strecke die Fahrtrichtung fest. Falls es keine kürzere gibt entscheidet die aktuelle Orientierung des Roboters
    bool driveDirection;
    if (distanceForw < distanceBack)
    {
        driveDirection = true;
    }
    else if (distanceForw > distanceBack)
    {
        driveDirection = false;
    }
    else
    {
        driveDirection = directionCity(currentPosition, currentDirection);
    }
    std::cout << "driveDirection: " << driveDirection << std::endl;

    //Dreht den Roboter gegebenenfalls in Fahrtrichtung
    if (directionCity(currentPosition, currentDirection) != driveDirection)
    {
        turn2(1, 5, "degree", 480, 1, true);
        std::cout<<"turn into driveDirection" << std::endl;
    }

    //übernimmt Fahrtgeschwindigkeit oder setzt Startspeed
    if (abs(cSpeed) < abs(speedLevel(1)))
        cSpeed = 1;

    std::cout<< "cSpeed " << cSpeed << std::endl;
    //Strecken
    int toMiddle = 310;      //Von Crossline bis der Roboter zwischen den Linien steht
    int degT1continue = 495; //Strecke für 90 Grad turn1 ohne abbremsen
    motor_port_t turnMotor;

    int endSpeed;
    bool stopNow;

    int nextPosition;

    //Zählt je nach Fahrtrichtung die Positionen hoch oder runter und fährt von einer zur nächsten
    if (driveDirection == true)
    {
        turnMotor = motor_left;
        for (int i = currentPosition; i != endPosition; i++)
        {
            if (i == 17)
                i = 1;

            nextPosition = i + 1;
            if (i == 16)
                nextPosition = 1;

            if (i != endPosition)
            {
                endSpeed = 3;
                stopNow = false;
                if (nextPosition == endPosition && stop == true)
                {
                    endSpeed = 1;
                    stopNow = true;
                }
                std::cout<< "position: " << i << std::endl;
                switch (i)
                {
                case 1:
                case 9:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 125, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 60, endSpeed, stopNow);
                    break;
                case 2:
                case 3:
                case 4:
                    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", toMiddle, endSpeed, stopNow);
                    break;
                case 10:
                case 11:
                case 12:
                    line1(cSpeed, 3, pGL1, dGL1, LSl,false, "crossline", 0, 3, false);
                    line1(3,3,pGL1,dGL1,LSl,false,"degree",toMiddle,endSpeed,stopNow);
                    break;
                case 13:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 10, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                    break;
                case 5:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                    break;
                case 6:
                case 7:
                case 14:
                case 15:
                    line1(cSpeed,3,pGL1,dGL1,LSl,false,"crossline",0,3,false);
                    line1(3,3,pGL1,dGL1,LSl,false,"degree",415,endSpeed,stopNow);
                    break;
                case 8:
                case 16:
                    line1(cSpeed,3,pGL1,dGL1,LSl,false,"crossline",0,3,false);
                    line1(3,3,pGL1,dGL1,LSl,false,"degree",440,endSpeed,stopNow);
                    break;
                default:
                    break;
                }
            }
            else
                break;
        }
    }
    else
    {
        turnMotor = motor_right;
        for (int i = currentPosition; i != endPosition; i--)
        {
            if (i == 0)
                i = 16;

            nextPosition = i - 1;
            if (i == 1)
                nextPosition = 16;           
            if (i != endPosition)
            {
                endSpeed = 3;
                stopNow = false;
                if (nextPosition == endPosition && stop == true)
                {
                    endSpeed = 1;
                    stopNow = true;
                }
                std::cout<< "position: " << i << std::endl;
                switch (i)
                {
                case 1:
                case 8:
                case 9:
                case 16:
                    line1(cSpeed,3,pGL1,dGL1,LSr,true,"crossline",0,3,false);
                    line1(3,3,pGL1,dGL1,LSr,true,"degree",415,endSpeed,stopNow);
                    break;
                case 2:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                    break;
                case 10:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 10, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                    break;
                case 3:
                case 4:
                case 5:
                    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", toMiddle, endSpeed, stopNow);
                    break;
                case 6:
                case 14:
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 160, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", degT1continue, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 60, endSpeed, stopNow);
                    break;
                case 7:
                case 15:
                    line1(cSpeed,3,pGL1,dGL1,LSr,true,"crossline",0,3,false);
                    line1(3,3,pGL1,dGL1,LSr,true,"degree",440,endSpeed,stopNow);
                    break;
                case 11:
                case 12:
                case 13:
                    line1(cSpeed, 3, pGL1, dGL1, LSr,true, "crossline", 0, 3, false);
                    line1(3,3,pGL1,dGL1,LSr,true,"degree",toMiddle,endSpeed,stopNow);
                    break;
                default:
                    break;
                }
            }
            else
                break;
        }
    }

    //Dreht am Ende, falls die Orientierung noch nicht übereinstimmt
    if (directionCity(endPosition, endDirection) != driveDirection)
    {
        turn2(1, 5, "degree", 480, 1, true);
        std::cout<< "turn to endPosition" << std::endl;
    }

    std::cout << "Exit Router-Kreis at: " << endPosition << " " << endDirection << std::endl;
    
}

void trickshot()
{
    while (true)
    {
        waitForButton();
        tslp_tsk(200);
        mediumMotor(longMotor, 100, "degree", 150, true);
        tslp_tsk(500);
        mediumMotor(longMotor, -50, "time", 500, false);
    }
}

void main_task(intptr_t unused)
{
    //Log-Datei initialisieren
    std::ofstream out("logFile.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf());                //redirect std::cout to out.txt!
    start();
    //Stopwatch run;
    tslp_tsk(200);
    city(1,4,9,2,false);
    city(9,2,1,4,false);
    //city(1,2,14,2,false);
    line2(3,3,pGL2,dGL2,"degree",200,1,true);
    moveStraight(1,3,1,0.163,"degree",1170,1,true);
    routerAbladen();
    //router(7,1,2,1,true);
    //routerEinsammeln(false,true);
    //trickshot();
    //router(10, 3, 6, 2, true);
    //turn1(motor_right,1,false,60,"degree",900,1,true);
    //krasseDrehung();
    //test();
    return;

    //Anfang - Scannt Positionen, scannt Router und sammelt das südliche Kabel
        //moveStraight(30, 50, "degree", 115, 30, true);
    turn1(motor_left, 1, false, 4, "degree", -45, 1, true);
    //waitForButton();
        //moveStraight(20, 3, "degree", 115, 3, false);
    //zusatzAndererStart();
    positionenScannen();
    turn1(motor_right, 1, false, 4, "degree", -90, 1, true);

    //surprise();

    line1(cSpeed, 100, pGL1, dGL1, LSr, true, "degree", 1900, 70, false);
    line1(cSpeed, 100, pGL1, dGL1, LSr, true, "degree", 380, 3, false);
    line1(cSpeed, 3, 0.3, 5, LSr, true, "crossline", 0, 3, false);
        //moveStraight(cSpeed, 3, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", -90, 1, true);
    routerScannen(HTr, "routerO");
    turn2(1, 5, "degree", 90, 1, true);
    kabelSammeln(true);
    routerScannen(HTl, "routerW");

    fallunterscheidung();
    //arrays router: 0 schwarz; 1 weiß; 2 getauscht; 3 leer

    //kabelAbladen();
    //wegbringen1();
    kabelSammeln(false);

    //auskommentieren für surprise2
    mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
        //moveStraight(1, 100, "degree", 255, 100, false);
    turn1(motor_left, 100, true, 4, "degree", 50, 100, false);

    int neededTime = run.getTime();
        //moveStraight(100, 100, "time", 300, 1, true);
        //moveStraight(-30, -50, "degree", 40, 1, true);

    //int neededTime = run.getTime();
    std::cout << "Needed time: " << neededTime << std::endl;
    std::cout << batteryLevel << " " << batteryFactor << " " << fall1 << " " << fall2 << " " << neededTime << " " << blue << " " << red << " " << green << " " << yellow << " " << routerO[0] << " " << routerO[1] << " " << routerO[2] << " " << routerW[0] << " " << routerW[1] << " " << routerW[2] << std::endl;
    char msgbuf[10];
    sprintf(msgbuf, "Time: %d", neededTime);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    std::cout.rdbuf(coutbuf);

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