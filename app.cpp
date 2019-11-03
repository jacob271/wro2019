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

//Enthält nurnoch die Logik - Die weiteren Untermethoden finden sich in methods.cpp
void logic()
{
    //waitForButton();
    if (routerW[1] == 0) //blau
    {
        routerW[1] = 2;
        updateLogDatei();
        if (blue == 1 || blue == 4) //drehen
        {
            line2(1, 4, pGL2, dGL2, "degree", 50, 1, true);
            routerEinsammeln(false, 3, true);
            liftParallel = true;
            router(4, 3, 1, 2, false);
        }
        else //nicht drehen
        {
            routerEinsammeln(true, 3, false);
            turn2(1, 5, "degree", -spin90, 1, true);
            router(3, 3, 1, 2, false);
        }

        if (blue == 1 || blue == 3)
        {
            router(1, 2, 10, 1, false);
            manualSetDriveDirection = true;
            city(2, 1, -2, 2, false);
            routerAbladen(LSr, true, "blueL");
            city(-2, 2, -3, 2, false); //M2
        }
        else
        {
            line1(3, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
            if (liftParallel)
            {
                line1(3, 3, pGL1, dGL1, LSr, true, "degree", 200, 3, false, longMotor, liftParallelSpeed, "degree", 250, true); //parallel
            }
            else
            {
                line1(3, 3, pGL1, dGL1, LSr, true, "degree", 200, 3, false);
            }
            manualSetDriveDirection = false;
            city(1, 2, -1, 1, true);
            routerAbladen(LSl, false, "blueR");
            city(-1, 1, -3, 2, false); //M2
        }
        kabelAbladen(true, true);
        cout << "Kabel abgeladen" << endl;
        updateLogDatei();

        //rot
        if (routerO[1] == 0 && (red == 3 || red == 4))
        {
            move(cSpeed, -6, 1, 1, "degree", 50, -1, true);
            turn2(1, 5, "degree", spin180, 1, true);
            routerEinsammeln(true, 0, false); //M7
            routerO[1] = 2;
            turn2(1, 5, "degree", spin90, 1, true);
            line2(1, 3, pGL2, dGL2, "degree", 150, 3, false);
            if (red == 3)
            {
                manualSetDriveDirection = true;
                city(4, 1, -4, 2, false);
                routerAbladen(LSl, true, "redR");
                city(-4, 2, 6, 4, false); //M2
            }
            else
            {
                city(4, 1, -5, 3, true);
                routerAbladen(LSr, false, "redL");
                city(-5, 3, 6, 4, false);
            }
        }
        else if (routerO[2] == 0 && (red == 3 || red == 4))
        {
            //M4
            city(-3, 2, 4, 1, false);
            routerEinsammeln(true, 3, true);
            routerO[2] = 2;
            if (red == 3)
            {
                turn2(1, 5, "degree", spin180, 1, true);
                line2(1, 4, pGL2, dGL2, "degree", 150, 6, false);
                line1(cSpeed, 30, pGL2, dGL2, LSl, false, "degree", 150, 30, false, longMotor, liftParallelSpeed, "degree", 230, true);
                routerAbladen(LSl, true, "redR");
                city(-4, 2, 6, 4, false);
            }
            else
            {
                turn2(1, 5, "degree", spin90, 1, true);
                line2(1, 3, pGL2, dGL2, "degree", 180, 3, false); //M5
                city(5, 1, -5, 3, true);
                routerAbladen(LSr, false, "redL");
                city(-5, 3, 6, 4, false);
            }
        }
        else if (routerO[2] == 0)
        {
            city(-3, 2, 4, 1, true);
            routerEinsammeln(false, 1, true);
            liftParallel = true;
            routerO[2] = 2;
            if (red == 1)
            {
                manualSetDriveDirection = true;
                city(4, 1, -4, 2, true); // parallel
                liftParallel = false;
                routerAbladen(LSl, true, "redR");
                city(-4, 2, 6, 4, false);
            }
            else
            {
                manualSetDriveDirection = true;
                city(4, 1, -5, 3, true); //parallel
                routerAbladen(LSr, false, "redL");
                city(-5, 3, 6, 4, false);
            }
        }
        else
        {
            city(-3, 2, 3, 3, true);
            routerEinsammeln(false, 4, true);
            liftParallel = true;
            routerO[1] = 2; //M4
            line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false);
            line2(1, 4, pGL2, dGL2, "degree", 165, 4, false, longMotor, liftParallelSpeed, "degree", 250, true); //parallel
            liftParallel = false;

            //city(3, 1, 4, 1, true);
            //mediumMotor(longMotor, 60, "degree", 250, true);
            if (red == 1)
            {
                manualSetDriveDirection = true;
                city(4, 1, -4, 2, false);
                routerAbladen(LSl, true, "redR");
                city(-4, 2, 6, 4, false);
            }
            else
            {
                city(4, 1, -5, 3, false);
                routerAbladen(LSr, false, "redL");
                city(-5, 3, 6, 4, false);
            }
        }
        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
        line1(3, 3, pGL1, dGL1, LSr, true, "degree", 150, 3, false);
    }
    else //rot
    {
        routerW[2] = 2;
        if (red == 1 || red == 2) //drehen
        {
            line2(cSpeed, 4, pGL2, dGL2, "degree", 50, 1, true);
            routerEinsammeln(false, 1, true);
            liftParallel = true;
            router(4, 1, 6, 2, false);
        }
        else //nicht drehen
        {
            routerEinsammeln(true, 3, true);
            turn2(1, 5, "degree", spin90, 1, true);
            line1(1, 4, pGL1, dGL1, LSr, true, "degree", 195, 4, false);
            router(5, 1, 6, 2, false);
        }

        if (red == 1 || red == 3)
        {
            router(6, 2, 7, 3, false);
            manualSetDriveDirection = false;
            city(5, 3, -4, 2, true);
            routerAbladen(LSl, true, "redR");
            city(-4, 2, -3, 2, true);
        }
        else
        {
            if (liftParallel)
            {
                line1(cSpeed, 4, pGL1, dGL1, LSl, false, "crossline", 0, 4, false, longMotor, liftParallelSpeed, "degree", 250, true);
                liftParallel = false;
            }
            else
            {
                line1(cSpeed, 4, pGL1, dGL1, LSl, false, "crossline", 0, 4, false);
            }
            line1(4, 4, pGL1, dGL1, LSl, false, "degree", 150, 4, false);
            manualSetDriveDirection = true;
            city(6, 2, -5, 3, true);
            routerAbladen(LSr, false, "redL");
            city(-5, 3, -3, 2, true);
        }
        kabelAbladen(true, true);
        cout << "Kabel abgeladen" << endl;
        updateLogDatei();

        //blau

        if (routerO[1] == 0 && (blue == 3 || blue == 2))
        {
            turn2(1, 5, "degree", spin180, 1, true);
            routerEinsammeln(true, 0, false);
            routerO[1] = 2;
            turn2(1, 5, "degree", -spin90, 1, true);
            line2(1, 3, pGL2, dGL2, "degree", 150, 3, false);
            if (blue == 3)
            {
                manualSetDriveDirection = false;
                city(3, 3, -2, 2, false);
                routerAbladen(LSr, true, "blueL");
                city(-2, 4, 5, 1, false);
            }
            else
            {
                city(3, 3, -1, 1, true);
                routerAbladen(LSl, false, "blueR");
                city(-1, 1, 5, 1, false);
            }
        }
        else if (routerO[0] == 0 && (blue == 3 || blue == 2))
        {
            city(-3, 2, 3, 3, false);
            routerEinsammeln(true, 1, true);
            routerO[0] = 2;
            if (blue == 3)
            {
                turn2(1, 5, "degree", spin180, 1, true);
                line2(1, 4, pGL2, dGL2, "degree", 150, 6, false);
                line1(cSpeed, 30, pGL2, dGL2, LSr, true, "degree", 150, 30, false, longMotor, liftParallelSpeed, "degree", 230, true);

                routerAbladen(LSr, true, "blueL");
                city(-2, 2, 5, 1, false);
            }
            else
            {
                turn2(1, 5, "degree", -spin90, 1, true);
                line2(1, 3, pGL2, dGL2, "degree", 160, 3, false);
                city(2, 3, -1, 1, true);
                routerAbladen(LSl, false, "blueR");
                city(-1, 1, 5, 1, false);
            }
        }
        else if (routerO[0] == 0)
        {
            city(-3, 2, 3, 3, true);
            routerEinsammeln(false, 2, true);
            liftParallel = true;
            routerO[0] = 2;
            if (blue == 1)
            {
                manualSetDriveDirection = false;
                city(3, 3, -2, 2, true); // parallel
                liftParallel = false;
                routerAbladen(LSr, true, "blueL");
                city(-2, 2, 5, 1, false);
            }
            else
            {
                manualSetDriveDirection = false;
                city(3, 3, -1, 1, true); //parallel
                routerAbladen(LSl, false, "blueR");
                city(-1, 1, 5, 1, false);
            }
        }
        else
        {
            city(-3, 2, 4, 1, true);
            routerEinsammeln(false, 3, true);
            liftParallel = true;
            routerO[1] = 2;
            manualSetDriveDirection = false;
            if (blue == 1)
            {
                manualSetDriveDirection = false;
                city(4, 3, -2, 2, true); // parallel
                liftParallel = false;
                routerAbladen(LSr, true, "blueL");
                city(-2, 2, 5, 1, false);
            }
            else
            {
                city(4, 3, -1, 1, false); //parallel
                routerAbladen(LSl, false, "blueR");
                city(-1, 1, 5, 1, false);
            }
        }
        line2(cSpeed, 4, pGL2, dGL2, "degree", 30, 3, false);
        turn1(motor_right, 3, true, 3, "degree", goTurn90, 3, false);
    }
    kabelSammeln(false);
    bool greenFirst = true; // green == true
    bool kabelAbladenShort = true;
    if (routerW[2] == 0)
    {
        routerW[2] = 2;
        if (green == 3 || green == 2)
        {
            router(5, 3, 4, 3, true);
            routerEinsammeln(false, 4, true);
            liftParallel = true;
            router(4, 1, 6, 2, false);
            line1(1, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true); //parallel
            liftParallel = false;
        }
        else
        {
            routerEinsammeln(true, 1, true);
            turn2(1, 5, "degree", spin90, 1, true);
            line1(1, 4, pGL1, dGL1, LSr, true, "degree", 195, 4, false);
            router(5, 1, 6, 2, false);
            line1(1, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
        }
        line1(3, 3, pGL1, dGL1, LSl, false, "degree", 150, 3, false);
        if (green == 4 || green == 2)
        {
            city(6, 2, -7, 3, true);
            routerAbladen(LSl, false, "greenR");
            city(-7, 3, -9, 4, false);
            kabelAbladenShort = false;
        }
        else
        {
            city(6, 2, -8, 4, true);
            routerAbladen(LSr, true, "greenL");
            city(-8, 4, -9, 4, false);
            kabelAbladenShort = true;
        }
    }
    else
    {
        routerW[0] = 2; //M4
        greenFirst = false;
        if (yellow == 3 || yellow == 4)
        {
            router(5, 3, 3, 3, true);
            routerEinsammeln(false, 2, true);
            liftParallel = true;
            router(3, 3, 1, 2, false);
            line1(3, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true); //parallel
            liftParallel = false;
        }
        else
        {
            router(5, 3, 3, 3, false);
            routerEinsammeln(true, 1, true);
            turn2(1, 5, "degree", -spin90, 1, true);
            line1(1, 4, pGL1, dGL1, LSl, false, "degree", 195, 4, false);
            router(2, 3, 1, 2, false);
            line1(3, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
        }
        line1(3, 3, pGL1, dGL1, LSr, true, "degree", 150, 3, false);
        if (yellow == 4 || yellow == 2)
        {
            city(1, 2, -11, 1, true);
            routerAbladen(LSr, false, "yellowL");
            city(-11, 1, -9, 4, false);
            kabelAbladenShort = false;
        }
        else
        {
            city(1, 2, -10, 4, true);
            routerAbladen(LSl, true, "yellowR");
            city(-10, 4, -9, 4, false);
            kabelAbladenShort = true;
        }
    }
    kabelAbladen(true, kabelAbladenShort);
    cout << "Zweites Kabel abgeladen!" << endl;

    if (greenFirst == true)
    {
        if (yellow == 2 || yellow == 1) //nicht drehen
        {
            if (routerO[2] == 0)
            {
                city(-9, 4, 5, 3, false);
                routerEinsammeln(true, 1, true);
                routerO[2] = 2;
                turn2(1, 5, "degree", -spin90, 1, true);
                city(4, 3, 15, 2, false);
            }
            else if (routerO[1] == 0)
            {
                city(-9, 4, 4, 3, true);
                routerEinsammeln(true, 1, true);
                routerO[1] = 2;
                turn2(1, 5, "degree", -spin90, 1, true);
                city(3, 3, 15, 2, false);
            }
            else
            {
                city(-9, 4, 2, 1, false);
                routerEinsammeln(true, 3, true);
                routerO[0] = 2;
                turn2(1, 5, "degree", -spin90, 1, true);
                line2(1, 3, pGL2, dGL2, "degree", 215, 1, false);
                city(2, 3, 15, 2, false);
            }
        }
        else //drehen
        {
            if (routerO[2] == 0)
            {
                city(-9, 4, 5, 3, false);
                line2(cSpeed, 3, pGL2, dGL2, "degree", 50, 1, true);
                routerEinsammeln(false, 2, true);
                liftParallel = true;
                routerO[2] = 2;
                city(5, 3, 1, 2, false);
            }
            else if (routerO[1] == 0)
            {
                city(-9, 4, 4, 3, true);
                routerEinsammeln(false, 2, true);
                liftParallel = true;
                routerO[1] = 2;
                city(4, 3, 1, 2, false);
            }
            else
            {
                city(-9, 4, 3, 1, true);
                routerEinsammeln(false, 3, true);
                liftParallel = true;
                routerO[0] = 2;
                city(3, 3, 1, 2, false);
            }
            city(1, 2, 15, 2, false); //parallel
        }
        if (yellow == 4 || yellow == 2)
        {
            manualSetDriveDirection = false;
            city(15, 2, -11, 1, true);
            routerAbladen(LSr, false, "yellowL");
            city(-11, 1, 14, 2, false);
            //-> base
            line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
            move(cSpeed, 4, 1, 0.9, "degree", 700, 1, true);
        }
        else
        {
            city(15, 2, -10, 4, true);
            routerAbladen(LSl, true, "yellowR");
            city(-10, 4, 13, 3, false);
            //-> base
            turn1(motor_right, 4, true, 4, "degree", 480, 4, false);
            move(cSpeed, 4, 1, 1, "degree", 400, 1, true);
        }
    }
    else
    {
        //green
        if (green == 4 || green == 1) //nicht drehen
        {
            if (routerO[2] == 0)
            {
                city(-9, 4, 5, 3, false);
                routerEinsammeln(true, 1, true);
                routerO[2] = 2;
                turn2(1, 5, "degree", spin90, 1, true);
                line2(1, 3, pGL2, dGL2, "degree", 200, 1, false);
                city(5, 1, 8, 2, false);
            }
            else if (routerO[1] == 0)
            {
                city(-9, 4, 3, 1, false);
                routerEinsammeln(true, 3, true);
                routerO[1] = 2;
                turn2(1, 5, "degree", spin90, 1, true);
                city(4, 1, 8, 2, false);
            }
            else
            {
                city(-9, 4, 2, 1, false);
                routerEinsammeln(true, 3, true);
                routerO[0] = 2;
                turn2(1, 5, "degree", spin90, 1, true);
                city(3, 1, 8, 2, false);
            }
        }
        else //drehen
        {
            if (routerO[2] == 0)
            {
                city(-9, 4, 4, 3, true);
                routerEinsammeln(false, 4, true);
                liftParallel = true;
                routerO[2] = 2;
                city(4, 1, 8, 2, false);
            }
            else if (routerO[1] == 0)
            {
                city(-9, 4, 3, 1, true);
                routerEinsammeln(false, 1, true);
                liftParallel = true;
                routerO[1] = 2;
                city(3, 1, 8, 2, false);
            }
            else
            {
                city(-9, 4, 2, 1, false);
                routerEinsammeln(false, 1, true);
                liftParallel = true;
                routerO[0] = 2;
                city(2, 1, 8, 2, false);
            }
        }
        if (green == 4 || green == 2)
        {
            manualSetDriveDirection = true;
            city(8, 2, -7, 3, true);
            routerAbladen(LSl, false, "greenR");
            city(-7, 3, 13, 3, false);
        }
        else
        {
            city(8, 2, -8, 4, true);
            routerAbladen(LSr, true, "greenL");
            city(-8, 4, 13, 3, false);
        }
        //-> base
        turn1(motor_right, 4, true, 4, "degree", 480, 4, false);
        move(cSpeed, 4, 1, 1, "degree", 400, 1, true);
    }
}
void test()
{
    while(true){
        waitForButton();
        int red = getRGB(LSr,1);
        int green = getRGB(LSr,2);
        int blue = getRGB(LSr, 3);
        cout << "red: " << red << " green: " << green << " blue: " << blue << endl;
    }
}

void main_task(intptr_t unused)
{
    //Log-Datei initialisieren
    std::ofstream out("logFile.txt");
    std::streambuf *coutbuf = cout.rdbuf(); //save old buf
    cout.rdbuf(out.rdbuf());                //redirect cout to out.txt!
    start();
    //test();
    //return;

    //Anfang
    move(1, 3, 0.4, 1, "degree", 220, 3, false);
    move(cSpeed, 3, 1, 0.4, "degree", 205, 3, false);
    move(3, 3, 1, 1, "degree", 80,3, false);
    positionenScannen();
    city(9, 4, 5, 3, false);
    routerScannen(HTr, "routerO");
    turn1(motor_left, 3, true, 3, "degree", 490, 3, false);
    kabelSammeln(true);
    //line2(1, 3, pGL2, dGL2, "degree", 100, 3, false);
    routerScannen(HTl, "routerW");
    fallunterscheidung();
    cout << "fallunterscheidung fertig: " << endl;
    updateLogDatei();

    //Anfang Ende

    logic();

    int neededTime = run.getTime();

    //int neededTime = run.getTime();
    cout << "Needed time: " << neededTime << endl;
    cout << batteryLevel << " " << batteryFactor << " " << fall1 << " " << fall2 << " " << neededTime << " " << blue << " " << red << " " << green << " " << yellow << " " << routerO[0] << " " << routerO[1] << " " << routerO[2] << " " << routerW[0] << " " << routerW[1] << " " << routerW[2] << endl;
    char msgbuf[10];
    sprintf(msgbuf, "Time: %d", neededTime);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    cout.rdbuf(coutbuf);

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