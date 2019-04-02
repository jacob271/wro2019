
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

//Speeds
double cSpeed = 0;

// Variables for Moves
double pGain = 0.02; //0.017
double pGainTurn2 = 1.5;
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.2;  // 0.1   6
const double bfTurn1 = 0.5; // 9
const double bfTurn2 = 0.4; // 0.3
const double afMove = 0.25;
const double afLine1 = 0.18;

// Constants for vertical line alignment
const double pi = 3.14159265358979324;
const double wheelDiameter = 6.24; //6.24;
const double wheelCircumferance = 17.6;
//const double wheelConverter = 6.24 / wheelDiameter;

int positions[4] = {0};
int routerO[3] = {0};
int routerW[3] = {0};
//int router[5] = {0};
int alignDuration = 300;
int miniDistance = 37;
int leverDistance = 120;

int LSrMiddle = (92 + 9) / 2;

int wallDistanceE = 0;

double batteryFactor = 1;

void align(int duration)
{
    double pCorrection = 0;
    Stopwatch align;

    while (align.getTime() < duration)
    {
        pGain = 1.5;
        pCorrection = ev3_color_sensor_get_reflect(LSr) - ev3_color_sensor_get_reflect(LSl);
        ev3_motor_set_power(motor_left, (-1) * (cSpeed - pCorrection * pGain));
        ev3_motor_set_power(motor_right, cSpeed + pCorrection * pGain);
    }
    brake(true, 0);
}

void align1(int duration)
{
    double pCorrection = 0;
    Stopwatch align;

    while (align.getTime() < duration)
    {
        pGain = 1.5;
        pCorrection = 50 - ev3_color_sensor_get_reflect(LSr);
        ev3_motor_set_power(motor_left, (-1) * (cSpeed - pCorrection * pGain));
        ev3_motor_set_power(motor_right, cSpeed + pCorrection * pGain);
    }
    brake(true, 0);
}

void routerAbladen(int vonWo) //1 von Osten oder Westen; 0 von Norden oder Süden
{
    resetMotors();
    if (vonWo == 1)
    {
        line2(35, 40, 0.7, 0.0, "degree", 405, 0, true);
    }
    else
    {
        line2(35, 40, 0.7, 0.0, "degree", 128, 0, true);
    }
    mediumMotor(longMotor, -30, "degree", 242, true);
}

void kabelSammeln()
{
    resetMotors();
    line2(35, 40, 0.7, 0.0, "degree", 277, 0, true);
    mediumMotor(doubleLever, 90, "degree", leverDistance, true);
}

void kabelAbladen()
{
    resetMotors();
    line2(35, 40, 0.7, 0.0, "degree", 266, 40, false);
    resetMotors();
    moveStraight(cSpeed, 40, "degree", 183, 20, true);
    tslp_tsk(300);
    mediumMotor(doubleLever, -50, "degree", leverDistance, true);
    moveStraight(-10, -40, "degree", 400, 20, true);
}

void positionenScannen()
{
    line1(1, 40, 0.5, 0.0, LSr, false, "degree", 315, 40, false); //810
    for (int i = 0; i < 3; i++)
    {
        positions[i] = line1(50, 50, 0.5, 0.0, LSr, false, "degree", 147, 60, false, true, HTr, "color");
        display(positions[1]);
    }
    positions[3] = findColor(positions, "positions");
    line1(50, 50, 0.5, 0.0, LSr, false, "degree", 425, 1, true); //810
}

