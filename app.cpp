
#pragma once
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
motor_port_t longMotor = EV3_PORT_B;   //Motor für Router
motor_port_t doubleLever = EV3_PORT_C; //Motor für Kabel
motor_port_t motor_right = EV3_PORT_D;
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

// Variables for Moves
double pGain = 0.02; //0.017
double pGainTurn2 = 1.5;
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.22; // Je höher, desto früher wird gebremst
const double bfTurn1 = 0.8;
const double bfTurn2 = 0.6; //0.4
const double bfLine = 1;
const double afMove = 0.2; //Beschleunigung in Einheiten pro Millisekunde
const double afLine = 0.2;

const double pGL1 = 0.4;
const double dGL1 = 8;

const double pGL2 = 0.14;
const double dGL2 = 3.1;

// Constants for vertical line alignment
const double pi = 3.1415926535897932384626433832795029;
const double wheelDiameter = 6.24; //6.24;
const double wheelCircumferance = 17.6;
//const double wheelConverter = 6.24 / wheelDiameter;

//Distances for main
int miniDistance = 55;      //55     //Distanz um direkt wieder perfekt auf Linie zu stehen
int turnBackDistance = 220; //Distanz vor einer Drehung zum Router
int leverDistance = 110;
int wallDistanceE = 0;

//Values for Line1
int LSrMiddle = (92 + 9) / 2;
double batteryFactor = 1;

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
int currentPosition1 = 5;
int currentColor;
Stopwatch run;

void updateLogDatei()
{
    std::cout << "time: " << run.getTime() << std::endl;
    std::cout << "fall1: " << fall1 << std::endl;
    std::cout << "fall2: " << fall2 << std::endl;
    std::cout << "routerO 0: " << routerO[0] << std::endl;
    std::cout << "routerO 1: " << routerO[1] << std::endl;
    std::cout << "routerO 2: " << routerO[2] << std::endl;
    std::cout << "routerW 0: " << routerW[0] << std::endl;
    std::cout << "routerW 1: " << routerW[1] << std::endl;
    std::cout << "routerW 2: " << routerW[2] << std::endl;
}

void routerAbladen(int vonWo) //1 von Osten oder Westen; 0 von Norden oder Süden
{
    resetMotors();
    if (vonWo == 1)
    {
        line2(35, 40, 0.7, dGL2, "degree", 400, 0, true);
    }
    else
    {
        line2(35, 40, 0.7, dGL2, "degree", 128, 0, true);
    }
    mediumMotor(longMotor, -30, "degree", 242, true);
    turn2(1, 5, "degree", 2, 25, true);
    turn2(1, 5, "degree", -4, 25, true);
    turn2(1, 5, "degree", 2, 25, true);
}

