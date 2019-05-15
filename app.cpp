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
motor_port_t longMotor = EV3_PORT_B;   //Motor für Router
motor_port_t doubleLever = EV3_PORT_C; //Motor für Kabel
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

// Variables for Moves
double pGain = 0.02; //0.017
double pGainTurn2 = 1.5;
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.4; // Je höher, desto früher wird gebremst
const double bfTurn1 = 0.8;
const double bfTurn2 = 1.1; //0.4
const double bfLine = 1;
const double afMove = 0.2;  //Beschleunigung in Einheiten pro Millisekunde
const double afLine = 0.15; //0.2

const double pGL1 = 0.4;
const double dGL1 = 8;

const double pGL2 = 0.14;
const double dGL2 = 3.1;

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
int longMotorUpSpeed = 80;
int longMotorDownSpeed = -80;
int longMotorDistance = 310;

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

    batteryFactor = (8045 / batteryLevel);

    std::cout << batteryFactor << std::endl;

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
    moveStraight(cSpeed, 2, "degree", miniDistance, 1, true);
}

void positionenScannen()
{
    line1(1, 90, 0.4, 8, LSr, false, "degree", 315, 90, false); //810
    for (int i = 0; i < 3; i++)
    {
        positions[i] = line1(cSpeed, 90, 0.4, 4, LSr, false, "degree", 147, 60, false, true, HTr, "color");
    }
    positions[3] = findColor(positions, "positions");
    line1(cSpeed, 90, 0.4, 8, LSr, false, "degree", 375, 1, true); //810
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
    moveStraight(1, 3, "degree", 115, 1, true);
    turn2(1, 5, "degree", 90 * richtung, 1, true);
    mediumMotor(doubleLever, -80, "degree", leverDistance, false); //down
    line2(1, 3, pGL2, dGL2, "degree", 275, 1, true);
    mediumMotor(doubleLever, 80, "time", 630, true); //up
    moveStraight(-1, -3, "crossline", 0, -1, true);
    moveStraight(1, 2, "degree", miniDistanceShort, 1, true);
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
    router[i] = line2(cSpeed, 4, 0.2, 2, "degree", 349, 4, false, true, searchSensor, "bw");
    i++;
    if (mode == "routerO")
    {
        router[i] = line2(cSpeed, 4, 0.2, 2, "degree", 349, 3, false, true, searchSensor, "bw");
        line2(cSpeed, 3, pGL2, dGL2, "degree", 360, 3, false);
        crossline(cSpeed, 1);
    }
    else
    {
        router[i] = line2(cSpeed, 4, 0.2, 2, "degree", 349, 1, true, true, searchSensor, "bw");
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

void zuRouterDrehen(int startSpeed, motor_port_t turnMotor)
{
    line2(startSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
    line2(1, 3, pGL2, dGL2, "degree", moveBackDistance, 1, true);
    turn1(turnMotor, -1, false, -4, "degree", 90, -1, true);
}

void routerEinsammeln(int longMotorSpeed, int straightDistance, int mode)
{
    //mode 1/-1: gegenüber (Vorzeichen = rechst/links) mode 2: stehen bleiben für turn2 mode 3: in Position für turn2 fahren
    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
    line2(1, 2, pGL2, dGL2, "degree", straightDistance, 1, true);
    mediumMotor(longMotor, longMotorSpeed, "degree", longMotorDistance, true);
    if (abs(mode) == 1)
    {
        turn2(1, 5, "degree", 180 * mode, 1, true);
    }
    else if (mode == 3)
    {
        moveStraight(-1, -3, "crossline", 0, -1, true);
        moveStraight(1, 2, "degree", miniDistanceShort, 1, true);
    }
}

void kabelAbladen()
{
    line2(1, 3, 0.7, dGL2, "degree", 300, 6, false);
    line2(cSpeed, 6, 0.7, dGL2, "degree", 140, 6, false);
    moveStraight(cSpeed, 6, "degree", 160, 1, true);
    //tslp_tsk(300);
    mediumMotor(doubleLever, -50, "time", 350, false);
    moveStraight(-1, -6, "degree", 320, 1, true); //400
    turn2(1, 5, "degree", 180, 1, true);
    line2(1, 2, pGL2, dGL2, "degree", 105, 1, true); //neu
    //line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false);
    //moveStraight(4, 4, "degree", miniDistance, 1, true);
}

void routerAbladen(int vonWo) //1 von Osten oder Westen; 0 von Norden oder Süden
{
    if (vonWo == 1)
    {
        line2(1, 6, 0.7, dGL2, "degree", 385, 1, true);
    }
    else
    {
        line2(1, 6, 0.7, dGL2, "degree", 123, 1, true);
    }
    mediumMotor(longMotor, -30, "degree", 210, true);
    turn2(1, 5, "degree", 3, 1, true);
    tslp_tsk(50);
    turn2(1, 5, "degree", -6, 1, true);
    tslp_tsk(50);
    turn2(1, 5, "degree", 3, 1, true);
    mediumMotor(longMotor, -30, "degree", 30, true);
}

void test()
{
    int batteryVoltage = ev3_battery_voltage_mV();

    Stopwatch voltageMessen;

    ev3_motor_set_power(motor_left, 100);
    ev3_motor_set_power(motor_right, 100);
    while (voltageMessen.getTime() < 10000)
    {
        tslp_tsk(1);
    }
    ev3_motor_stop(motor_left, true);
    ev3_motor_stop(motor_right, true);
    int motorLeft = ev3_motor_get_counts(motor_left);
    int motorRight = ev3_motor_get_counts(motor_right);

    std::cout << "batteryVoltage: " << batteryVoltage << " left " << motorLeft << " right " << motorRight << std::endl;
    return;

    int x = 1;
    while (x == 1)
    {
        moveStraight(1, 3, "degree", 500, 1, true);
        waitForButton();
        /*turn2(1, 4, "degree", 90, 1, true);
        waitForButton();
        resetMotors();
        turn2(1, 4, "degree", -90, 1, true);
        waitForButton();
        resetMotors();*/
    }
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
    else if ((blue == 3 && red == 4 && green == 1 && yellow == 2) || (blue == 2 && red == 3 && green == 4 && yellow == 1))
    {
        fall1 = 4;
        fall2 = 10;
    }
}

void task1()
{
    std::cout << "task1" << std::endl;
    updateLogDatei();

    if (fall1 == 1 || fall1 == 2 || fall1 == 3) //fall 1, 2, 3
    {
        int currentPosition;
        if (routerW[1] == 0 && routerO[1] == 0)
        {
            turn2(1, 5, "degree", 180, 1, true);
            zuRouterDrehen(1, motor_right);
            currentPosition = 1;
            routerW[1] = 3;
            routerO[1] = 3;
        }
        else if (routerW[2] == 0 && routerO[2] == 0)
        {
            zuRouterDrehen(1, motor_left);
            currentPosition = 2;
            routerW[2] = 3;
            routerO[2] = 3;
        }
        else
        {
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
            currentPosition = 0;
            routerW[0] = 3;
            routerO[0] = 3;
        }
        std::cout << "currentPosition: " << currentPosition << std::endl;
        //WO
        routerEinsammeln(longMotorDownSpeed, 170, -1);
        //OW
        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(cSpeed, 50, pGL2, dGL2, "degree", 220 + miniDistance, 1, true);
        mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true); //up

        if (fall1 == 1)
        {
            //WW
            line2(1, 3, pGL2, dGL2, "degree", 100, 1, true);
            moveStraight(-1, -2, "crossline", 0, -1, true);
            moveStraight(1, 2, "degree", miniDistanceShort, 1, true);
            if (currentPosition == 0)
            {
                turn2(1, 5, "degree", -90, 1, true);
                routerO[0] = 2;
                if (routerO[1] == 0)
                {
                    crossline(1, 1);
                    routerO[1] = 3;
                    currentPosition = 1;
                }
                else
                {
                    crossline(1, 2);
                    routerO[2] = 3;
                    currentPosition = 2;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90, 1, true);
            }
            else if (currentPosition == 1)
            {
                routerO[1] = 2;
                int x = 1;
                currentPosition = 0;
                if (routerO[2] == 0)
                {
                    currentPosition = 2;
                    x = -1;
                    routerO[2] = 3;
                }
                else
                {
                    routerO[0] = 3;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * x, 1, true);
                crossline(1, 1);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
            }
            else // currentPosition == 2
            {
                routerO[2] = 2;
                turn2(1, 3, "degree", 90, 1, true);
                if (routerO[1] == 0)
                {
                    crossline(1, 1);
                    currentPosition = 1;
                    routerO[1] = 3;
                }
                else
                {
                    currentPosition = 0;
                    routerO[0] = 3;
                    crossline(1, 2);
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", -90, 1, true);
            }
            //OW
            //line2(1, 3, pGL2, dGL2, "degree", 220, 3, false);
        }
        crossline(1, 1);
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            if (currentPosition == 2)
            {
                x = -1;
            }
            turn2(1, 5, "degree", 90 * (-x), 1, true);
            crossline(1, 1);
            turn2(1, 5, "degree", 90 * x, 1, true);
        }
    }
    else //fall 4
    {
        if (routerW[1] == 1 || (routerW[0] == 1))
        {
            zuRouterDrehen(1, motor_left);
        }
        else
        {
            turn2(1, 5, "degree", 180, 1, true);
            zuRouterDrehen(1, motor_right);
        }
        //WO
        routerEinsammeln(longMotorDownSpeed, 170, 3);
        turn2(1, 5, "degree", -90, 1, true);
        if (routerW[1] == 1)
        {
            // um von 2 zu 0 zu gehen
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
        }
        else
        {
            // um von 1 zu 0 zu gehen oder von 2 zu 1
            zuRouterDrehen(1, motor_right);
        }
        //WO
        routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
        int anzahl = 2;
        if (routerW[0] == 1)
        {
            anzahl = 3;
        }
        turn2(1, 5, "degree", -90, 1, true);
        crossline(1, anzahl);
        turn2(1, 5, "degree", -90, 1, true);
        crossline(1, 1);
        turn2(1, 5, "degree", -90, 1, true);
        crossline(1, 2);
        turn2(1, 5, "degree", 90, 1, true); //vor Kabel absetzen
    }
}

void wegbringen1()
{
    std::cout << "wegbringen1:" << std::endl;
    updateLogDatei();
    if (fall1 == 3)
    {
        turn1(motor_left, 1, false, 4, "degree", 90, 1, true);
    }
    else
    {
        turn1(motor_right, 1, false, 4, "degree", 90, 1, true);
    }
    //sline2(1, 3, pGL2, dGL2, "degree", 50, 3, false); //neu
    crossline(cSpeed, 1);
    //int currentColor; // 1 = red, 2 = blue
    for (int i = 0; i < 2; i++)
    {
        int x; //Variable für Richtung der Drehungen
        if ((fall1 == 3 && i == 0) || ((fall1 == 1 || fall1 == 2 || fall1 == 4) && i == 1))
        {
            currentColor = 1;
            x = 1;
            temporalMotor = motor_right;
        }
        else
        {
            currentColor = 2;
            x = -1;
            temporalMotor = motor_left;
        }

        if ((currentColor == 1 && (red == 1 || red == 3)) || (currentColor == 2 && (blue == 1 || blue == 3)))
        {
            turn2(1, 5, "degree", 90 * x, 1, true);
            routerAbladen(1); //node absetzen
            moveStraight(-1, -3, "degree", 113, -1, true);
            mediumMotor(longMotor, longMotorUpSpeed, "degree", 242, true); //up
            if (i == 0)
            {
                mediumMotor(doubleLever, 90, "time", leverUpTime, true);                       //up
                moveStraight(-1, -40, "degree", 260, -1, true);                                //bis kurz vor Node
                mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
                moveStraight(1, 50, "degree", 190, 1, true);                                   //evt. Linefollow!!
                mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true);   //up//node von unten nach oben getauscht
                moveStraight(1, 4, "degree", 70, 1, true);
            }
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 4, pGL2, dGL2, "degree", 92, 1, true);
            if (i == 0)
            {
                turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true); //neu
                crossline(1, 2);
            }
            else if (i == 1)
            {
                //turn2(1, 5, "degree", 90, 1, true);
                turn1(motor_left, 1, false, 4, "degree", 90, 1, true); //neu
                if (currentColor == 1)
                {
                    crossline(1, 1);
                }
                else
                {
                    crossline(1, 3);
                }
                turn2(1, 5, "degree", -90, 1, true);
            }
        }
        else
        {
            bool edge = true;
            temporalSensor = LSr;
            if (currentColor == 2)
            {
                edge = false;
                temporalSensor = LSl;
            }
            crossline(1, 1);
            turn2(1, 5, "degree", 90 * x, 1, true);
            line1(1, 3, pGL1, dGL1, temporalSensor, edge, "degree", 450, 3, false);
            line1(cSpeed, 2, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 2, false);
            moveStraight(cSpeed,2,"degree",miniDistance,1,true);//(-10)
            turn2(1, 5, "degree", 90 * x, 1, true);
            routerAbladen(0); //node absetzen
            moveStraight(-1, -3, "degree", 110, -1, true);
            mediumMotor(longMotor, longMotorUpSpeed, "degree", 242, true); //Up
            turn2(1, 5, "degree", 90 * x, 1, true);
            if (i == 0)
            {
                line2(1, 3, pGL2, dGL2, "degree", 230, 1, true);
                mediumMotor(doubleLever, 100, "time", leverUpTime, true);                      //up
                moveStraight(-1, -40, "degree", 260, -1, true);                                //bis kurz vor Node
                mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
                moveStraight(1, 40, "degree", 210, 1, true);                                   //evt. Linefollow!!
                mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true);   //up//node von unten nach oben getauscht
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                crossline(1, 3);
            }
            else if (i == 1)
            {
                crossline(1, 1);
                if (currentColor == 2)
                {
                    turn2(1, 5, "degree", 90, 1, true);
                    crossline(1, 4);
                    turn2(1, 5, "degree", -90, 1, true);
                }
            }
            else
            {
                crossline(1, 1);
            }
        }
    }
}

