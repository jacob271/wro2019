/**
 * This sample program balances a two-wheeled Segway type robot such as Gyroboy in EV3 core set.
 *
 * References:
 * http://www.hitechnic.com/blog/gyro-sensor/htway/
 * http://www.cs.bgu.ac.il/~ami/teaching/Lejos-2013/classes/src/lejos/robotics/navigation/Segoway.java
 */

#pragma once
//#include <thread>
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
motor_port_t tool1 = EV3_PORT_B;
motor_port_t tool2 = EV3_PORT_C;
motor_port_t motor_right = EV3_PORT_D;
sensor_port_t HTl = EV3_PORT_4;
sensor_port_t LSr = EV3_PORT_2;
sensor_port_t LSl = EV3_PORT_3;
sensor_port_t HTr = EV3_PORT_1;

//Speeds
double cSpeed = 0;
const int minSpeed = 25;

// Variables for Moves
double pGain = 0.02; //0.017
double pGainTurn2 = 1.5;
int resetRightDegree = 0;
int resetLeftDegree = 0;
const double bfMove = 0.2;  // 0.1   6
const double bfTurn1 = 0.5; // 9
const double bfTurn2 = 0.4; // 0.3
const double afMove = 4.0;
//const int averageBlueLeft = 1;
//const int averageBlueRight = 1;

// Constants for vertical line alignment
//const double lsDistance = 11.3; //11.7;
const double pi = 3.14159265358979324;
const double wheelDiameter = 6.24; //6.24;
double wheelCircumferance = 17.6;
//const double wheelConverter = 6.24 / wheelDiameter;
//const double wheelLSdist = 3.6 * 2 * (360 / (6.24 * pi));

int positions[3] = {0};
int router[5] = {0};

//double batteryFactor = 1;

void routerAbladen()
{
    resetMotors("total", 0, 0, 0);
    line2(35, 40, 0.7, 0.0, "degree", 211, 0, true, false, HTr, " ");
    mediumMotorDeg(tool1, -30, 242, true);
}

void kabelSammeln()
{
    resetMotors("total", 0, 0, 0);
    line2(35, 40, 0.7, 0.0, "degree", 277, 0, true, false, HTr, " ");
    mediumMotorDeg(tool2, 90, 120, true);
}

void kabelAbladen()
{
    resetMotors("total", 0, 0, 0);
    line2(35, 40, 0.7, 0.0, "degree", 266, 40, false, false, HTr, " ");
    resetMotors("total", 0, 0, 0);
    moveStraight(cSpeed, 40, "degree", 183, 20, true, false, HTr, " ");
    tslp_tsk(300);
    mediumMotorDeg(tool2, -50, 120, true);
    moveStraight(-10, -40, "degree", 400, 20, true, false, HTr, " ");
}

void positionenScannen()
{
    line1(60, 60, 0.9, 0.0, LSr, false, "degree", 330, 60, false, false, HTr, " "); //810
    for (int i = 0; i < 3; i++)
    {
        positions[i] = line1(60, 60, 0.9, 0.0, LSr, false, "degree", 147, 60, false, true, HTr, "color");
        char buffer[10];
        itoa(positions[i], buffer, 10);
        ev3_lcd_draw_string(buffer, 20, 50);
    }
    positions[3] = findColor(positions, "positions");
    line1(60, 60, 0.9, 0.0, LSr, false, "degree", 500, 60, true, false, HTr, " "); //810
}

void routerScannen(std::string mode, sensor_port_t searchSensor)
{
    int i = 0;
    if (mode == "router1")
    {
        i = 0;
    }
    else
    {
        i = 3;
    }
    line2(40, 40, 0.7, 0.0, "degree", 64, 40, false, false, HTr, " ");
    router[i] = line2(40, 40, 0.7, 0.0, "degree", 183, 40, false, true, searchSensor, "bw");
    i++;
    line2(40, 40, 0.7, 0.0, "degree", 165, 40, false, false, searchSensor, "bw");
    router[i] = line2(40, 40, 0.7, 0.0, "degree", 183, 40, true, true, searchSensor, "bw");
    i++;
    router[i] = findColor(router, mode);
}

void routerAufnehmen()
{
    line2(40, 40, 0.7, 0.0, "degree", 300, 40, true, false, HTr, " ");
    mediumMotorDeg(tool1, 60, 315, true);
}

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
}