void kabelSammeln(bool south)
{
    int richtung = 1;
    if (south)
    {
        richtung = -1;
    }
    line2(cSpeed, 3, pGL2, dGL2, "crossline", 277, 70, false);
    moveStraight(cSpeed, 50, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", -90 * richtung, 1, true);
    line2(cSpeed, 3, 0.17, 3.5, "crossline", 0, 3, false);
    moveStraight(cSpeed, 50, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", 90 * richtung, 1, true);
    mediumMotor(doubleLever, -60, "degree", leverDistance, false); //down
    line2(1, 70, 0.17, 3.5, "degree", 275, 1, true);
    mediumMotor(doubleLever, 80, "time", 700, true); //up
    resetMotors();
    moveStraight(-1, -3, "crossline", 0, -1, true);
    moveStraight(1, 70, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", -90 * richtung, 1, true);
}

void kabelAbladen()
{
    resetMotors();
    line2(35, 4, 0.7, dGL2, "degree", 300, 3, false);
    line2(cSpeed, 40, 0.7, dGL2, "degree", 140, 40, false);
    resetMotors();
    moveStraight(cSpeed, 40, "degree", 180, 20, true);
    //tslp_tsk(300);
    mediumMotor(doubleLever, -50, "degree", leverDistance - 10, false);
    resetMotors();
    moveStraight(-10, -40, "degree", 340, 20, true); //400
    turn2(1, 5, "degree", 180, 1, true);
    line2(1, 4, pGL2, dGL2, "degree", 80, 1, true); //neu
    //line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false);
    //moveStraight(4, 4, "degree", miniDistance, 1, true);
}

void positionenScannen()
{
    line1(1, 90, 0.4, 8, LSr, false, "degree", 315, 40, false); //810
    for (int i = 0; i < 3; i++)
    {
        positions[i] = line1(cSpeed, 90, 0.4, 4, LSr, false, "degree", 147, 60, false, true, HTr, "color");
    }
    ev3_speaker_play_tone(NOTE_E4, 10);
    positions[3] = findColor(positions, "positions");
    line1(cSpeed, 100, 0.4, 8, LSr, false, "degree", 390, 1, true); //810
}

void routerScannen(sensor_port_t searchSensor, std::string mode)
{
    //direkt nach der Drehung
    int router[3] = {0};
    int i = 0;
    if (mode == "routerO")
    {
        resetMotors();
        line2(1, 4, pGL2, dGL2, "degree", 165, 4, false);
    }
    else
    {
        line2(1, 4, pGL2, dGL2, "degree", 40, 90, false);
    }
    router[i] = line2(cSpeed, 90, 0.2, 2, "degree", 349, 90, false, true, searchSensor, "bw");
    i++;
    if (mode == "routerO")
    {
        router[i] = line2(cSpeed, 90, 0.2, 2, "degree", 349, 4, false, true, searchSensor, "bw");
        line2(cSpeed, 4, pGL2, dGL2, "degree", 360, 4, false);
        line2(cSpeed, 4, pGL2,dGL2, "crossline", 0, 3, false);
        moveStraight(cSpeed, 3, "degree", miniDistance, 1, true);
        //line2(cSpeed, 4, 0.2, 4, "degree", 305, 1, true);
    }
    else
    {
        router[i] = line2(cSpeed, 90, 0.2, 2, "degree", 349, 1, true, true, searchSensor, "bw");
        
    }
    i++;
    router[i] = findColor(router, "router");

    if (mode == "routerO")
    {
        for (int x = 0; x < 3; x++)
        {
            routerO[2 - x] = router[x];
        }
        std::cout << "RouterO0: " << routerO[0] << std::endl;
        std::cout << "RouterO1: " << routerO[1] << std::endl;
        std::cout << "RouterO2: " << routerO[2] << std::endl;
    }
    else
    {
        for (int x = 0; x < 3; x++)
        {
            routerW[x] = router[x];
        }
        std::cout << "RouterW0: " << routerW[0] << std::endl;
        std::cout << "RouterW1: " << routerW[1] << std::endl;
        std::cout << "RouterW2: " << routerW[2] << std::endl;
    }
}

void routerAufnehmen(motor_port_t turnMotor) // mit welchem Rad zurück
{
    //Position anfang: 90° versetzt vor node ohne minimove
    line2(cSpeed, 3, pGL2, dGL2, "degree", 180, 1, true);
    resetMotors();
    turn1(turnMotor, -1, false, -4, "degree", 90, -1, true);
    mediumMotor(longMotor, -50, "degree", 315, true);
    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
    line2(3, 3, pGL2, dGL2, "degree", 92, 1, true);
    mediumMotor(longMotor, 50, "degree", 315, true);
    moveStraight(1, -3, "degree", 92 - miniDistance, 1, true);
    //Position Ende: auf Cross vor Node
}

void test()
{
    turn2(1,5,"degree",360,1,true);
    waitForButton();

    line2(cSpeed, 90, 0.2, 2, "degree", 1200, 60, true, true, HTr, "bw");
    return;
    line1(cSpeed, 90, 0.4, 4, LSr, false, "degree", 1200, 60, true, true, HTr, "color");
    return;
    /*
    display(positions[0]);
    waitForButton();
    tslp_tsk(300);
    display(positions[1]);
    waitForButton();
    tslp_tsk(300);
    display(positions[2]);
    waitForButton();
    tslp_tsk(300);
    display(positions[3]);
    waitForButton();
    tslp_tsk(300);
    line2(20, 100, 0.2, 2, "degree", 1150, 1, true, true, HTr, "bw");
    waitForButton();
    line1(15, 100, 1, 25, LSr, true, "degree", 2500, 80, true);
    */

    resetMotors();
    turn1(motor_right, -1, false, -3, "degree", 720, -10, true);
    return;
    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
    moveStraight(3, 3, "degree", miniDistance, 1, true);

    return;
}

void start() //Konfiguration für den Start
{
    //Konfiguration der Hardware
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_motor_config(motor_left, UNREGULATED_MOTOR);
    ev3_motor_config(motor_right, UNREGULATED_MOTOR);
    ev3_motor_config(longMotor, UNREGULATED_MOTOR);
    ev3_motor_config(doubleLever, UNREGULATED_MOTOR);
    ev3_sensor_config(HTl, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(HTr, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSl, COLOR_SENSOR);
    ev3_sensor_config(LSr, COLOR_SENSOR);
    ev3_speaker_play_tone(NOTE_E4, 500);
    tslp_tsk(500);

    std::cout << "Battery at: " << ev3_battery_voltage_mV() << "Volt" << std::endl;
    display(ev3_battery_voltage_mV());

    waitForButton();
    run.reset();
    resetMotors();
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

    std::cout << "Fall1: " << fall1 << " Fall2: " << fall2 << std::endl;
    std::cout << "blue: " << blue << " red: " << red << " green: " << green << " yellow: " << yellow << std::endl;
}
void task1()
{
    std::cout << "task1" << std::endl;
    updateLogDatei();

    if (fall1 == 1 || fall1 == 2 || fall1 == 3)
    {
        int currentPosition;

        if (routerW[1] == 0 && routerO[1] == 0)
        {
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 180, 1, true);
            resetMotors();
            turn1(motor_right, -1, false, -4, "degree", 90, -1, true);
            currentPosition = 1;
            routerW[1] = 3;
            routerO[1] = 3;
        }
        else if (routerW[2] == 0 && routerO[2] == 0)
        {
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(1, 3, pGL2, dGL2, "degree", 180, 1, true);
            resetMotors();
            turn1(motor_left, -1, false, -4, "degree", 90, -1, true);
            currentPosition = 2;
            routerW[2] = 3;
            routerO[2] = 3;
        }
        else
        {
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 180, 1, true);
            resetMotors();
            turn1(motor_right, -1, false, -4, "degree", 90, -1, true);
            currentPosition = 0;
            routerW[0] = 3;
            routerO[0] = 3;
        }
        line2(1, 3, pGL2, dGL2, "degree", 295, 1, true);
        mediumMotor(longMotor, -50, "degree", 315, true); //down
        if (currentPosition == 2)
        {
            turn2(-1, -5, "degree", 180, -1, true);
        }
        else
        {
            turn2(1, 5, "degree", 180, 1, true);
        }
        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(3, 50, pGL2, dGL2, "degree", 300, 1, true);
        mediumMotor(longMotor, 50, "degree", 315, true); //up

        if (fall1 == 1)
        {
            line2(1, 3, pGL2, dGL2, "degree", 72, 1, true);
            tslp_tsk(5);
            moveStraight(-1, -2, "crossline", 0, -1, true);
            moveStraight(1, 2, "degree", miniDistance, 1, true);
            if (currentPosition == 0)
            {
                turn2(1, 5, "degree", -90, 1, true);
                routerO[0] = 2;
                if (routerO[1] == 0)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                    routerO[1] = 3;
                    currentPosition = 1;
                }
                else
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 1, true);
                    routerO[2] = 3;
                    currentPosition = 2;
                }
                moveStraight(1, 3, "degree", miniDistance, 1, true);
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
                turn2(1, 5, "degree", 90 * x, 1, true);

                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                line2(50, 50, pGL2, dGL2, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * (-x), 1, true);
            }
            else // currentPosition == 2
            {
                routerO[2] = 2;
                turn2(1, 3, "degree", 90, 1, true);
                if (routerO[1] == 0)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    currentPosition = 1;
                    routerO[1] = 3;
                }
                else
                {
                    currentPosition = 0;
                    routerO[0] = 3;
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    line2(50, 50, pGL2, dGL2, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                }
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", -90, 1, true);
            }

            line2(1, 3, pGL2, dGL2, "degree", 240, 1, true);
        }

        moveStraight(1, 3, "degree", 185, 3, false);
        line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
        moveStraight(50, 50, "degree", miniDistance, 1, true);
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            if (currentPosition == 2)
            {
                x = -1;
            }
            turn2(1, 5, "degree", 90 * (-x), 1, true);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90 * x, 1, true);
        }
    }
    else
    { //fall4--------------------------------------------------------------------------------------------------------------
        if (routerW[1] == 1 || (routerW[0] == 1))
        {
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(1, 3, pGL2, dGL2, "degree", 180, 1, true); //um zu 2 zu gehen
            resetMotors();
            turn1(motor_left, -1, false, -4, "degree", 90, -1, true);
        }
        else
        {
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 180, 1, true); //um zu 1 zu gehen
            resetMotors();
            turn1(motor_right, -1, false, -4, "degree", 90, -1, true);
        }
        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(3, 3, pGL2, dGL2, "degree", 202, 1, true);
        mediumMotor(longMotor, -50, "degree", 315, true); //down
        moveStraight(-1, -3, "crossline", 0, -1, true);
        moveStraight(1, 3, "degree", miniDistance, 1, true);
        turn2(1, 5, "degree", -90, 1, true);
        if (routerW[1] == 1)
        {
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false); // um von 2 zu 0 zu gehen
        }
        else
        {                                                      // routerW[2] == 0
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false); // um von 1 zu 0 zu gehen oder von 2 zu 1
        }
        line2(3, 3, pGL2, dGL2, "degree", 180, 1, true); //moveBackDistance
        turn1(motor_right, -1, false, -4, "degree", 90, -1, true);
        line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
        line2(50, 50, pGL2, dGL2, "degree", miniDistance, 1, true); //bis Node eingesammelt ist
        mediumMotor(longMotor, 50, "degree", 315, true);            //up
        if (routerW[0] == 1)
        {
            moveStraight(1, 4, "degree", 105, 1, true);
            resetMotors();
            turn1(motor_left, 1, false, 4, "degree", 93, 1, true);
            waitForButton();
            resetMotors();
            turn1(motor_left, 1, false, 4, "degree", 93, 1, true);
            waitForButton();
            resetMotors();
            turn1(motor_right, 1, false, 4, "degree", 93, 1, true);
            waitForButton();
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            for (int i = 0; i < 2; i++)
            {
                turn2(1, 5, "degree", 90, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
            }
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", -90, 1, true); //bevor Schwabell absetzen
        }
        else
        {
            turn1(motor_right, 1, false, 4, "degree", 90, 1, true);
            moveStraight(1, 3, "degree", 110, 1, true);
            turn1(motor_right, 1, false, 4, "degree", 90, 1, true);
            moveStraight(1, 3, "degree", 40, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", -90, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90, 1, true); //bevor Schwabell absetzen
        }
    }
}