void routerScannen(sensor_port_t searchSensor, std::string mode)
{
    int router[3] = {0};
    int i = 0;
    line2(40, 40, 0.1, 0.18, "degree", 91, 40, false);
    router[i] = line2(40, 40, 0.7, 0.0, "degree", 183, 40, false, true, searchSensor, "bw");
    i++;
    line2(40, 40, 0.7, 0.0, "degree", 165, 40, false);
    router[i] = line2(40, 40, 0.7, 0.0, "degree", 183, 1, true, true, searchSensor, "bw");
    i++;
    router[i] = findColor(router, "router");
    if (mode == "routerO")
    {
        for (int x = 0; x < 3; x++)
        {
            routerO[x] = router[x];
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

void routerAufnehmen(motor_port_t turnMotor) // mit welchem Rad zurück
{
    //Position anfang: 90° versetzt vor node ohne minimove
    line2(1, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", 184, 1, true);
    turn1(turnMotor, 1, false, -5, "degree", 90, 1, true);
    align(alignDuration);
    mediumMotor(longMotor, 100, "degree", -315, true);
    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", 92, 1, true);
    mediumMotor(longMotor, 100, "degree", 315, true);
    moveStraight(1, -6, "degree", 92 - miniDistance, 1, true);
    //Position Ende: auf Cross vor Node
}

void test()
{
   /* while(abs(measureMotorRight()) < 10000){
        ev3_motor_set_power(motor_left, -100);
    ev3_motor_set_power(motor_right, 100);
        std::cout << "R: "  << abs(measureMotorRight()) << " " << "L: "<< abs(measureMotorLeft()) << std::endl;

    }
return;
*/
    //align(alignDuration);
    line1(20, 100, 0.5, 10, LSr, true, "degree", 2500, 100, true);
    waitForButton();
    line1(15, 100, 1, 25, LSr, true, "degree", 2500, 80, true);
return;
}

void anfang()
{
    moveStraight(20, 30, "degree", 18, 20, true);
    turn1(motor_left, 1, false, 3, "degree", 90, 1, true);
    moveStraight(20, 3, "degree", 55, 20, true);
    align1(200);
    positionenScannen();
    turn1(motor_right, 1, false, 4, "degree", -90, 1, true);
    align(alignDuration);
    line2(1, 40, 0.1, 0.18, "degree", 91, 40, false);
    line1(40, 40, 0.1, 0.18, LSr, true, "degree", 128, 40, false);
    line2(40, 3, 0.1, 0.18, "degree", 550, 3, false);
    line1(3, 3, 0.1, 0.18, LSr, true, "degree", 128, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", 367, 3, false);
    line1(3, 3, 0.1, 0.18, LSr, true, "degree", 128, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", 360, 3, false);
    //ev3_speaker_play_tone(NOTE_E4, 100);
    line1(3, 3, 0.1, 0.18, LSr, true, "blackleft", 128, 3, false);
    line1(3, 3, 0.1, 0.18, LSr, true, "degree", 70, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", 500, 1, true);
    ev3_speaker_play_tone(NOTE_E4, 200);
    waitForButton();
    turn1(motor_right, 1, false, 3, "degree", -90, 1, true);
    align(alignDuration);
    routerScannen(HTr, "routerO");

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

    //routerO[] = routerScannen(LSr);
    //routerW[] = routerScannen(LSl);
}

void main_task(intptr_t unused)
{
    Stopwatch run;
    std::ofstream out("AA_LOG_.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf());                //redirect std::cout to out.txt!*/
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    ev3_motor_config(motor_left, UNREGULATED_MOTOR);
    ev3_motor_config(motor_right, UNREGULATED_MOTOR);
    ev3_motor_config(longMotor, UNREGULATED_MOTOR);
    ev3_motor_config(doubleLever, UNREGULATED_MOTOR);
    ev3_sensor_config(HTl, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(HTr, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSl, COLOR_SENSOR);
    ev3_sensor_config(LSr, COLOR_SENSOR);
    ev3_speaker_play_tone(NOTE_C4, 500);
    ev3_speaker_play_tone(NOTE_E4, 500);
    tslp_tsk(500);

    std::cout << "Battery at: " << ev3_battery_voltage_mV() << "Volt" << std::endl;

    display(ev3_battery_voltage_mV());
    waitForButton();
    test();
    std::cout.rdbuf(coutbuf);
    return;
    //Paul's Scheiße
    //arrays Nodes: 0 schwarz; 1 weiß; 2 getauscht; 3 leer

    int turnBackDistance = 220; //distance um über querlinie rüber zu fahren un dann mit rechts/links zurückzudrehen
    motor_port_t temporalMotor;
    motor_port_t secondTemporalMotor;

    resetMotors();
    anfang();

    int blue = 2; //je nach kombie
    int red = 1;
    int green = 3;
    int yellow = 4;

    int fall1 = 0;
    //fallunterscheidung

    // fall 1-4
    if (fall1 == 1 || fall1 == 2 || fall1 == 3)
    {
        int currentPosition;

        if (routerW[1] == routerO[1])
        {
            moveStraight(-1, -6, "crossline", 0, -1, true);
            line2(1, 3, 0.1, 0.18, "degree", 205, 1, true);
            currentPosition = 1;
            routerW[1] = 3;
            routerO[1] = 3;
        }
        else if (routerW[2] == routerO[2])
        {
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(1, 3, 0.1, 0.18, "degree", 205, 1, true);
            currentPosition = 2;
            routerW[2] = 3;
            routerO[2] = 3;
        }
        else
        {
            moveStraight(-1, -3, "crossline", 0, -3, false);
            moveStraight(-3, -3, "degree", 133, 1, true);
            currentPosition = 0;
            routerW[0] = 3;
            routerO[0] = 3;
        }
        turn1(motor_left, -1, false, -5, "degree", 90, -1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "degree", 295, 1, true);
        mediumMotor(longMotor, 100, "degree", -315, true); //down
        turn2(1, 6, "degree", 180, 1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 240, 1, true);
        mediumMotor(longMotor, 100, "degree", 315, true); //up

        if (fall1 == 1)
        {
            line2(1, 3, 0.1, 0.18, "degree", 132, 1, true);
            moveStraight(-1, -6, "crossline", 0, -1, true);
            moveStraight(1, 6, "degree", miniDistance, 1, true);
            if (currentPosition == 0)
            {
                routerO[0] = 2;
                turn2(1, 6, "degree", -90, 1, true);
                align(alignDuration);
                if (routerO[1] == 0)
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
                    currentPosition == 1;
                }
                else
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                    currentPosition == 2;
                }
                moveStraight(1, 6, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", 90, 1, true);
            }
            else if (currentPosition == 1)
            {
                routerO[1] = 2;
                int x = 1;
                currentPosition == 0;
                if (routerO[2] == 0)
                {
                    currentPosition == 2;
                    x = -1;
                }
                turn2(1, 6, "degree", 90 * x, 1, true);
                align(alignDuration);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", 90 * (-x), 1, true);
            }
            else // currentPosition == 2
            {
                routerO[2] = 2;
                turn2(1, 6, "degree", 90, 1, true);
                if (routerO[1] == 0)
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    currentPosition == 1;
                }
                else
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                    currentPosition == 0;
                }
                moveStraight(3, 6, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", -90, 1, true);
            }
            align(alignDuration);
            line2(1, 3, 0.1, 0.18, "degree", 240, 1, true);
        }

        moveStraight(1, 3, "degree", 125, 3, false);
        line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
        moveStraight(3, 3, "degree", miniDistance, 1, true);
        if (currentPosition == 0 || currentPosition == 2)
        {
            int x = 1;
            if (currentPosition == 2)
            {
                x = -1;
            }
            turn2(1, 6, "degree", 90 * (-x), 1, true);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            turn2(1, 6, "degree", 90 * x, 1, true);
        }
    }
    else
    { //fall4
        if (routerW[1] == 1 || (routerW[0] == 1))
        {
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(1, 3, 0.1, 0.18, "degree", 205, 1, true); //um zu 2 zu gehen
        }
        else
        {
            moveStraight(-1, -6, "crossline", 0, -1, true);
            line2(1, 3, 0.1, 0.18, "degree", 205, 1, true); //um zu 1 zu gehen
        }
        turn1(motor_left, -1, false, -5, "degree", 90, -1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 202, 1, true);
        mediumMotor(longMotor, 100, "degree", -315, true); //down
        moveStraight(-1, -6, "crossline", 0, -1, true);
        moveStraight(1, 3, "degree", miniDistance, 1, true);
        turn2(1, 6, "degree", -90, 1, true);
        if (routerW[1] == 1)
        {
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false); // um von 2 zu 0 zu gehen
        }
        else
        {                                                     // routerW[2] == 0
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false); // um von 1 zu 0 zu gehen oder von 2 zu 1
        }
        moveStraight(3, 3, "degree", miniDistance, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 184, 1, true); //moveBackDistance
        turn1(motor_right, -1, false, -5, "degree", 90, -1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", miniDistance, 1, true); //bis Node eingesammelt ist
        mediumMotor(longMotor, 100, "degree", 315, true);        //up
        if (routerW[0] == 1)
        {
            turn1(motor_left, 1, false, 5, "degree", 180, 1, true);
            turn1(motor_right, 1, false, 5, "degree", 90, 1, true);
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 3, false);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
            for (int i = 0; i < 2; i++)
            {
                turn2(1, 6, "degree", 90, 1, true);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
            }
            turn2(1, 6, "degree", -90, 1, true); //bevor Schwabell absetzen
        }
        else
        {
            turn1(motor_left, 1, false, 5, "degree", 90, 1, true);
            moveStraight(1, 6, "degree", 120, 1, true);
            turn1(motor_left, 1, false, 5, "degree", 90, 1, true);
            moveStraight(1, 6, "degree", 40, 1, true);
            align(alignDuration);
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            turn2(1, 6, "degree", -90, 1, true);
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            turn2(1, 6, "degree", 90, 1, true); //bevor Schwabell absetzen
        }
    }
    line2(1, 3, 0.1, 0.18, "degree", 459, 1, true);
    moveStraight(1, 6, "degree", 174, 1, true);
    mediumMotor(doubleLever, -50, "degree", leverDistance, true); //down
    moveStraight(-1, -6, "degree", 120, -1, true);
    turn2(1, 6, "degree", 180, 1, true);
    line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
    moveStraight(1, 6, "degree", miniDistance, 1, true);
    if (fall1 == 3)
    {
        turn2(1, 6, "degree", 90, 1, true);
    }
    else
    {
        turn2(1, 6, "degree", -90, 1, true);
    }
    align(alignDuration);
    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
    line2(3, 3, 0.1, 0.18, "degree", miniDistance, 1, true);
    int currentColor; // 1 = red, 2 = blue
    for (int i = 0; i < 2; i++)
    {
        int x; //Variable für Richtung der Drehungen
        if ((fall1 == 3 && i == 0) || ((fall1 == 1 || fall1 == 2 || fall1 == 4) && i == 1))
        {
            currentColor = 1;
            x = 1;
        }
        else
        {
            currentColor = 2;
            x = -1;
        }

        if ((currentColor == 1 && (red == 1 || red == 3)) || (currentColor == 2 && (blue == 1 || blue == 3)))
        {
            turn2(1, 6, "degree", 90 * x, 1, true);
            routerAbladen(1); //node absetzen
            moveStraight(-1, -6, "degree", 128, -1, true);
            if (i == 0)
            {
                mediumMotor(doubleLever, 90, "degree", leverDistance, true); //up
                mediumMotor(longMotor, 100, "degree", 242, true);            //up
                moveStraight(-1, -6, "degree", 257, -1, true);               //bis kurz vor Node
                mediumMotor(longMotor, 100, "degree", -315, true);           //down
                moveStraight(1, 6, "degree", 184, 1, true);                  //evt. Linefollow!!
                mediumMotor(longMotor, 100, "degree", 315, true);            //up//node von unten nach oben getauscht
            }
            turn2(1, 6, "degree", 180, 1, true);
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            moveStraight(3, 3, "degree", miniDistance, 1, true);
            if (i == 0)
            {
                turn2(1, 6, "degree", 90 * (-x), 1, true);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                turn2(1, 6, "degree", 90, 1, true);
                if (currentColor == 1)
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                }
                else
                {
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                }
                moveStraight(3, 3, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", 90, 1, true);
            }
        }
        else
        {
            moveStraight(1, 6, "degree", miniDistance, 1, true); // da er sonst auf der Linie anfängt
            for (int i = 0; i < 2; i++)
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", 90 * x, 1, true);
            }
            routerAbladen(0); //node absetzen
            moveStraight(-1, -6, "degree", 128, -1, true);
            mediumMotor(longMotor, 30, "degree", 242, true); //Up
            turn2(1, 6, "degree", 90 * x, 1, true);
            align(alignDuration);
            line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
            if (i == 0)
            {
                mediumMotor(doubleLever, 50, "degree", leverDistance, true); //up
                moveStraight(-1, -6, "degree", 257, -1, true);               //bis kurz vor Node
                mediumMotor(longMotor, 100, "degree", -315, true);           //down
                moveStraight(1, 6, "degree", 184, 1, true);                  //evt. Linefollow!!
                mediumMotor(longMotor, 100, "degree", 315, true);            //down//node von unten nach oben getauscht
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
                turn2(1, 6, "degree", 90 * (-x), 1, true);

                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                moveStraight(3, 3, "degree", miniDistance, 1, true);
            }
            else if (i == 1)
            {
                moveStraight(1, 3, "degree", miniDistance, 1, true);
                if (currentColor == 2)
                {
                    turn2(1, 6, "degree", 90, 1, true);
                    align(alignDuration);
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 1, true);
                    turn2(1, 6, "degree", -90, 1, true);
                }
            }
        }
    }
    line2(1, 3, 0.1, 0.18, "degree", 550, 1, true);
    turn1(motor_right, 1, false, 5, "degree", 50, 1, true);       //evtl. größerer winkel
    turn1(motor_left, 1, false, 5, "degree", 50, 1, true);        //evtl. größerer winkel
    mediumMotor(doubleLever, -90, "degree", leverDistance, true); //schwabell down
    line2(1, 3, 0.1, 0.18, "degree", 165, 1, true);
    mediumMotor(doubleLever, 90, "degree", leverDistance, true); //schwabell up
    moveStraight(-1, -6, "crossline", 0, -1, true);
    moveStraight(1, 6, "degree", miniDistance, 1, true);
    turn2(1, 6, "degree", -90, 1, true);

    int fall2;
    //fallunterscheidung 1-10_____________________________________________________________________________________________________________________________________________

    int entscheidung; //ob oben oder unten rum zum wegbringen // 1 == obenrum, 0 == untenrum
    int currentPosition;
    int currentPosition1 = 5;

    if (fall2 == 1 || fall2 == 4 || fall2 == 10) //Fall 1 und 4 und 10
    {
        if (fall2 == 1 || fall2 == 4)
        {

            if (routerW[2] == 0)
            {
                currentPosition = 2;
                routerW[2] == 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
                routerAufnehmen(motor_right);
            }
            else if (routerW[1] == 0)
            {
                currentPosition = 1;
                routerW[1] == 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", 40, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                routerAufnehmen(motor_right);
            }
            else
            {
                currentPosition = 0;
                routerW[0] == 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", 40, 3, false);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", 40, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                routerAufnehmen(motor_right);
            }
            //auf Cross vor Nodes (Westen)
            if ((currentPosition == 1 && routerO[1] == 3) || (currentPosition == 2 && routerO[2] == 3) || (currentPosition == 3 && routerO[3] == 3))
            {
                turn2(1, 4, "degree", 180, 1, true);
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
                turn2(1, 4, "degree", 90 * x, 1, true);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 4, "degree", 90 * x, 1, true);
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
                turn2(1, 4, "degree", 90 * x, 1, true);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                moveStraight(1, 2, "degree", miniDistance, 1, true);
                turn2(1, 4, "degree", 90 * x, 1, true);
            }
            routerO[currentPosition] == 2;
            line2(1, 4, 0.1, 0.18, "degree", 166, 9, true);    //Node zu Wechsel bringen
            mediumMotor(longMotor, 100, "degree", -315, true); //runter
            moveStraight(-1, -6, "degree", 166, 9, true);
            mediumMotor(longMotor, 100, "degree", 315, true); //hoch
            if (fall2 == 4)
            {
                if (currentPosition == 0 || currentPosition == 2)
                {
                    int x = 1;
                    if (currentPosition == 0)
                    {
                        x = -1;
                    }
                    turn2(1, 4, "degree", 90 * x, 1, true);
                    if (routerO[1] == 0)
                    {
                        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                        currentPosition1 = 1;
                    }
                    else
                    {
                        currentPosition1 = 2;
                        if (x == -1)
                        {
                            currentPosition1 = 0;
                        }
                        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                        line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                        line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    }
                    moveStraight(3, 3, "degree", miniDistance, 1, true);
                    turn2(1, 4, "degree", 90 * (-x), 1, true);
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
                    turn2(1, 4, "degree", 90 * x, 1, true);
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    moveStraight(3, 3, "degree", miniDistance, 1, true);
                    turn2(1, 4, "degree", 90 * (-x), 1, true);
                }
                align(alignDuration);
                line2(1, 4, 0.1, 0.18, "degree", 350, 1, true);
                mediumMotor(longMotor, 100, "degree", -315, true);
                line2(1, 4, 0.1, 0.18, "degree", -350, 1, true);
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

            turn2(1, 4, "degree", 90 * x, 1, true);
            if ((fall2 == 1 && currentPosition == 1) || (fall2 == 4 && currentPosition1 == 1))
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            else
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            turn1(temporalMotor, 3, true, 3, "degree", 90, 1, true);
            align(alignDuration);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 1, false);
            moveStraight(1, 3, "degree", miniDistance, 1, true);
            turn2(1, 4, "degree", 90 * (-x), 1, true);
            align(alignDuration);
            if ((currentPosition == 0 && currentPosition1 == 2) || (currentPosition == 2 && (currentPosition1 == 1 || currentPosition1 == 0)))
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            else if (currentPosition == 1)
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            else
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
        }
        else
        { //fall 10
            turn2(1, 4, "degree", 180, 1, true);
            for (int i = 0; i < 2; i++)
            {
                line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
                moveStraight(1, 3, "degree", miniDistance, 1, true);
                turn2(1, 4, "degree", 90, 1, true);
            }
            if (routerO[2] == 0)
            {
                currentPosition = 2;
                routerO[2] = 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            else if (router[1] == 0)
            {
                currentPosition = 1;
                routerO[1] = 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
            else
            {
                currentPosition = 0;
                routerO[0] = 3;
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            }
        }
        line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 184, 1, true); //moveBackDistance
        turn1(secondTemporalMotor, 1, false, -5, "degree", 90, 1, true);
        align(alignDuration);
        if (fall2 == 1) //Ausgangsposition long oben
        {
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(3, 3, 0.1, 0.18, "degree", 205, 1, true);
            mediumMotor(longMotor, 100, "degree", -315, true); //runter
            moveStraight(-1, -6, "degree", 205 - miniDistance, -1, true);
            int x = 1;
            if (currentPosition == 0 || currentPosition == 2)
            {
                if (currentPosition == 2)
                {
                    x = -1;
                }
                turn2(1, 4, "degree", 90 * x, 1, true);
                align(alignDuration);
                if (routerO[1] == 2 || routerO[1] == 0)
                {
                    currentPosition = 1;
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
                }
                else
                {
                    currentPosition = 2;
                    if (currentPosition == 2)
                    {
                        currentPosition = 0;
                    }
                    line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                    line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                    line2(3, 3, 0.1, 0.18, "crossline", 0, 1, true);
                }
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", 184, 1, true); //moveBackDistance
                temporalMotor = motor_right;
                if (x == 1)
                {
                    temporalMotor = motor_left;
                }
                turn1(temporalMotor, 1, false, 5, "degree", 90, 1, true);
            }
            else
            {
                currentPosition = 2;
                if (routerO[0] == 2 || routerO[0] == 0)
                {
                    x = -1;
                    currentPosition = 0;
                }
                turn2(1, 4, "degree", 90 * x, 1, true);
                line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
                line2(3, 3, 0.1, 0.18, "degree", 184, 1, true); //moveBackDistance
                temporalMotor = motor_right;
                if (currentPosition == 2)
                {
                    temporalMotor = motor_left;
                }
                turn1(temporalMotor, 1, false, 5, "degree", 90, 1, true);
            }
        }

        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 35, 1, true);
        mediumMotor(longMotor, 100, "degree", 315, true); //hoch
        temporalMotor = motor_right;
        entscheidung = 0;
        if ((currentPosition == 1 && routerO[0] == 1) || currentPosition == 2)
        {
            entscheidung = 1;
            temporalMotor = motor_left;
        }
        turn1(temporalMotor, 1, false, 5, "degree", 90, 1, true);
        if (currentPosition == 1)
        {
            moveStraight(1, 6, "degree", 460, 1, true);
        }
        else
        {
            moveStraight(1, 6, "degree", 110, 1, true);
        }
        turn1(temporalMotor, 1, false, 5, "degree", 90, 1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
    }                                  // _______________________________________________________________________________________________________
    else if (fall2 == 2 || fall2 == 3) //Fall 2 und 3
    {
        turn2(1, 4, "degree", 180, 1, true);
        for (int i = 0; i < 2; i++)
        {
            line2(1, 3, 0.1, 0.18, "crossline", 0, 1, true);
            moveStraight(1, 3, "degree", miniDistance, 1, true);
            turn2(1, 4, "degree", 90, 1, true);
        }
        if (routerO[2] == 2)
        {
            currentPosition = 2;
            routerO[2] = 3;
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        }
        else if (router[1] == 2)
        {
            currentPosition = 1;
            routerO[1] = 3;
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
        }
        else
        {
            currentPosition = 0;
            routerO[0] = 3;
            line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
            line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
            line2(3, 3, 0.1, 0.18, "crossline", 0, 3, false);
        }
        line2(3, 3, 0.1, 0.18, "degree", miniDistance, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 184, 1, true); //moveBackDistance
        turn1(secondTemporalMotor, 1, false, -5, "degree", 90, 1, true);
        align(alignDuration);
        line2(1, 3, 0.1, 0.18, "crossline", 0, 3, false);
        line2(3, 3, 0.1, 0.18, "degree", 205, 1, true);
        mediumMotor(longMotor, 100, "degree", -315, true); //runter
        line2(1, 3, 0.1, 0.18, "degree", 385, 1, true);
        if (currentPosition == 2 || currentPosition == 0)
        { //entscheidung zu welchem router westen
            temporalMotor = motor_left;
            if (currentPosition == 2)
            {
                temporalMotor = motor_right;
            }
            if (routerW[2])
                turn1(temporalMotor, 1, false, 5, "degree", 90, 1, true);
        }
    }
    else if (fall2 == 5 || fall2 == 6)
    {
    }

    int neededTime = run.getTime();
    std::cout << "Needed time: " << neededTime << std::endl;
    char msgbuf[10];
    sprintf(msgbuf, "Time: %d", neededTime);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    ev3_motor_stop(motor_left, true);
    ev3_motor_stop(motor_right, true);
    ev3_speaker_play_tone(NOTE_E4, 500);
    tslp_tsk(500);
    ev3_motor_stop(motor_left, false);
    ev3_motor_stop(motor_right, false);
    std::cout.rdbuf(coutbuf);
    ev3_speaker_play_tone(NOTE_E4, 100);
}

//git config --global user.email "you@example.com"
//git config --global user.name "Your Name"