void test()
{

    line2(0, 70, 0.1, 0.18, "oneLine", 0, 40, false, false, HTr, " ");
    line2(60, 60, 0.1, 0.18, "degree", 585, 0, true, false, HTr, " ");
    resetMotors("total", 0, 0, 0);

    while (abs(ev3_motor_get_counts(motor_left) - resetLeftDegree) < 505)
    {
        ev3_motor_set_power(motor_left, -70);
    }
    align(150);

    // ev3_speaker_play_tone(NOTE_C5, 100);
    //waitForButton();

    //ev3_speaker_play_tone(NOTE_C5, 100);

    //waitForButton();
    resetMotors("total", 0, 0, 0);
    line2(70, 70, 0.1, 0.18, "oneLine", 0, 0, true, false, HTr, " ");
    return;

    Stopwatch move;
    cSpeed = 40.0;

    double pCorrection;
    while (move.getTime() < 10000)
    {
        pCorrection = ev3_color_sensor_get_reflect(LSr) - ev3_color_sensor_get_reflect(LSl);
        char buffer[10];
        itoa(pCorrection, buffer, 10);
        ev3_lcd_draw_string(buffer, 20, 50);
        double speedRight = cSpeed + pCorrection * 2;
        double speedLeft = (cSpeed - pCorrection * 2) * (-1);
        ev3_motor_set_power(motor_left, speedLeft);
        ev3_motor_set_power(motor_right, speedRight);
    }
    ev3_speaker_play_tone(NOTE_C5, 100);
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
    ev3_motor_config(tool1, UNREGULATED_MOTOR);
    ev3_motor_config(tool2, UNREGULATED_MOTOR);
    ev3_sensor_config(HTl, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(HTr, HT_NXT_COLOR_SENSOR);
    ev3_sensor_config(LSl, COLOR_SENSOR);
    ev3_sensor_config(LSr, COLOR_SENSOR);
    ev3_speaker_play_tone(NOTE_C4, 500);
    ev3_speaker_play_tone(NOTE_E4, 500);
    tslp_tsk(500);

    std::cout << "Battery at: " << ev3_battery_voltage_mV() << "Volt" << std::endl;
    char buffer[10];
    itoa(ev3_battery_voltage_mV(), buffer, 10);
    ev3_lcd_draw_string(buffer, 20, 50);
    waitForButton();
    //test();

    //Paul's Scheiße
    //arrays Nodes: 0 schwarz; 1 weiß; 2 getauscht; 3 leer

    anfang();

    int blau == 2; //je nach kombie
    int red == 1;
    int green == 3;
    int yellow == 4;

    int Fall;
    //Fallunterscheidung

    // Fall 1-4
    if (Fall == 1 || Fall == 2 || Fall == 3)
    {
        int currentPosition;

        if (arrayW[1] == arrayO[1])
        {
            lineFollow(short);
            currentPosition = 1;
            arrayW[1] = 3;
            arrayO[1] = 3;
        }
        else if (arrayW[2] == arrayO[2])
        {
            lineFollow(long);
            currentPosition = 2;
            arrayW[2] = 3;
            arrayO[2] = 3;
        }
        else
        {
            move(back);
            currentPosition = 0;
            arrayW[0] = 3;
            arrayO[0] = 3;
        }
        turnLeft(back);
        lineFollow();
        longMotor(down);
        turn2(180°);
        lineFollow(); //bis node osten
        longMotor(up);

        if (Fall == 1)
        {
            move(back);
            if (currentPosition == 0)
            {
                arrayO[0] = 2;
                turnLeft(back) if (arrayO[1] == 0)
                {
                    lineFollow(short);
                    currentPositon == 1;
                }
                else
                {
                    lineFollow(long);
                    currentPositon == 2;
                }
                turnLeft();
            }
            else if (currentPosition == 1)
            {
                arrayO[1] = 2;
                if (arrayO[2] == 0)
                {
                    turnLeft(back);
                    lineFollow();
                    turnLeft();
                    currentPositon == 2;
                }
                else
                {
                    turnRight(back);
                    lineFollow();
                    turnRight();
                    currentPositon == 0;
                }
            }
            else
            {
                arrayO[2] = 2;
                turnRight(back);
                if (arrayO[1] == 0)
                {
                    lineFollow(short);
                    currentPositon == 1;
                }
                else
                {
                    lineFollow(long);
                    currentPositon == 0;
                }
                turnRight();
            }
        }
        lineFollow();
        if (currentPosition == 0)
        {
            turnRight();
            lineFollow();
            turnLeft();
            lineFollow(); //bevor Schwabell absetzen
        }
        else if (currentPosition == 1)
        {
            lineFollow(); //bevor Schwabell absetzen
        }
        else
        {
            turnLeft();
            lineFollow();
            turnRight();
            lineFollow(); //bevor Schwabell absetzen
        }
    }
    else
    { //Fall4
        if(arrayW[1]==1||(arrayW[0] == 1){
            lineFollow(long); //um zu 2 zu gehen
        }else{
            lineFollow(short); //um zu 1 zu gehen
        }            
        turnLeft(back);            
        lineFollow(); //bis Node unten im Robotor ist
        longMotor(down); //um Node festzuhalten
        move(back);
        turn2(); //nach Links            
        if(arrayW[1]==1){
            lineFollow(long); // um von 2 zu 0 zu gehen
        }else{ // arrayW[2] == 0
            lineFollow(short); // um von 1 zu 0 zu gehen oder von 2 zu 1
        }         
        turnRight(back);
        longMotor(down);
        lineFollow(); //bis Node eingesammelt ist
        longMotor(up);
        if (arrayW[0] == 1){
            turnLeft(180°);
            turnRight();
            lineFollow(bis Querlinie);
            move(mini);
            turnLeft();
            lineFollow(bis fakeQuerlinie);
            move(mini);
            turn2(nach Rechts);
            lineFollow(bis zweiteQuerlinie);
            move(mini);
            turn2(nach Links);
            lineFollow(); //bevor Schwabell absetzen
        }else {
            turnLeft();
            move(); //kleines Stückchen damit bei der nächsten Drehung auf Linie ist
            turLeft();
            move(); // bis auf linie um Linie zu folgen
            lineFollow(bis fakeQuerlinie);
            move(mini);
            turn2(nach Links);
            lineFollow(bis zweiteQuerlinie);
            move(mini);
            turn2(nach Rechts);
            lineFollow(); //bevor Schwabell absetzen
        }
    }

    move();
    schwabellMotor(down); //schwabell absetzen
    move(back);
    turn2(180°);
    lineFollow(bis Querlinie);
    move(mini);

    if (Fall == 3)
    {
        turn2(nach Rechts);
    }
    else
    {
        turn2(nach Links);
    }
    lineFollow(bis Querlinie);
    move(mini);
    int currentColor; // 1 = red, 2 = blue
    for (int i = 0; i < 2; i++)
    {
        int x; //Variable für Richtung der Drehungen
        if ((Fall == 3 && i == 0) || ((Fall == 1 || Fall == 2 || Fall == 4) && i == 1))
        {
            currentColor = 1;
            x = 1;
        }
        else
        {
            currentColor = 2;
            x = -1;
        }

        if ((currentColor == 1 && (rot == 1 || rot == 3)) || (currentColor == 2 && (blau == 1 || blau == 3)))
        {
            turn2(nachRechts, x);
            lineFollow();
            longMotor(halfDown); //node absetzen
            move(back);
            if (i == 0)
            {
                schwabellMotor(up);
                longMotor(up);
                move(back); //bis kurz vor Node
                longMotor(down);
                move();        //evt. Linefollow!!
                longMotor(up); //node von unten nach oben getauscht
            }
            turn2(180°); // (x)
            lineFollow(bis Querlinie);
            move(mini);
            if (i == 0)
            {
                turn2(nach Links, x);
                lineFollow(bis zweite Querlinie);
            }
            else if (i == 1)
            {
                turn2(nach rechts);
                if (currentColor == 1)
                {
                    lineFollow(bis Querlinie);
                }
                else
                {
                    lineFollow(bis dritte Querlinie);
                }
                move(mini);
                turn2(nach Links); // eher turn1 nach links, macht aber eventuell Probleme mit der Positionierung
            }
        }
        else
        {
            lineFollow(bis Querlinie);
            move(mini);
            turn2(nach rechts, x);
            lineFollow(bis Fakequerlinie);
            move(mini);
            turn2(nach rechts, x);
            lineFollow();        //bis zum Nodeabsetzen
            longMotor(halfDown); //node absetzen
            move(back);          //darf nicht zu lang sein
            turn2(180°);
            lineFollow(bis Querlinie);
            move(mini);
            turn2(nach links, x);
            lineFollow(bis Fakequerlinie);
            if (i == 0)
            {
                schwabellMotor(up);
                longMotor(up);
                move(back); //bis kurz vor Node
                longMotor(down);
                lineFollow();  //bis node eingesammelt
                longMotor(up); //node von unten nach oben getauscht
                lineFollow(bis Fakequerlinie);
                move(mini);
                turn2(nach Links, x);
                lineFollow(bis dritte Querlinie);
            }
            else if (i == 1)
            {
                move(mini);
                if (currentColor == 2)
                {
                    turn2(nach rechts);
                    lineFollow(bis vierte Querlinie);
                    move(mini);
                    turn2(nach Links);
                }
            }
        }
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
}