void wegbringen1()
{
    std::cout << "wegbringen1:" << std::endl;
    updateLogDatei();
    if (fall1 == 3)
    {
        turn1(motor_left, 1, false, 4, "degree", 90, 1, true);
        //turn2(1, 5, "degree", 90, 1, true);
    }
    else
    {
        turn1(motor_right, 1, false, 4, "degree", 90, 1, true);
        //turn2(1, 5, "degree", -90, 1, true);
    }
    //line2(1, 3, pGL2, dGL2, "degree", 50, 3, false);
    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
    line2(3, 3, pGL2, dGL2, "degree", miniDistance, 1, true);
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
            moveStraight(-1, -3, "degree", 128, -1, true);
            mediumMotor(longMotor, 50, "degree", 242, true); //up
            if (i == 0)
            {
                mediumMotor(doubleLever, 90, "degree", leverDistance, true); //up
                moveStraight(-1, -40, "degree", 260, -1, true);              //bis kurz vor Node
                mediumMotor(longMotor, -50, "degree", 315, true);            //down
                moveStraight(1, 50, "degree", 190, 1, true);                 //evt. Linefollow!!
                mediumMotor(longMotor, 50, "degree", 315, true);             //up//node von unten nach oben getauscht
                moveStraight(1, 4, "degree", 70, 1, true);
            }
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 4, pGL2, dGL2, "degree", 92, 1, true);
            //line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            //moveStraight(50, 50, "degree", miniDistance, 1, true);
            if (i == 0)
            {
                //turn2(1, 5, "degree", 90 * (-x), 1, true);
                turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true); //neu
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                //turn2(1, 5, "degree", 90, 1, true);
                turn1(motor_left, 1, false, 4, "degree", 90, 1, true); //neu
                if (currentColor == 1)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                }
                else
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                }
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", -90, 1, true);
            }
        }
        else
        {
            moveStraight(1, 3, "degree", miniDistance, 1, true); // da er sonst auf der Linie anfängt
            for (int i = 0; i < 2; i++)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            routerAbladen(0); //node absetzen
            moveStraight(-1, -3, "degree", 128, -1, true);
            mediumMotor(longMotor, 50, "degree", 242, true); //Up
            turn2(1, 5, "degree", 90 * x, 1, true);
            if (i == 0)
            {
                line2(1, 3, pGL2, dGL2, "degree", 230, 1, true);
                mediumMotor(doubleLever, 100, "degree", leverDistance, true); //up
                moveStraight(-1, -40, "degree", 260, -1, true);               //bis kurz vor Node
                mediumMotor(longMotor, -50, "degree", 315, true);             //down
                moveStraight(1, 40, "degree", 210, 1, true);                  //evt. Linefollow!!
                mediumMotor(longMotor, 50, "degree", 315, true);              //up//node von unten nach oben getauscht
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * (-x), 1, true);

                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 3, "degree", miniDistance, 1, true);
                if (currentColor == 2)
                {
                    turn2(1, 5, "degree", 90, 1, true);
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    moveStraight(50, 50, "degree", miniDistance, 1, true);
                    turn2(1, 5, "degree", -90, 1, true);
                }
            }
            else
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
            }
        }
    }
}