void task2()
{
    std::cout << "task2:" << std::endl;
    updateLogDatei();

    if (fall2 == 1 || fall2 == 4 || fall2 == 10) //fall 1, 4, 10
    {
        if (fall2 == 1 || fall2 == 4)
        {
            if (routerW[2] == 0)
            {
                currentPosition = 2;
                routerW[2] = 3;
                std::cout << "Neuer Wert routerW 2: " << routerW[2] << std::endl;
                zuRouterDrehen(1, motor_right);
            }
            else if (routerW[1] == 0)
            {
                currentPosition = 1;
                routerW[1] = 3;
                std::cout << "Neuer Wert routerW 1: " << routerW[1] << std::endl;
                line2(1, 3, pGL2, dGL2, "degree", 80, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                zuRouterDrehen(cSpeed, motor_right);
            }
            else
            {
                currentPosition = 0;
                routerW[0] = 3;
                std::cout << "Neuer Wert routerW 0: " << routerW[0] << std::endl;
                line2(1, 3, pGL2, dGL2, "degree", 80, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                zuRouterDrehen(cSpeed, motor_right);
            }
            std::cout << "currentPosition: " << currentPosition << std::endl;
            mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
            //WO
            routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
            if ((currentPosition == 1 && routerO[1] == 3) || (currentPosition == 2 && routerO[2] == 3) || (currentPosition == 0 && routerO[0] == 3))
            {
                turn2(1, 5, "degree", 180, 1, true);
            }
            else if ((currentPosition == 1 && routerO[2] == 3) || (currentPosition == 0 && routerO[1] == 3) || (currentPosition == 1 && routerO[0] == 3) || (currentPosition == 2 && routerO[1] == 3))
            {
                int x = 1;
                if ((currentPosition == 1 && routerO[0] == 3) || (currentPosition == 2 && routerO[1] == 3))
                {
                    x = -1;
                    currentPosition--;
                }
                else
                {
                    currentPosition++;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                crossline(1, 1);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            else
            {
                int x = 1;
                if (currentPosition == 2 && routerO[0] == 3)
                {
                    x = -1;
                    currentPosition = 0;
                }
                else
                {
                    currentPosition = 2;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * x, 1, true);
                crossline(1, 2);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            routerO[currentPosition] = 2;
            std::cout << "Neuer Wert routerO " << currentPosition << ": " << routerO[currentPosition] << std::endl;
            line2(1, 4, pGL2, dGL2, "degree", 197, 1, true); //Node zu Wechsel bringen //?
            //WW + OO oder OW
            mediumMotor(longMotor, longMotorDownSpeed + 40, "degree", longMotorDistance, true); //down
            moveStraight(-1, -3, "degree", 187, 1, true);                                       //?
            mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true);        //up
            if (fall2 == 4)
            {
                if (currentPosition == 0 || currentPosition == 2)
                {
                    int x = 1;
                    if (currentPosition == 0)
                    {
                        x = -1;
                        currentPosition = 2;
                    }
                    else
                    {
                        currentPosition = 0;
                    }
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    if (routerO[1] == 0)
                    {
                        currentPosition = 1;
                        crossline(1, 1);
                    }
                    else
                    {
                        crossline(1, 2);
                    }
                    std::cout << "currentPosition: " << currentPosition << std::endl;
                    turn2(1, 5, "degree", 90 * (-x), 1, true);
                }
                else
                {
                    int x = 1;
                    currentPosition = 0;
                    if (routerO[2] == 0)
                    {
                        x = -1;
                        currentPosition = 2;
                    }
                    std::cout << "currentPosition: " << currentPosition << std::endl;
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    crossline(1, 1);
                    turn2(1, 5, "degree", 90 * (-x), 1, true);
                }
                line2(1, 4, pGL2, dGL2, "degree", 270, 4, false);
                line2(4, 2, pGL2, dGL2, "degree", 100, 1, true);
                mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
                crossline(1, 1);
                if ((currentPosition == 1 && routerO[2] == 2) || (currentPosition == 0 && routerO[1] == 2) || (currentPosition == 1 && routerO[0] == 2) || (currentPosition == 2 && routerO[1] == 2))
                {
                    int x = -1;
                    secondTemporalMotor = motor_left;
                    if ((currentPosition == 1 && routerO[0] == 2) || (currentPosition == 2 && routerO[1] == 2))
                    {
                        currentPosition--;
                        x = 1;
                        secondTemporalMotor = motor_right;
                    }
                    else
                    {
                        currentPosition++;
                    }
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    zuRouterDrehen(1, secondTemporalMotor);
                }
                else
                {
                    int x = -1;
                    if (currentPosition == 2 && routerO[0] == 2)
                    {
                        x = 1;
                        secondTemporalMotor = motor_right;
                        currentPosition = 0;
                    }
                    else
                    {
                        currentPosition = 2;
                        secondTemporalMotor = motor_left;
                    }
                    std::cout << "currentPosition: " << currentPosition << std::endl;
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                    zuRouterDrehen(cSpeed, secondTemporalMotor);
                }
            }
            else
            {
                int x = 1;
                temporalMotor = motor_right;
                secondTemporalMotor = motor_left;
                if (fall2 == 1 && currentPosition == 2)
                {
                    x = -1;
                    temporalMotor = motor_left;
                    secondTemporalMotor = motor_right;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                if (fall2 == 1 && currentPosition == 1)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 1, true);
                }
                else
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                }
                turn1(temporalMotor, 1, false, 3, "degree", 90, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                if (currentPosition == 1)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                    zuRouterDrehen(cSpeed, secondTemporalMotor);
                }
                else
                {
                    zuRouterDrehen(1, secondTemporalMotor);
                }
            }
        }
        else //fall 10
        {
            secondTemporalMotor = motor_right;
            turn2(1, 5, "degree", 90, 1, true);
            for (int i = 0; i < 2; i++)
            {
                crossline(1, 1);
                turn2(1, 5, "degree", 90, 1, true);
            }
            if (routerO[2] == 0)
            {
                currentPosition = 2;
                routerO[2] = 3;
                std::cout << "Neuer Wert routeO 2: " << routerO[2] << std::endl;
                zuRouterDrehen(1, secondTemporalMotor);
            }
            else if (routerO[1] == 0)
            {
                currentPosition = 1;
                routerO[1] = 3;
                std::cout << "Neuer Wert routerO 1: " << routerO[1] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                zuRouterDrehen(cSpeed, secondTemporalMotor);
            }
            else
            {
                currentPosition = 0;
                routerO[0] = 3;
                std::cout << "Neuer Wert routerO 0: " << routerO[0] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                zuRouterDrehen(cSpeed, secondTemporalMotor);
            }
            std::cout << "currentPosition: " << currentPosition << std::endl;
        }
        if (fall2 == 1 || fall2 == 10) //Ausgangsposition long oben
        {
            routerEinsammeln(longMotorDownSpeed, 170, 3);
            int x = 1;
            if (currentPosition == 0 || currentPosition == 2)
            {
                temporalMotor = motor_left;
                if (currentPosition == 2)
                {
                    x = -1;
                    temporalMotor = motor_right;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                if (routerO[1] == 2 || routerO[1] == 0)
                {
                    currentPosition = 1;
                    zuRouterDrehen(1, temporalMotor);
                }
                else
                {
                    if (currentPosition == 2)
                    {
                        currentPosition = 0;
                    }
                    else
                    {
                        currentPosition = 2;
                    }
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                    zuRouterDrehen(cSpeed, temporalMotor);
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
            }
            else
            {
                currentPosition = 2;
                temporalMotor = motor_left;
                if (routerO[0] == 2 || routerO[0] == 0)
                {
                    x = -1;
                    currentPosition = 0;
                    temporalMotor = motor_right;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * x, 1, true);
                zuRouterDrehen(1, temporalMotor);
            }
        }

        //OO
        routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
        entscheidung = 0;
        int rotation = 1;
        int anzahl = 1;
        if (currentPosition == 2)
        {
            entscheidung = 1;
            rotation = -1;
        }
        if (currentPosition == 1)
        {
            anzahl = 2;
        }
        turn2(1, 5, "degree", -90 * rotation, 1, true);
        crossline(1, anzahl);
        turn2(1, 5, "degree", -90 * rotation, 1, true);
    }
    else if (fall2 == 2 || fall2 == 3) //fall 2, 3
    {
        turn2(1, 5, "degree", 90, 1, true);
        for (int i = 0; i < 2; i++)
        {
            crossline(1, 1);
            turn2(1, 5, "degree", 90, 1, true);
        }
        if (routerO[2] == 2)
        {
            currentPosition = 2;
            routerO[2] = 3;
            std::cout << "Neuer Wert routerO 2: " << routerO[2] << std::endl;
            zuRouterDrehen(1, motor_right);
        }
        else if (routerO[1] == 2)
        {
            currentPosition = 1;
            routerO[1] = 3;
            std::cout << "Neuer Wert routerO 1: " << routerO[1] << std::endl;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
        }
        else
        {
            currentPosition = 0;
            routerO[0] = 3;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
        }
        std::cout << "currentPosition: " << currentPosition << std::endl;
        //OO
        routerEinsammeln(longMotorDownSpeed, 170, 2);
        line2(1, 3, pGL2, dGL2, "degree", 390, 1, true);
        if ((currentPosition == 1 && routerW[1] == 0) || (currentPosition == 2 && routerW[2] == 0) || (currentPosition == 0 && routerW[0] == 0))
        {
            //NICHT entfernen !!!
        }
        else if ((currentPosition == 1 && routerW[2] == 0) || (currentPosition == 0 && routerW[1] == 0) || (currentPosition == 1 && routerW[0] == 0) || (currentPosition == 2 && routerW[1] == 0))
        {
            temporalMotor = motor_left;
            if ((currentPosition == 1 && routerW[0] == 0) || (currentPosition == 2 && routerW[1] == 0))
            {
                currentPosition--;
                temporalMotor = motor_right;
            }
            else
            {
                currentPosition++;
            }
            turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
            zuRouterDrehen(1, temporalMotor);
        }
        else
        {
            if (currentPosition == 2 && routerW[0] == 0)
            {
                currentPosition = 0;
                temporalMotor = motor_right;
            }
            else
            {
                currentPosition = 2;
                temporalMotor = motor_left;
            }
            turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, temporalMotor);
        }
        //WO
        routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
        routerW[currentPosition] = 3;
        entscheidung = 0;
        int rotation = 1;
        int anzahl = 2;
        if (currentPosition == 2)
        {
            entscheidung = 1;
            rotation = -1;
        }
        if (currentPosition == 1)
        {
            anzahl = 3;
        }
        turn2(1, 5, "degree", -90 * rotation, 1, true);
        crossline(1, anzahl);
        turn2(1, 5, "degree", -90 * rotation, 1, true);
        crossline(1, 1);
    }
    else if (fall2 == 5 || fall2 == 6 || fall2 == 7) //fall 5, 6, 7
    {
        if (routerW[2] == 0)
        {
            currentPosition = 2;
            zuRouterDrehen(1, motor_right);
        }
        else if (routerW[1] == 0)
        {
            currentPosition = 1;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
        }
        else
        {
            currentPosition = 0;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, motor_right);
        }
        std::cout << "currentPosition: " << currentPosition << std::endl;
        //WO
        routerEinsammeln(longMotorDownSpeed, 170, 3);
        if (fall2 == 5 || fall2 == 6)
        {
            if ((currentPosition == 1 && routerO[1] == 0) || (currentPosition == 2 && routerO[2] == 0) || (currentPosition == 0 && routerO[0] == 0))
            {
                turn2(1, 5, "degree", 180, 1, true);
            }
            else if ((currentPosition == 1 && routerO[2] == 0) || (currentPosition == 0 && routerO[1] == 0) || (currentPosition == 1 && routerO[0] == 0) || (currentPosition == 2 && routerO[1] == 0))
            {

                int x = 1;
                if ((currentPosition == 1 && routerO[0] == 0) || (currentPosition == 2 && routerO[1] == 0))
                {
                    x = -1;
                    currentPosition--;
                }
                else
                {
                    currentPosition++;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                crossline(1, 1);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            else
            {
                int x = 1;
                if (currentPosition == 0 && routerO[2] == 0)
                {
                    x = -1;
                    currentPosition = 2;
                }
                else
                {
                    currentPosition = 0;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                crossline(1, 2);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
            }
            routerO[currentPosition] = 2;
            //OW
            line2(cSpeed, 3, pGL2, dGL2, "degree", 220, 1, true);
            mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true); //up
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        else
        {
            int x = 1;
            if ((currentPosition == 1 && (routerO[0] == 3 || routerO[2] == 3)) || ((currentPosition == 2 || currentPosition == 0) && routerO[1] == 3))
            {
                if (currentPosition == 0 || (currentPosition == 1 && routerO[2] == 3))
                {
                    x = -1;
                    currentPosition++;
                }
                else
                {
                    currentPosition--;
                }
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                crossline(1, 1);
            }
            else
            {
                if (currentPosition == 2)
                {
                    x = 1;
                    currentPosition = 0;
                }
                else
                {
                    x = -1;
                    currentPosition = 2;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                crossline(1, 2);
            }
            turn2(1, 5, "degree", 90 * (-x), 1, true);
            //Fährt über bereits leeres Feld zur Ostreihe
            crossline(cSpeed, 1);
            if ((currentPosition == 1 && (routerO[0] == 0 || routerO[2] == 0)) || ((currentPosition == 2 || currentPosition == 0) && routerO[1] == 0))
            {
                int x = 1;
                if (currentPosition == 0 || (currentPosition == 1 && routerO[2] == 0))
                {
                    x = -1;
                    currentPosition++;
                    temporalMotor = motor_left;
                }
                else
                {
                    currentPosition--;
                    temporalMotor = motor_right;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                zuRouterDrehen(1, temporalMotor);
            }
            else
            {
                int x = 1;
                temporalMotor = motor_right;
                if (currentPosition == 0)
                {
                    x = -1;
                    currentPosition = 2;
                    temporalMotor = motor_left;
                }
                else
                {
                    currentPosition = 0;
                }
                std::cout << "currentPosition: " << currentPosition << std::endl;
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
                zuRouterDrehen(cSpeed, temporalMotor);
            }
            //OO
            routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
        }
        if (currentPosition == 0 || currentPosition == 2)
        {
            std::cout << "currentPosition: " << currentPosition << std::endl;
            int x = 1;
            entscheidung = 0;
            if (currentPosition == 2)
            {
                entscheidung = 1;
                x = -1;
            }
            int rotation = 1;
            if (fall2 == 7)
            {
                rotation = -1;
            }
            turn2(1, 5, "degree", 90 * x * rotation, 1, true);
            crossline(1, 1);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
        }
        else
        {
            int rotation = 1;
            if (fall2 == 7)
            {
                rotation = -1;
            }
            entscheidung = 0;
            turn2(1, 5, "degree", 90 * rotation, 1, true);
            crossline(1, 2);
            turn2(1, 5, "degree", -90, 1, true);
        }
    }
    else if (fall2 == 8 || fall2 == 9) //fall 8, 9
    {
        if (routerO[2] == 0)
        {
            currentPosition = 2;
            routerO[2] = 3;
            crossline(1, 1);
        }
        else if (routerO[1] == 0)
        {
            currentPosition = 1;
            routerO[1] = 3;
            crossline(1, 2);
        }
        else
        {
            currentPosition = 0;
            routerO[0] = 3;
            crossline(1, 3);
        }
        std::cout << "currentPosition: " << currentPosition << std::endl;
        turn2(1, 5, "degree", -90, 1, true);
        //OW
        crossline(cSpeed, 1);
        mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
        if ((currentPosition == 1 && (routerO[0] == 0 || routerO[2] == 0)) || ((currentPosition == 2 || currentPosition == 0) && routerO[1] == 0))
        {
            int x = 1;
            if (currentPosition == 0 || (currentPosition == 1 && routerO[2] == 0))
            {
                x = -1;
                currentPosition++;
                temporalMotor = motor_left;
            }
            else
            {
                currentPosition--;
                temporalMotor = motor_right;
            }
            turn2(1, 5, "degree", 90 * x, 1, true);
            zuRouterDrehen(1, temporalMotor);
        }
        else
        {
            int x = 1;
            if (currentPosition == 0)
            {
                x = -1;
                currentPosition = 2;
                temporalMotor = motor_left;
            }
            else
            {
                currentPosition = 0;
                temporalMotor = motor_right;
            }
            std::cout << "currentPosition: " << currentPosition << std::endl;
            turn2(1, 5, "degree", 90 * x, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(cSpeed, 3, pGL2, dGL2, "degree", 80, 3, false);
            zuRouterDrehen(cSpeed, temporalMotor);
        }
        //OO
        routerEinsammeln(longMotorUpSpeed, miniDistance, 2);
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            entscheidung = 0;
            if (currentPosition == 2)
            {
                entscheidung = 1;
                x = -1;
            }
            turn2(1, 5, "degree", -90 * x, 1, true);
            crossline(1, 1);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
        }
        else
        {
            entscheidung = 0;
            turn2(1, 5, "degree", -90, 1, true);
            crossline(1, 2);
            turn2(1, 5, "degree", -90, 1, true);
        }
    }
}

void welcherWeg()
{
    std::cout << "welcherWeg entscheidung: " << entscheidung << std::endl;
    //zum wegbringen fahren _________________________________________________________________________________________________________________________________________________
    int x = 1;
    bool lineEdge = false;
    temporalMotor = motor_left;
    temporalSensor = LSl;
    if (entscheidung == 0) //south
    {
        x = -1;
        temporalMotor = motor_right;
        temporalSensor = LSr;
        lineEdge = true;
    }
    // zum schwabbelabsetzen fahren
    line1(cSpeed, 4, pGL1, dGL1+1, temporalSensor, lineEdge, "degree", 1380, 4, false);
    line1(cSpeed, 4, pGL1, dGL1+1, temporalSensor, lineEdge, "crossline", 0, 4, false);
    line1(cSpeed, 4, pGL1, dGL1+1, temporalSensor, lineEdge, "degree", 540, 1, true);
    turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
    line1(1, 4, pGL1, dGL1, temporalSensor, lineEdge, "degree", 520, 3, false);
    crossline(cSpeed, 1);
    turn2(1, 5, "degree", 90 * x, 1, true);
}

void wegbringen2()
{
    //1 == yellow , 2 == green
    std::cout << "wegbringen2" << std::endl;
    updateLogDatei();
    if (fall2 == 2 || fall2 == 6 || fall2 == 8) //Achtung: Fall2 wird geändert !!!
    {
        fall2 = 2;
        turn1(motor_left, 1, false, 4, "degree", 90, 1, true);
        //turn2(1, 5, "degree", 90, 1, true);
    }
    else
    {
        fall2 = 1;
        turn1(motor_right, 1, false, 4, "degree", 90, 1, true);
        //turn2(1, 5, "degree", -90, 1, true);
    }
    //line2(1, 3, pGL2, dGL2, "degree", 50, 3, false);
    crossline(1, 1);
    for (int i = 0; i < 2; i++)
    {
        int x; //Variable für Richtung der Drehungen
        if ((fall2 == 2 && i == 0) || (fall2 == 1 && i == 1))
        {
            currentColor = 1;
            x = 1;
        }
        else
        {
            currentColor = 2;
            x = -1;
        }

        if ((currentColor == 1 && (yellow == 1 || yellow == 3)) || (currentColor == 2 && (green == 1 || green == 3)))
        {
            turn2(1, 5, "degree", 90 * x, 1, true);
            routerAbladen(1); //node absetzen
            moveStraight(-1, -3, "degree", 113, -1, true);
            mediumMotor(longMotor, longMotorUpSpeed, "degree", 242, true); //up
            if (i == 0)
            {
                mediumMotor(doubleLever, 90, "time", leverUpTime, true);                       //up
                moveStraight(-1, -40, "degree", 260, -1, true);                                //bis kurz vor Node
                mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
                moveStraight(1, 50, "degree", 190, 1, true);                                   //evt. Linefollow!!
                mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true);   //up//node von unten nach oben getauscht
            }
            turn2(1, 5, "degree", 180, 1, true);
            crossline(1, 1);
            if (i == 0)
            {
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                temporalSensor = LSr;
                bool edge = true;
                if (currentColor == 2)
                {
                    temporalSensor = LSl;
                    edge = false;
                }
                line1(1, 3, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 3, false);
                for (int i = 1; i < 2; i++)
                {
                    line1(cSpeed, 3, pGL1, dGL1, temporalSensor, edge, "degree", 80, 3, false);
                    line1(cSpeed, 3, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 3, false);
                }
                moveStraight(cSpeed, 2, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                turn2(1, 5, "degree", 90, 1, true);
                if (currentColor == 1)
                {
                    crossline(1, 1);
                }
                else
                {
                    //line1

                    line1(1, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                    for (int i = 1; i < 3; i++)
                    {
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "degree", 80, 3, false);
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                    }
                    moveStraight(cSpeed, 2, "degree", miniDistance, 1, true);
                }
                turn2(1, 5, "degree", -90, 1, true);
            }
        }
        else
        {
            bool edge = true;
            temporalSensor = LSr;
            if (currentColor == 2)
            {
                edge = false;
                temporalSensor = LSl;
            }
            crossline(1, 1);
            turn2(1, 5, "degree", 90 * x, 1, true);
            line1(1, 3, pGL1, dGL1, temporalSensor, edge, "degree", 450, 3, false);
            line1(cSpeed, 2, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 2, false);
            moveStraight(cSpeed,2,"degree",miniDistance,1,true);//(-10)
            turn2(1, 5, "degree", 90 * x, 1, true);
            routerAbladen(0); //node absetzen
            moveStraight(-1, -3, "degree", 110, -1, true);
            mediumMotor(longMotor, longMotorUpSpeed, "degree", 242, true); //Up
            turn2(1, 5, "degree", 90 * x, 1, true);
            if (i == 0)
            {
                line2(1, 3, pGL2, dGL2, "degree", 230, 1, true);
                mediumMotor(doubleLever, 100, "time", leverUpTime, true);                      //up
                moveStraight(-1, -40, "degree", 260, -1, true);                                //bis kurz vor Node
                mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
                moveStraight(1, 40, "degree", 210, 1, true);                                   //evt. Linefollow!!
                mediumMotor(longMotor, longMotorUpSpeed, "degree", longMotorDistance, true);   //up//node von unten nach oben getauscht
                crossline(1, 1);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                temporalSensor = LSr;
                bool edge = true;
                if (currentColor == 2)
                {
                    temporalSensor = LSl;
                    edge = false;
                }
                line1(1, 3, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 3, false);
                for (int i = 1; i < 3; i++)
                {
                    line1(cSpeed, 3, pGL1, dGL1, temporalSensor, edge, "degree", 80, 3, false);
                    line1(cSpeed, 3, pGL1, dGL1, temporalSensor, edge, "crossline", 0, 3, false);
                }
                moveStraight(cSpeed, 2, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                crossline(1, 1);
                if (currentColor == 2)
                {
                    turn2(1, 5, "degree", 90, 1, true);
                    line1(1, 90, pGL1, dGL1, LSl, false, "degree", 1250, 3, false);
                    line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                    moveStraight(1, 3, "degree", miniDistance, 1, true);
                    turn2(1, 5, "degree", -90, 1, true);
                }
            }
            else
            {
                crossline(1, 1);
            }
        }
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

    //test();
    //return;

    //Anfang - Scannt Positionen, scannt Router und sammelt das südliche Kabel
    moveStraight(30, 50, "degree", 18, 30, true);
    turn1(motor_left, 1, false, 4, "degree", -90, 1, true);
    moveStraight(20, 3, "degree", 65, 3, false);
    positionenScannen();
    turn1(motor_right, 1, false, 4, "degree", -90, 1, true);
    line1(cSpeed, 100, pGL1, dGL1, LSr, true, "degree", 1900, 70, false);
    line1(cSpeed, 100, pGL1, dGL1, LSr, true, "degree", 380, 3, false);
    line1(cSpeed, 3, 0.3, 5, LSr, true, "crossline", 0, 3, false);
    moveStraight(cSpeed, 3, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", -90, 1, true);
    routerScannen(HTr, "routerO");
    turn2(1, 5, "degree", 90, 1, true);
    kabelSammeln(true);
    routerScannen(HTl, "routerW");

    fallunterscheidung();
    //arrays router: 0 schwarz; 1 weiß; 2 getauscht; 3 leer
    task1();
    kabelAbladen();
    wegbringen1();
    kabelSammeln(false);
    task2();
    welcherWeg();
    kabelAbladen();
    wegbringen2();
    mediumMotor(longMotor, longMotorDownSpeed, "degree", longMotorDistance, true); //down
    moveStraight(1, 100, "degree", 270, 100, false);
    turn1(motor_left, 100, true, 4, "degree", 50, 100, false);
    int neededTime = run.getTime();
    moveStraight(100, 100, "time", 300, 1, true);
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
}
//Das Programm braucht noch eine Zeile hintendran, sonst kompiliert es nicht