void task2()
{
    std::cout << "task2:" << std::endl;
    updateLogDatei();

    //Runde 2 - fall2: 1-10________________________________________________________________________________________________
    if (fall2 == 1 || fall2 == 4 || fall2 == 10) //Fall 1 und 4 und 10-----------------------------------------------------
    {
        if (fall2 == 1 || fall2 == 4)
        {
            if (routerW[2] == 0)
            {
                currentPosition = 2;
                routerW[2] = 3;
                std::cout << "Neuer Wert routerW 2: " << routerW[2] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
            }
            else if (routerW[1] == 0)
            {
                ev3_speaker_play_tone(NOTE_E4, 500);
                std::cout << "juhhhhhhhhhhhhhhhuuuuuuu " << std::endl;
                currentPosition = 1;
                routerW[1] = 3;
                std::cout << "Neuer Wert routerW 1: " << routerW[1] << std::endl;
                line2(1, 3, pGL2, dGL2, "degree", 60, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", 60, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else
            {
                currentPosition = 0;
                routerW[0] = 3;
                std::cout << "Neuer Wert routerW 0: " << routerW[0] << std::endl;
                line2(1, 3, pGL2, dGL2, "degree", 60, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", 60, 3, false);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", 60, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            routerAufnehmen(motor_right); //auf Cross vor Nodes (Westen)

            if ((currentPosition == 1 && routerO[1] == 3) || (currentPosition == 2 && routerO[2] == 3) || (currentPosition == 0 && routerO[0] == 3))
            {
                turn2(1, 5, "degree", 180, 1, true);
            }
            else if ((currentPosition == 1 && routerO[2] == 3) || (currentPosition == 0 && routerO[1] == 3) || (currentPosition == 1 && routerO[0] == 3) || (currentPosition == 2 && routerO[1] == 3))
            {
                int x = 1;
                if ((currentPosition == 1 && routerO[0] == 3) || (currentPosition == 2 && routerO[1] == 3))
                {
                    ev3_speaker_play_tone(NOTE_E4, 500);
                    x = -1;
                    currentPosition--;
                }
                else
                {
                    currentPosition++;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            else
            {
                int x = 1;
                if ((currentPosition == 2 && routerO[0] == 3) || (currentPosition == 0 && routerO[2] == 3))
                {
                    x = -1;
                    currentPosition = 0;
                }
                else
                {
                    currentPosition = 2;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            routerO[currentPosition] = 2;
            std::cout << "Neuer Wert routerO " << currentPosition << ": " << routerO[currentPosition] << std::endl;
            line2(1, 4, pGL2, dGL2, "degree", 166, 1, true);  //Node zu Wechsel bringen
            mediumMotor(longMotor, -50, "degree", 315, true); //runter
            moveStraight(-1, -3, "degree", 166, 1, true);
            mediumMotor(longMotor, 50, "degree", 315, true); //hoch
            if (fall2 == 4)
            {
                if (currentPosition == 0 || currentPosition == 2)
                {
                    int x = 1;
                    if (currentPosition == 0)
                    {
                        x = -1;
                    }
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    if (routerO[1] == 0)
                    {
                        currentPosition1 = 1;
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    }
                    else
                    {
                        currentPosition1 = 2;
                        if (x == -1)
                        {
                            currentPosition1 = 0;
                        }
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                        line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    }
                    moveStraight(50, 50, "degree", miniDistance, 1, true);
                    turn2(1, 5, "degree", 90 * (-x), 1, true);
                }
                else
                {
                    int x = 1;
                    currentPosition1 = 0;
                    if (routerO[2] == 0)
                    {
                        x = -1;
                        currentPosition1 = 2;
                    }
                    turn2(1, 5, "degree", 90 * x, 1, true);
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    moveStraight(50, 50, "degree", miniDistance, 1, true);
                    turn2(1, 5, "degree", 90 * (-x), 1, true);
                }
                line2(1, 4, pGL2, dGL2, "degree", 350, 1, true);
                mediumMotor(longMotor, -50, "degree", 315, true);
                line2(1, 4, pGL2, dGL2, "degree", -350, 1, true);
            }
            int x = 1;
            temporalMotor = motor_right;
            secondTemporalMotor = motor_left;
            if ((fall2 == 1 && currentPosition == 2) || (fall2 == 4 && currentPosition1 == 2))
            {
                x = -1;
                temporalMotor = motor_left;
                secondTemporalMotor = motor_right;
            }
            turn2(1, 5, "degree", 90 * x, 1, true);
            if ((fall2 == 1 && currentPosition == 1) || (fall2 == 4 && currentPosition1 == 1))
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 1, true);
            }
            else
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
            }
            turn1(temporalMotor, 1, false, 3, "degree", 90, 1, true);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
            if ((currentPosition == 0 && currentPosition1 == 2) || (currentPosition == 2 && (currentPosition1 == 1 || currentPosition1 == 0)))
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else if (currentPosition == 1)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
        }
        else
        { //fall 10
            turn2(1, 5, "degree", 180, 1, true);
            for (int i = 0; i < 2; i++)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 3, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90, 1, true);
            }
            if (routerO[2] == 0)
            {
                currentPosition = 2;
                routerO[2] = 3;
                std::cout << "Neuer Wert routeO 2: " << routerO[2] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else if (routerO[1] == 0)
            {
                currentPosition = 1;
                routerO[1] = 3;
                std::cout << "Neuer Wert routerO 1: " << routerO[1] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else
            {
                currentPosition = 0;
                routerO[0] = 3;
                std::cout << "Neuer Wert routerO 0: " << routerO[0] << std::endl;
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
        }
        line2(3, 3, pGL2, dGL2, "degree", 180, 1, true); //moveBackDistance
        turn1(secondTemporalMotor, -1, false, -4, "degree", 90, -1, true);//...............
        if (fall2 == 1 || fall2 == 10) //Ausgangsposition long oben
        {
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 205, 1, true);
            mediumMotor(longMotor, -50, "degree", 315, true); //runter
            moveStraight(-1, -3, "degree", 205 - miniDistance, -1, true);
            int x = 1;
            if (currentPosition == 0 || currentPosition == 2)
            {
                if (currentPosition == 2)
                {
                    x = -1;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                if (routerO[1] == 2 || routerO[1] == 0)
                {
                    currentPosition = 1;
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
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
                    line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                }
                line2(3, 3, pGL2, dGL2, "degree", 180, 1, true); //moveBackDistance
                temporalMotor = motor_right;
                if (x == 1)
                {
                    temporalMotor = motor_left;
                }
                turn1(temporalMotor, -1, false, -4, "degree", 90, -1, true);
            }
            else
            {
                currentPosition = 2;
                if (routerO[0] == 2 || routerO[0] == 0)
                {
                    x = -1;
                    currentPosition = 0;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", 180, 1, true); //moveBackDistance
                temporalMotor = motor_right;
                if (currentPosition == 2)
                {
                    temporalMotor = motor_left;
                }
                turn1(temporalMotor, -1, false, -4, "degree", 90, -1, true);
            }
        }

        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(3, 3, pGL2, dGL2, "degree", 50, 1, true);
        mediumMotor(longMotor, 50, "degree", 315, true); //hoch
        moveStraight(1, 4, "degree", 100, 1, true);
        temporalMotor = motor_right;
        entscheidung = 0;
        if ((currentPosition == 1 && routerO[0] == 1) || currentPosition == 2)
        {
            entscheidung = 1;
            temporalMotor = motor_left;
        }
        turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
        if (currentPosition == 1)
        {
            moveStraight(1, 3, "degree", 450, 1, true);
        }
        else
        {
            moveStraight(1, 3, "degree", 100, 1, true);
        }
        turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
        line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
    }
    else if (fall2 == 2 || fall2 == 3) //Fall 2 und 3--------------------------------------------------------------------------
    {
        turn2(1, 5, "degree", 180, 1, true);
        for (int i = 0; i < 2; i++)
        {
            line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
            moveStraight(1, 3, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90, 1, true);
        }
        if (routerO[2] == 2)
        {
            currentPosition = 2;
            routerO[2] = 3;
            std::cout << "Neuer Wert routerO 2: " << routerO[2] << std::endl;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        else if (routerO[1] == 2)
        {
            currentPosition = 1;
            routerO[1] = 3;
            std::cout << "Neuer Wert routerO 1: " << routerO[1] << std::endl;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        else
        {
            currentPosition = 0;
            routerO[0] = 3;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
        line2(3, 3, pGL2, dGL2, "degree", 184, 1, true); //moveBackDistance
        turn1(secondTemporalMotor, -1, false, -4, "degree", 90, -1, true);
        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(3, 3, pGL2, dGL2, "degree", 205, 1, true);
        mediumMotor(longMotor, -50, "degree", 315, true); //runter
        line2(1, 3, pGL2, dGL2, "degree", 385, 1, true);
        if (currentPosition == 2 || currentPosition == 0)
        { //entscheidung zu welchem router westen
            temporalMotor = motor_left;
            if (currentPosition == 2)
            {
                temporalMotor = motor_right;
            }
            if (routerW[2])
            {
                turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
            }
        }
        //hier fehlt noch was !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else if (fall2 == 5 || fall2 == 6 || fall2 == 7) //fall2 == 5 == 6 == 7--------------------------------------------------
    {
        if (routerW[2] == 0)
        {
            currentPosition = 2;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 205, 1, true);
        }
        else if (routerW[1] == 0)
        {
            currentPosition = 1;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 205, 1, true);
        }
        else
        {
            currentPosition = 0;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 205, 1, true);
        }
        turn1(motor_right, -1, false, -4, "degree", 90, -1, true);
        line2(1, 3, pGL2, dGL2, "degree", 295, 1, true);
        mediumMotor(longMotor, -50, "degree", 315, true); //down
        moveStraight(-1, -3, "crossline", 0, -1, true);
        moveStraight(1, 3, "degree", miniDistance, 1, true);
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
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            else
            {
                int x = 1;
                if ((currentPosition == 2 && routerO[0] == 0) || (currentPosition == 0 && routerO[2] == 0))
                {
                    x = -1;
                    currentPosition = 0;
                }
                else
                {
                    currentPosition = 2;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                line2(3, 3, pGL2, dGL2, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            routerO[currentPosition] = 2;
            line2(3, 3, pGL2, dGL2, "degree", 202, 1, true);
            mediumMotor(longMotor, 50, "degree", 315, true); //up
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
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
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            }
            else
            {
                if (currentPosition == 2)
                {
                    x = -1;
                    currentPosition = 0;
                }
                else
                {
                    currentPosition = 2;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            }
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
            line2(1, 3, pGL2, dGL2, "degree", 312, 3, false);
            moveStraight(3, 3, "degree", 220, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
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
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            else
            {
                int x = 1;
                currentPosition = 0;
                temporalMotor = motor_right;
                if (currentPosition == 0)
                {
                    x = -1;
                    currentPosition = 2;
                    temporalMotor = motor_left;
                }
                turn2(1, 5, "degree", 90 * x, 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
            }
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 165, 1, true);
            turn1(temporalMotor, -1, false, -4, "degree", 90, -1, true);
            line2(1, 3, pGL2, dGL2, "degree", 275, 1, true);
            mediumMotor(longMotor, 50, "degree", 315, true); //up
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
        }
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            entscheidung = 0;
            if (currentPosition == 2)
            {
                entscheidung = 1;
                x = -1;
            }
            turn2(1, 5, "degree", 90 * x, 1, true);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
        }
        else
        {
            entscheidung = 0;
            turn2(1, 5, "degree", 90, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", -90, 1, true);
        }
    }
    else if (fall2 == 8 || fall2 == 9) //fall2 == 8 == 9 --------------------------------
    {
        if (routerO[2] == 0)
        {
            currentPosition = 2;
            routerW[2] = 3;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        else if (routerO[1] == 0)
        {
            currentPosition = 1;
            routerW[1] = 3;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 40, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
        }
        else
        {
            currentPosition = 0;
            routerO[0] = 3;
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 40, 3, false);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            line2(3, 3, pGL2, dGL2, "degree", 40, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
        }
        moveStraight(50, 50, "degree", miniDistance, 1, true);
        turn2(1, 5, "degree", -90, 1, true);
        line2(1, 3, pGL2, dGL2, "degree", 312, 3, false);
        moveStraight(3, 3, "degree", 128, 3, false);
        line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
        moveStraight(50, 50, "degree", miniDistance, 1, true);
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
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        else
        {
            int x = 1;
            currentPosition = 0;
            temporalMotor = motor_right;
            if (currentPosition == 0)
            {
                x = -1;
                currentPosition = 2;
                temporalMotor = motor_left;
            }
            turn2(1, 5, "degree", 90 * x, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
        }
        moveStraight(3, 3, "degree", miniDistance, 3, false);
        line2(3, 3, pGL2, dGL2, "degree", 165, 1, true);
        turn1(temporalMotor, -1, false, -4, "degree", 90, -1, true);
        mediumMotor(longMotor, -50, "degree", 315, true); //down
        line2(1, 3, pGL2, dGL2, "degree", 275, 1, true);
        mediumMotor(longMotor, 50, "degree", 315, true); //up
        turn2(1, 5, "degree", 180, 1, true);
        line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
        moveStraight(50, 50, "degree", miniDistance, 1, true);
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            entscheidung = 0;
            if (currentPosition == 2)
            {
                entscheidung = 1;
                x = -1;
            }
            turn2(1, 5, "degree", 90 * x, 1, true);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", 90 * (-x), 1, true);
        }
        else
        {
            entscheidung = 0;
            turn2(1, 5, "degree", 90, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            turn2(1, 5, "degree", -90, 1, true);
        }
    }
}

void welcherWeg()
{
    std::cout << "welcherWeg entscheidung: " << entscheidung << std::endl;
    //zum wegbringen fahren _________________________________________________________________________________________________________________________________________________
    int a = 1;
    bool lineEdge = false;
    temporalMotor = motor_left;
    temporalSensor = LSl;
    if (entscheidung == 0)
    {
        a = -1;
        temporalMotor = motor_right;
        temporalSensor = LSr;
        lineEdge = true;
    }
    // zum schwabbelabsetzen fahren
    line1(1, 4, pGL1, dGL1, temporalSensor, lineEdge, "degree", 1380, 4, false);
    line1(4, 4, pGL1, dGL1, temporalSensor, lineEdge, "crossline", 0, 4, false);
    line1(4, 4, pGL1, dGL1, temporalSensor, lineEdge, "degree", 520, 1, true);
    turn1(temporalMotor, 1, false, 4, "degree", 90, 1, true);
    line1(1, 4, pGL1, dGL1, temporalSensor, lineEdge, "degree", 520, 4, false);
    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
    moveStraight(50, 50, "degree", miniDistance, 1, true);
    turn2(1, 5, "degree", 90 * a, 1, true);
}

void wegbringen2()
{
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
    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
    line2(3, 3, pGL2, dGL2, "degree", miniDistance, 1, true);
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
            moveStraight(-1, -3, "degree", 128, -1, true);
            mediumMotor(longMotor, 50, "degree", 242, true); //up
            if (i == 0)
            {
                mediumMotor(doubleLever, 90, "degree", leverDistance, true); //up
                moveStraight(-1, -40, "degree", 260, -1, true);              //bis kurz vor Node
                mediumMotor(longMotor, -50, "degree", 315, true);            //down
                moveStraight(1, 50, "degree", 190, 1, true);                 //evt. Linefollow!!
                mediumMotor(longMotor, 50, "degree", 315, true);             //up//node von unten nach oben getauscht
            }
            turn2(1, 5, "degree", 180, 1, true);
            line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
            moveStraight(50, 50, "degree", miniDistance, 1, true);
            if (i == 0)
            {
                turn2(1, 5, "degree", 90 * (-x), 1, true);
                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                turn2(1, 5, "degree", 90, 1, true);
                if (currentColor == 1)
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                }
                else
                {
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                }
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", -90, 1, true);
            }
        }
        else
        {
            moveStraight(1, 3, "degree", miniDistance, 1, true); // da er sonst auf der Linie anfängt
            for (int i = 0; i < 2; i++)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * x, 1, true);
            }
            routerAbladen(0); //node absetzen
            moveStraight(-1, -3, "degree", 128, -1, true);
            mediumMotor(longMotor, 50, "degree", 242, true); //Up
            turn2(1, 5, "degree", 90 * x, 1, true);
            if (i == 0)
            {
                line2(1, 3, pGL2, dGL2, "degree", 230, 1, true);
                mediumMotor(doubleLever, 100, "degree", leverDistance, true); //up
                moveStraight(-1, -40, "degree", 260, -1, true);               //bis kurz vor Node
                mediumMotor(longMotor, -50, "degree", 315, true);             //down
                moveStraight(1, 40, "degree", 210, 1, true);                  //evt. Linefollow!!
                mediumMotor(longMotor, 50, "degree", 315, true);              //up//node von unten nach oben getauscht
                line2(1, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
                turn2(1, 5, "degree", 90 * (-x), 1, true);

                line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                moveStraight(50, 50, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
                moveStraight(1, 3, "degree", miniDistance, 1, true);
                if (currentColor == 2)
                {
                    turn2(1, 5, "degree", 90, 1, true);
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, pGL2, dGL2, "crossline", 0, 50, false);
                    moveStraight(50, 50, "degree", miniDistance, 1, true);
                    turn2(1, 5, "degree", -90, 1, true);
                }
            }
            else
            {
                line2(1, 3, pGL2, dGL2, "crossline", 0, 1, true);
            }
        }
    }
}

void main_task(intptr_t unused)
{
    //Log-Datei initialisieren
    std::ofstream out("AA_LOG_.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf());                //redirect std::cout to out.txt!
    start();
    //Stopwatch run;

    //test();
    //return;
    
    //Anfang - Scannt Positionen, scannt Router und sammelt das südliche Kabel
    resetMotors();
    moveStraight(30, 50, "degree", 18, 30, true);
    turn1(motor_left, 1, false, 4, "degree", -90, 1, true);
    moveStraight(20, 3, "degree", 50, 3, false);
    positionenScannen();
    turn1(motor_right, 1, false, 4, "degree", -90, 1, true);
    line1(cSpeed, 4, pGL1, dGL1, LSr, true, "degree", 2296, 4, false);
    line1(cSpeed, 70, 0.3, 5, LSr, true, "crossline", 0, 70, false);
    moveStraight(70, 70, "degree", 58, 1, true);
    turn2(1, 60, "degree", -90, 1, true);
    routerScannen(HTr, "routerO");
    turn2(1, 5, "degree", 90, 1, true);
    line2(1, 4, pGL2, dGL2, "degree", 300, 4, false);
    kabelSammeln(true);
    routerScannen(HTl, "routerW");
    

    //Fake
    /*routerO[0] = 1;
    routerO[1] = 0;
    routerO[2] = 0;
    routerW[0] = 1;
    routerW[1] = 0;
    routerW[2] = 0;
    blue = 2;
    red = 3;
    green = 4;
    yellow = 1;
    */

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
    mediumMotor(longMotor, -100, "degree", 315, true); //down
    moveStraight(1, 100, "degree", 320, 100, false);
    turn1(motor_left, 100, true, 4, "degree", 50, 100, false);
    moveStraight(100, 100, "time", 1000, 1, true);

    //Ende
    int neededTime = run.getTime();
    std::cout << "Needed time: " << neededTime << std::endl;
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
