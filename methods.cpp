#include "blocks.h"

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

    cout << batteryFactor << endl;

    pGL1 = pGL1 * batteryFactor;
    dGL1 = dGL1 * batteryFactor;

    pGL2 = pGL2 * batteryFactor;
    dGL2 = dGL2 * batteryFactor;

    cout << "Battery at: " << batteryLevel << "Volt" << endl;
    cout << "speedsLevel:" << endl;
    cout << "1 " << speedLevel(1) << endl;
    cout << "2 " << speedLevel(2) << endl;
    cout << "3 " << speedLevel(3) << endl;
    cout << "4 " << speedLevel(4) << endl;
    cout << "5 " << speedLevel(5) << endl;
    cout << "6 " << speedLevel(6) << endl;
    display(batteryLevel);

    waitForButton();
    resetMotors();
    tslp_tsk(200);
    run.reset();
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
    //todo - Überarbeiten oder rausschmeißen
    cout << "-" << endl;
    cout << "time " << run.getTime() << endl;
    cout << "fall1 " << fall1 << " fall2 " << fall2 << endl;
    cout << "blue " << blue << " red " << red << " green " << green << " yellow " << yellow << endl;
    cout << "routerO " << routerO[0] << " " << routerO[1] << " " << routerO[2] << endl;
    cout << "routerW " << routerW[0] << " " << routerW[1] << " " << routerW[2] << endl;
    cout << "-" << endl;
}

void positionenScannen()
{
    line1(cSpeed, 90, pGL1, dGL1, LSr, false, "degree", 335, 90, false);
    for (int i = 0; i < 3; i++)
    {
        ev3_speaker_play_tone(NOTE_F4, 3);
        positions[i] = line1(cSpeed, 90, pGL1, dGL1 * 0.6, LSr, false, "degree", 147, 60, false, true, HTr, "color");
    }
    positions[3] = findColor(positions, "positions");
    line1(cSpeed, 90, pGL1, dGL1, LSr, false, "degree", 315, 3, false); //330
    turn1(motor_right, 3, true, 3, "degree", goTurn90, 3, false);
    line1(cSpeed, 3, pGL1, dGL1, LSr, true, "degree", 620, 3, false);
}

void kabelSammeln(bool south)
{
    if (south)
    {
        line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false, doubleLever, -70, "degree", leverDistance, true);
        turn1(motor_left, cSpeed, true, 4, "degree", 270, 4, false);
        turn1(motor_right, cSpeed, true, 4, "degree", 250, 1, true);
        line2(4, 30, pGL2 * 0.5, dGL2 * 0.5, "degree", 70, 30, false);
        move(cSpeed, 30, 1, 1, "degree", 50, 1, true);
        mediumMotor(doubleLever, 50, "degree", leverDistance, false);
        mediumMotor(doubleLever, 100, "time", 300, true);
        move(-1, -4, 1, 1, "degree", 275, -1, true);
        turn2(1, 5, "degree", spin90, 1, true);
        
        /*
        line1(cSpeed, 4, pGL1, dGL1, LSl, false, "degree", 100, 4, false);
        line1(cSpeed, 4, pGL1, dGL1, LSl, false, "crossline", 0, 4, false, doubleLever, -100, "degree", 200, true);
        move(4, 4, 1, 0.7, "degree", 300, 35, false);
        move(35, 30, 1, 1, "degree", 235, 1, true);
        mediumMotor(doubleLever, 50, "degree", leverDistance, false);
        mediumMotor(doubleLever, 100, "time", 300, true);
        move(-1, -4, 1, 1, "degree", 165, -4, false);
        move(-4, -4, 1, 0.7, "degree", 200, -1, true);
        turn2(1, 5, "degree", spin90, 1, true);
        */
    }
    else
    {
        line2(1, 4, pGL2, dGL2, "crossline", 0, 4, false, doubleLever, -70, "degree", leverDistance, true);
        turn1(motor_right, cSpeed, true, 4, "degree", 270, 4, false);
        turn1(motor_left, cSpeed, true, 4, "degree", 250, 1, true);
        line2(4, 30, pGL2 * 0.5, dGL2 * 0.5, "degree", 70, 30, false);
        move(cSpeed, 30, 1, 1, "degree", 50, 1, true);
        mediumMotor(doubleLever, 50, "degree", leverDistance, false);
        mediumMotor(doubleLever, 100, "time", 300, true);
        move(-1, -4, 1, 1, "degree", 275, -1, true);
        turn2(1, 5, "degree", -spin90, 1, true);

        /*
        line1(cSpeed, 4, pGL1, dGL1, LSr, true, "degree", 100, 4, false);
        line1(cSpeed, 4, pGL1, dGL1, LSr, true, "crossline", 0, 4, false, doubleLever, -100, "degree", 200, true);
        move(4, 4, 0.7, 1, "degree", 300, 35, false);
        move(35, 30, 1, 1, "degree", 235, 1, true);
        mediumMotor(doubleLever, 50, "degree", leverDistance, false);
        mediumMotor(doubleLever, 100, "time", 300, true);
        move(-1, -4, 1, 1, "degree", 165, -4, false);
        move(-4, -4, 0.7, 1, "degree", 200, -1, true);
        turn2(1, 5, "degree", -spin90, 1, true);
        */
    }
    line2(1, 4, pGL2, dGL2, "degree", 100, 4, false);
}

void routerScannen(sensor_port_t searchSensor, std::string mode)
{
    //direkt nach der Drehung
    int router[3] = {0};
    int i = 0;
    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
    router[i] = line2(cSpeed, 3, pGL2, dGL2 * 0.6, "degree", 280, 3, false, true, searchSensor, "bw");
    line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
    i++;
    if (mode == "routerO")
    {
        router[i] = line2(cSpeed, 4, pGL2, dGL2 * 0.6, "degree", 320, 3, false, true, searchSensor, "bw");
        line2(cSpeed, 3, pGL2, dGL2, "crossline", 0, 3, false);
        line2(cSpeed, 3, pGL2, dGL2, "degree", 340, 3, false);
        //crossline(cSpeed, 1);
    }
    else
    {
        router[i] = line2(cSpeed, 4, pGL2, dGL2 * 0.6, "degree", 260, 10, true, true, searchSensor, "bw");//36
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

void routerEinsammeln(bool directionEast, int mode, bool crossline)
{
    if (directionEast == true)
    {
        if (mode == 0) //direkt nach dem Kabel
        {
            mediumMotor(longMotor, -100, "degree", 210, true);
            line2(cSpeed, 5, pGL2, dGL2, "degree", 100, 5, false);
            line2(cSpeed, 5, pGL2, dGL2, "crossline", 0, 5, false);
            line2(cSpeed, 5, pGL2, dGL2, "degree", 235, 1, true);
            mediumMotor(longMotor, 60, "degree", 220, true);
            move(-1, -4, 1, 1, "degree", 95, -1, true);
        }
        else
        {
            motor_port_t turnMotor = motor_right;
            if (mode == 3)
            {
                turnMotor = motor_left;
            }
            if (crossline)
            {
                line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
                line2(cSpeed, 4, pGL2, dGL2, "degree", 260, 1, true);
            }
            turn1(turnMotor, -1, false, -4, "degree", 450, -1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            //move(1,2,1,1,"degree",30,2,false);
            line2(1, 2, pGL2 * 0.3, dGL2 * 0.3, "degree", 240, 1, true);
            mediumMotor(longMotor, 60, "degree", 220, true);
            move(-1, -3, 1, 1, "degree", 95, -1, true);
        }
    }
    else
    {
        if (mode == 1)
        {
            turn2(1, 5, "degree", -spin90, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 35, "degree", spin90, 1, true);
        }
        else if (mode == 2)
        {
            turn2(1, 5, "degree", spin90, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 35, "degree", -spin90, 1, true);
        }
        else if (mode == 3)
        {
            turn2(1, 5, "degree", -spin90, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 35, "degree", -spin90, 1, true);
        }
        else if (mode == 4)
        {
            turn2(1, 5, "degree", spin90, 1, true);
            mediumMotor(longMotor, -100, "degree", 210, true);
            turn2(1, 35, "degree", spin90, 1, true);
        }
    }
}

void kabelAbladen(bool stop, bool kurzeDistanz)
{
    if (kurzeDistanz == true)
    {
        line2(cSpeed, 3, pGL2, dGL2, "degree", 70, 3, false);
        move(cSpeed, 3, 1, 1, "colorR", 0, 3, false);
    }
    else
    {
        line2(cSpeed, 4, pGL2, dGL2, "degree", 230, 4, false);
        move(cSpeed, 3, 1, 1, "colorR", 0, 3, false);
    }
    move(cSpeed, 6, 1, 1, "degree", 190, 1, true);
    //move(cSpeed, 35, "degree", 160, 1, true);
    //tslp_tsk(300);
    mediumMotor(doubleLever, -50, "time", 300, false);
    tslp_tsk(200);
    if (kurzeDistanz == true)
    {
        move(-1, -6, 1, 1, "degree", 280, -1, true);
    }
    else
    {
        move(-1, -6, 1, 1, "degree", 250, -1, true);
    }

    mediumMotor(doubleLever, 70, "degree", leverDistance, true);
}

void routerAbladen(sensor_port_t followSensor, bool lang)
{
    if (followSensor == LSr)
    {
        line1(cSpeed, 30, pGL2, dGL2, followSensor, true, "degree", 50, 30, false, longMotor, liftParallelSpeed, "degree", 230, true);
        line1(cSpeed, 30, pGL2, dGL2, followSensor, true, "colorL", 0, 1, true, longMotor, liftParallelSpeed, "degree", 230, true);
        //move(cSpeed, 30, 1, 1, "degree", 5, 1, true);
    }
    else
    {
        line1(cSpeed, 30, pGL2, dGL2, followSensor, false, "degree", 50, 30, false, longMotor, liftParallelSpeed, "degree", 230, true);
        line1(cSpeed, 30, pGL2, dGL2, followSensor, false, "colorR", 0, 1, true, longMotor, liftParallelSpeed, "degree", 230, true);
        //move(cSpeed, 30, 1, 1, "degree", 5, 1, true);
    }
    mediumMotor(longMotor, -30, "degree", 230, true);
    if (lang == true)
    {
        move(-1, -3, 1, 1, "degree", 90, -1, true);
        mediumMotor(longMotor, 100, "degree", 230, true);
    }
    else
    {
        move(-1, -3, 1, 1, "degree", 140, -1, true);
        mediumMotor(longMotor, 100, "degree", 230, true);
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

    /*
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
    */
}

//Gibt die Fahrtrichtung im Uhrzeigersinn anhand der Position auf dem Router-Kreis bzw. City-Kreis und der Himmelsrichtung (1-4) zurück
bool direction(int position, int direction, int nPositions)
{
    if (nPositions == 10)
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
    else
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
        else if ((direction == 4) && (position == 14 || position == 15 || position == 16 || position == 1))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int getDistance(int startPosition, int startDirection, int endPosition, int nPositions)
{
    int distanceForw = 0;
    for (int i = startPosition; i != endPosition; i++)
    {
        if (i == (nPositions + 1))
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
    cout << "distanceForw: " << distanceForw << endl;

    int distanceBack = 0;
    for (int i = startPosition; i != endPosition; i--)
    {
        if (i == 0)
        {
            i = nPositions;
        }
        if (i != endPosition)
        {
            distanceBack--;
        }
        else
        {
            break;
        }
    }

    cout << "distanceBack: " << distanceBack << endl;

    int distance = 0;
    if (distanceForw == abs(distanceBack))
    {
        //Nimmt standardmäßig die Distanz gegen den Uhrzeigersinn, wenn die Position nicht bekannt ist
        if (direction(startPosition, startDirection, nPositions))
            distance = distanceForw;
        else
            distance = distanceBack;
    }
    else if (distanceForw < abs(distanceBack))
    {
        distance = distanceForw;
    }
    else
    {
        distance = distanceBack;
    }
    return distance;
}

//Der Roboter kann anhand der Start- und Endposition auf dem Router-Kreis den kürzesten Weg erkennen und fährt diesen anschließend
void router(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop)
{
    //1 = kurze Seite im Süden, dann im Uhrezeigersinn
    cout << "Enter Router-Kreis at: " << currentPosition << " " << currentDirection << endl;

    //Berechnet die Abstände im und gegen den Uhrzeigersinn auf dem Router-Kreis
    int distance = getDistance(currentPosition, currentDirection, endPosition, 10);

    cout << "distance: " << distance << std ::endl;

    //Legt anhand der kürzeren Strecke die Fahrtrichtung fest. Falls es keine kürzere gibt entscheidet die aktuelle Orientierung des Roboters
    bool driveDirection;
    if (distance >= 0)
    {
        driveDirection = true;
    }
    else
    {
        driveDirection = false;
    }
    cout << "driveDirection: " << driveDirection << endl;

    //Dreht den Roboter gegebenenfalls in Fahrtrichtung
    if (direction(currentPosition, currentDirection, 10) != driveDirection)
    {
        turn2(1, 5, "degree", spin180, 1, true);
    }

    //übernimmt Fahrtgeschwindigkeit oder setzt Startspeed
    if (cSpeed < speedLevel(1))
        cSpeed = 1;

    //Strecken
    int toMiddle = 310; //Von Crossline bis der Roboter zwischen den Linien steht
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
                cout << "position: " << i << endl;
                switch (i)
                {
                case 1:
                case 6:
                    if (liftParallel)
                    {
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 215, 3, false, longMotor, liftParallelSpeed, "degree", 100, true);
                        liftParallel = false;
                    }
                    else
                    {
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 215, 3, false);
                    }
                    turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 110, endSpeed, stopNow);
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
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 155, endSpeed, stopNow);
                    break;
                default:
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        turnMotor = motor_right;
        for (int i = currentPosition; i != endPosition; i--)
        {
            if (i == 0)
            {
                i = 10;
            }

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
                cout << "position: " << i << endl;
                switch (i)
                {
                case 1:
                case 6:
                    if (liftParallel)
                    {
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 215, 3, false, longMotor, liftParallelSpeed, "degree", 100, true);
                        liftParallel = false;
                    }
                    else
                    {
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 215, 3, false);
                    }
                    turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 110, endSpeed, stopNow);
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
                    line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                    turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                    line2(1, 3, pGL2, dGL2, "degree", 155, endSpeed, stopNow);
                    break;
                default:
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    //Dreht am Ende, falls die Orientierung noch nicht übereinstimmt
    if (direction(endPosition, endDirection, 10) != driveDirection)
    {
        turn2(1, 5, "degree", spin180, 1, true);
    }

    cout << "Exit Router-Kreis at: " << endPosition << " " << endDirection << endl;
}

void city(int currentPosition, int currentDirection, int endPosition, int endDirection, bool stop)
{
    //1 = westlichste Seite im Süden, dann im Uhrzeigersinn
    cout << "Enter City-Kreis at: " << currentPosition << " " << currentDirection << endl;

    motor_port_t turnMotor;
    int endSpeed;
    bool stopNow;

    //Alle Anfahrmöglichkeiten des inneren City-Kreises gespeichert
    int cityPositionsIn[13][7] = {{0},
                                  {-1, 16, 1},
                                  {-2, 1, 2, 3, -3},
                                  {-3, -2, -4, 3, 4},
                                  {-4, 4, 5, 6, -3},
                                  {-5, 6, 7},
                                  {-6, 7, 8},
                                  {-7, 8, 9},
                                  {-8, 10, 11, -9}, //9
                                  {-9, 9, -8, -10, 14, 11, 12},
                                  {-10, -9, 12, 13}, //14
                                  {-11, 14, 15},
                                  {-12, 15, 16}};

    int cityPositionsOut[17][7] = {{0},
                                   {1, -1, 16, -2, -3, 2},
                                   {2, 1, 3, -2},
                                   {3, -2, -3, 4, 2},
                                   {4, -3, -4, 3, 5},
                                   {5, 4, 6, -4},
                                   {6, -3, -4, -5, 7, 5},
                                   {7, -6, -5, 6, 8},
                                   {8, -6, -7, 7, 9},
                                   {9, -9, -8, -7, 8, 10},
                                   {10, -8, 9, 11},
                                   {11, -9, -8, 10, 12},
                                   {12, -9, -10, 11, 13},
                                   {13, -10, 14, 12},
                                   {14, -9, -10, -11, 15, 13},
                                   {15, 14, 16, -11, -12},
                                   {16, 15, -1, -12, 1}};

    //Haben current- und endPosition gemeinsame Anfahrpukte?
    int gemeinsameOptionen[4] = {0};
    int n = 0;
    cout << "gemeinsameOptionen: ";
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            int option1 = 0;
            int option2 = 0;
            if (currentPosition < 0)
            {
                option1 = cityPositionsIn[abs(currentPosition)][i];
            }
            else
            {
                option1 = cityPositionsOut[abs(currentPosition)][i];
            }
            if (endPosition < 0)
            {
                option2 = cityPositionsIn[abs(endPosition)][j];
            }
            else
            {
                option2 = cityPositionsOut[abs(endPosition)][j];
            }
            if (option1 == option2 && option1 != 0)
            {
                gemeinsameOptionen[n] = option1;
                cout << gemeinsameOptionen[n] << " ";
                n++;
            }
        }
    }
    cout << endl;

    //Wählt aus den gemeinsamen Optionen zunächst negative auf dem inneren Kreis aus
    int gemeinsameOption = 0;
    for (int i = 0; i < 4; i++)
    {
        if (gemeinsameOption == 0)
        {
            gemeinsameOption = gemeinsameOptionen[i];
        }
        else if (gemeinsameOptionen[i] < gemeinsameOption && gemeinsameOptionen[i] != 0)
        {
            gemeinsameOption = gemeinsameOptionen[i];
        }
    }

    cout << "finale gemeinsameOption: " << gemeinsameOption << endl;

    //directInnenToInnen();
    if ((gemeinsameOption == endPosition || gemeinsameOption == currentPosition) && currentPosition < 0 && endPosition < 0)
    {
        //Fährt von negativer Position zu anderer negativer Position
        cout << "Fahrt von innerem Kreis zum inneren Kreis" << endl;
        switch (currentPosition)
        {
        case -3:
        case -9:
            switch (endPosition)
            {
            case -2:
            case -8:
                turn1(motor_left, -1, true, -3, "degree", 503, -1, true); //M3
                turn1(motor_right, -1, false, -3, "degree", 512, -1, true);
                line2(1, 3, pGL2, dGL2, "degree", 270, 4, false);
                break;
            case -4:
            case -10:
                turn1(motor_right, -1, true, -3, "degree", 503, -1, true);
                turn1(motor_left, -1, false, -3, "degree", 512, -1, true);
                line2(1, 3, pGL2, dGL2, "degree", 270, 4, false);
                break;
            }
        case -2:
        case -8:
            turn1(motor_right, -1, true, -4, "degree", 503, -1, true);
            turn1(motor_left, -1, false, -4, "degree", 512, -1, true);
            line2(1, 3, pGL2, dGL2, "degree", 270, 4, false);
            break;
        case -4:
        case -10:
            turn1(motor_left, -1, true, -4, "degree", 503, -1, true);
            turn1(motor_right, -1, false, -4, "degree", 512, -1, true);
            line2(1, 3, pGL2, dGL2, "degree", 270, 4, false);
            break;
        }
    }
    else
    {
        //Falls nein, negative Auswahlmöglichkeiten für current- und endPosition werden aussortiert.
        //Abstände zwischen den verbleibenden Anfahroptionen auf dem Citykreis
        cout << "Alle Abstände berechnen: ";
        int abstand[24][3] = {0};
        int n = 0;
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                int option1 = 0;
                int option2 = 0;
                if (currentPosition < 0)
                {
                    option1 = cityPositionsIn[abs(currentPosition)][i];
                }
                else
                {
                    option1 = cityPositionsOut[abs(currentPosition)][i];
                }
                if (endPosition < 0)
                {
                    option2 = cityPositionsIn[abs(endPosition)][j];
                }
                else
                {
                    option2 = cityPositionsOut[abs(endPosition)][j];
                }
                if (option1 > 0 && option2 > 0)
                {
                    abstand[n][0] = option1;
                    abstand[n][1] = option2;

                    int distance = getDistance(option1, 0, option2, 16);

                    if (option1 != currentPosition)
                    {
                        if (distance > 0)
                            distance++;
                        else if (distance < 0)
                            distance--;
                        else if ((option1 > currentPosition && currentPosition > 0) || (option1 == 1 && currentPosition == 16))
                        {
                            distance++;
                        }
                        else if ((option1 < currentPosition && currentPosition > 0) || (option1 == 16 && currentPosition == 1))
                        {
                            distance--;
                        }
                    }

                    if (option2 != endPosition)
                    {
                        if (distance > 0)
                            distance++;
                        else if (distance < 0)
                            distance--;
                        else if ((option2 > endPosition && endPosition > 0) || (option2 == 1 && endPosition == 16))
                        {
                            distance--;
                        }
                        else if ((option2 < endPosition && endPosition > 0) || (option2 == 16 && endPosition == 1))
                        {
                            distance++;
                        }
                    }
                    abstand[n][2] = distance;
                    cout << "abstand: " << option1 << " " << option2 << " " << abstand[n][2] << endl;
                    n++;
                }
            }
        }
        cout << endl;
        int shortestDistance = 100;
        for (int i = 0; i >= 0; i++)
        {
            if (abstand[i][0] != 0)
            {
                if (abs(abstand[i][2]) < abs(shortestDistance))
                    shortestDistance = abstand[i][2];
            }
            else
            {
                i = -2;
            }
        }
        cout << "Kürzester Abstand: " << shortestDistance << endl;

        //Zwischenpositionen speichern
        int startTemp = 0;
        int endTemp = 0;
        for (int i = 0; i < 24; i++)
        {
            if (abstand[i][2] == shortestDistance)
            {
                startTemp = abstand[i][0];
                endTemp = abstand[i][1];
                break;
            }
        }

        if (currentPosition > 0 && startTemp > 0)
        {
            startTemp = currentPosition;
        }
        if (endPosition > 0 && endTemp > 0)
        {
            endTemp = endPosition;
        }
        cout << "startTemp: " << startTemp << " endTemp: " << endTemp << endl;

        //Legt anhand der Strecke die Fahrtrichtung fest
        bool driveDirection;
        if (shortestDistance < 0)
        {
            driveDirection = false;
        }
        else if (shortestDistance > 0)
        {
            driveDirection = true;
        }
        else
        {
            driveDirection = manualSetDriveDirection;
        }
        cout << "driveDirection: " << driveDirection << endl;

        //Strecken
        int toMiddle = 310; //Von Crossline bis der Roboter zwischen den Linien steht

        if (startTemp != currentPosition && currentPosition < 0)
        {
            //Fährt von innerem Kreis auf startTemp (eventuell gleichzeitig endPosition)
            endSpeed = 3;
            stopNow = false;
            if (startTemp == endPosition && stop == true)
            {
                endSpeed = 1;
                stopNow = true;
            }
            switch (currentPosition)
            {
            case -1:
            case -7:
                switch (startTemp)
                {
                case 1:
                case 9:
                    turn2(1, 5, "degree", -spin90, 1, true);
                    line2(1, 3, pGL2, dGL2, "degree", 300, endSpeed, stopNow);
                    driveDirection = true;
                    break;
                case 16:
                case 8:
                    turn2(1, 5, "degree", spin90, 1, true);
                    line2(1, 3, pGL2, dGL2, "degree", 248, endSpeed, stopNow);
                    driveDirection = false;
                    break;
                }
                break;
            case -5:
            case -11:
                switch (startTemp)
                {
                case 6:
                case 14:
                    turn2(1, 5, "degree", spin90, 1, true);
                    line2(1, 3, pGL2, dGL2, "degree", 300, endSpeed, stopNow);
                    driveDirection = false;
                    break;
                case 7:
                case 15:
                    turn2(1, 5, "degree", -spin90, 1, true);
                    line2(1, 3, pGL2, dGL2, "degree", 248, endSpeed, stopNow);
                    driveDirection = true;
                    break;
                }
                break;
            case -12:
            case -6:
                switch (startTemp)
                {
                case 15:
                case 7:
                    turn2(1, 5, "degree", spin90, 1, true);
                    driveDirection = false;
                    break;
                case 16:
                case 8:
                    turn2(1, 5, "degree", -spin90, 1, true);
                    driveDirection = true;
                    break;
                }
                line2(1, 3, pGL2, dGL2, "degree", 248, endSpeed, stopNow);
                break;
            case -2:
            case -8:
                switch (startTemp)
                {
                case 1:
                case 9:
                    if (startTemp == endPosition && ((endDirection == 4 && startTemp == 1) || (endDirection == 2 && startTemp == 9)))
                    {
                        move(-1, -3, 1, 0.153, "degree", 1150, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        turn2(1, 5, "degree", -spin90, 1, true);
                        move(-1, -3, 1, 1, "degree", 225, -3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = false;
                    }
                    break;
                case 2:
                case 10:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 2) || (endDirection == 3 && startTemp == 10)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 340, endSpeed, stopNow);
                        driveDirection = false;
                    }
                    break;
                case 3:
                case 11:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 3) || (endDirection == 3 && startTemp == 11)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 248, endSpeed, stopNow);
                        driveDirection = true;
                    }
                    break;
                }
                break;
            case -10:
            case -4:
                switch (startTemp)
                {
                case 6:
                case 14:
                    if (startTemp == endPosition && ((endDirection == 4 && startTemp == 6) || (endDirection == 2 && startTemp == 14)))
                    {
                        move(-1, -3, 0.153, 1, "degree", 1150, -1, true);
                        driveDirection = false;
                    }
                    else
                    {
                        turn2(1, 5, "degree", spin90, 1, true);
                        move(-1, -3, 1, 1, "degree", 225, -3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = true;
                    }
                    break;
                case 5:
                case 13:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 13) || (endDirection == 3 && startTemp == 5)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = false;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 340, endSpeed, stopNow);
                        driveDirection = true;
                    }
                    break;
                case 4:
                case 12:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 4) || (endDirection == 3 && startTemp == 12)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 248, endSpeed, stopNow);
                        driveDirection = false;
                    }
                    break;
                }
                break;
            case -3:
            case -9:
                switch (startTemp)
                {
                case 4:
                case 12:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 12) || (endDirection == 3 && startTemp == 4)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = false;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 340, endSpeed, stopNow);
                        driveDirection = true;
                    }
                    break;
                case 3:
                case 11:
                    if (startTemp == endPosition && ((endDirection == 1 && startTemp == 3) || (endDirection == 3 && startTemp == 11)))
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_left, -3, true, -3, "degree", goTurn90, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 120, 3, false);
                        turn1(motor_right, -3, true, -3, "degree", goTurn90, -1, true);
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", 340, endSpeed, stopNow);
                        driveDirection = false;
                    }
                    break;
                case 1:
                case 9:
                    if (startTemp == endPosition && ((endDirection == 2 && startTemp == 9) || (endDirection == 4 && startTemp == 1)))
                    {
                        move(-1, -3, 1, 1, "degree", 50, -3, false);
                        turn1(motor_left, -3, true, -3, "degree", 540, -3, false);
                        move(-3, -3, 1, 1, "degree", 395, -3, false);
                        turn1(motor_left, -3, true, -3, "degree", 540, -1, true);
                        driveDirection = true;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 50, -3, false);
                        turn1(motor_left, -3, true, -3, "degree", 540, -3, false);
                        move(-3, -3, 1, 1, "degree", 395, -3, false);
                        turn1(motor_right, -3, true, -3, "degree", 540, -1, true);
                        driveDirection = false;
                    }
                    break;
                case 6:
                case 14:
                    if (startTemp == endPosition && ((endDirection == 2 && startTemp == 14) || (endDirection == 4 && startTemp == 6)))
                    {
                        move(-1, -3, 1, 1, "degree", 50, -3, false);
                        turn1(motor_right, -3, true, -3, "degree", 540, -3, false);
                        move(-3, -3, 1, 1, "degree", 395, -3, false);
                        turn1(motor_right, -3, true, -3, "degree", 540, -1, true);
                        driveDirection = false;
                    }
                    else
                    {
                        move(-1, -3, 1, 1, "degree", 50, -3, false);
                        turn1(motor_right, -3, true, -3, "degree", 540, -3, false);
                        move(-3, -3, 1, 1, "degree", 395, -3, false);
                        turn1(motor_left, -3, true, -3, "degree", 540, -1, true);
                        driveDirection = true;
                    }
                    break;
                }
            }
        }
        else
        {
            //Dreht den Roboter gegebenenfalls in Fahrtrichtung
            if (direction(currentPosition, currentDirection, 16) != driveDirection && abs(shortestDistance) > 1)
            {
                turn2(1, 5, "degree", spin180, 1, true);
                cout << "turn into driveDirection" << endl;
            }
        }

        //übernimmt Fahrtgeschwindigkeit oder setzt Startspeed
        if (abs(cSpeed) < abs(speedLevel(1)))
            cSpeed = 1;
        cout << "cSpeed " << cSpeed << endl;

        int nextPosition;

        //Zählt je nach Fahrtrichtung die Positionen hoch oder runter und fährt von einer zur nächsten
        //drive
        if (driveDirection == true)
        {
            turnMotor = motor_left;
            for (int i = startTemp; i != endTemp; i++)
            {
                if (i == 17)
                    i = 1;

                nextPosition = i + 1;
                if (i == 16)
                    nextPosition = 1;

                if (i != endTemp)
                {
                    endSpeed = 3;
                    stopNow = false;
                    if (endPosition == -9)
                    {
                        stopNow = true;
                        endSpeed = 1;
                        cout << "stopNow" << endl;
                    }
                    if (nextPosition == endTemp && stop == true && endTemp == endPosition)
                    {
                        endSpeed = 1;
                        stopNow = true;
                        cout << "stopNow" << endl;
                    }
                    cout << "position: " << i << endl;
                    switch (i)
                    {
                    case 1:
                    case 9:
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 155, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
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
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                        line1(3, 3, pGL1, dGL1, LSl, false, "degree", toMiddle, endSpeed, stopNow);
                        break;
                    case 13:
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 10, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                        line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                        break;
                    case 5:
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                        line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                        break;
                    case 6:
                    case 7:
                    case 14:
                    case 15:
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                        if (liftParallel)
                        {
                            line1(3, 3, pGL1, dGL1, LSl, false, "degree", 415, endSpeed, stopNow, longMotor, liftParallelSpeed, "degree", 250, true);
                            liftParallel = false;
                        }
                        else
                        {
                            line1(3, 3, pGL1, dGL1, LSl, false, "degree", 415, endSpeed, stopNow);
                        }
                        break;
                    case 8:
                    case 16:
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                        line1(3, 3, pGL1, dGL1, LSl, false, "degree", 440, endSpeed, stopNow);
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
            for (int i = startTemp; i != endTemp; i--)
            {
                if (i == 0)
                    i = 16;

                nextPosition = i - 1;
                if (i == 1)
                    nextPosition = 16;
                if (i != endTemp)
                {
                    endSpeed = 3;
                    stopNow = false;
                    if (endPosition == -9)
                    {
                        stopNow = true;
                        endSpeed = 1;
                    }
                    if (nextPosition == endTemp && stop == true && endTemp == endPosition)
                    {
                        endSpeed = 1;
                        stopNow = true;
                    }
                    cout << "position: " << i << endl;
                    switch (i)
                    {
                    case 1:
                    case 8:
                    case 9:
                    case 16:
                        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
                        if (liftParallel)
                        {
                            line1(3, 3, pGL1, dGL1, LSr, true, "degree", 415, endSpeed, stopNow, longMotor, liftParallelSpeed, "degree", 250, true);
                            liftParallel = false;
                        }
                        else
                        {
                            line1(3, 3, pGL1, dGL1, LSr, true, "degree", 415, endSpeed, stopNow);
                        }
                        break;
                    case 2:
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 30, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                        line2(1, 3, pGL2, dGL2, "degree", 195, endSpeed, stopNow);
                        break;
                    case 10:
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 10, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
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
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 155, 3, false);
                        turn1(turnMotor, 3, true, 3, "degree", goTurn90, 3, false);
                        line2(cSpeed, 3, pGL2, dGL2, "degree", 60, endSpeed, stopNow);
                        break;
                    case 7:
                    case 15:
                        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
                        line1(3, 3, pGL1, dGL1, LSr, true, "degree", 440, endSpeed, stopNow);
                        break;
                    case 11:
                    case 12:
                    case 13:
                        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
                        line1(3, 3, pGL1, dGL1, LSr, true, "degree", toMiddle, endSpeed, stopNow);
                        break;
                    default:
                        break;
                    }
                }
                else
                    break;
            }
        }

        //Zu endPosition fahren
        //toEnd
        if (endTemp != endPosition)
        {
            switch (endPosition)
            {
            case -1:
            case -5:
            case -6:
            case -7:
            case -11:
            case -12:
                if (driveDirection)
                {
                    cout << "LSl" << endl;
                    if (liftParallel)
                    {
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true);
                        liftParallel = false;
                    }
                    else
                    {
                        line1(cSpeed, 3, pGL1, dGL1, LSl, false, "crossline", 0, 3, false);
                    }
                    line1(cSpeed, 3, pGL1, dGL1, LSl, false, "degree", miniDistance1, 1, true);
                    turn2(1, 5, "degree", spin90, 1, true);
                }
                else
                {
                    cout << "LSr" << endl;
                    if (liftParallel)
                    {
                        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true);
                        liftParallel = false;
                    }
                    else
                    {
                        line1(cSpeed, 3, pGL1, dGL1, LSr, true, "crossline", 0, 3, false);
                    }
                    line1(cSpeed, 3, pGL1, dGL1, LSr, true, "degree", miniDistance1, 1, true);
                    turn2(1, 5, "degree", -spin90, 1, true);
                }
                break;
            case -2:
            case -8:
                switch (endTemp)
                {
                case 2:
                case 10:
                    line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
                    line2(cSpeed, 4, pGL1, dGL2, "degree", miniDistance2, 1, true);
                    turn2(1, 5, "degree", spin90, 1, true);
                    line2(cSpeed, 4, pGL1, dGL2, "degree", 100, 6, false);
                    break;
                case 3:
                case 11:
                    if (liftParallel && currentPosition == 4)
                    {
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true);
                        line2(3, 3, pGL2, dGL2, "degree", miniDistance2, 1, true, longMotor, liftParallelSpeed, "degree", 250, true);
                        mediumMotor(longMotor, liftParallelSpeed, "degree", 250, true);
                        turn2(1, 5, "degree", -spin90, 1, true);
                        line2(1, 4, pGL1, dGL2, "degree", 100, 3, false);
                        liftParallel = false;
                    }
                    else if (liftParallel)
                    {
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 4, pGL2, dGL2, "degree", 280, 1, true);
                        mediumMotor(longMotor, liftParallelSpeed, "degree", 250, true);
                        turn1(motor_left, -1, false, -4, "degree", turn90, -1, true);
                        line2(1, 4, pGL1, dGL2, "degree", 150, 3, false);
                        liftParallel = false;
                    }
                    else
                    {
                        line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
                        line2(cSpeed, 4, pGL1, dGL2, "degree", miniDistance2, 1, true);
                        turn2(1, 5, "degree", -spin90, 1, true);
                        line2(cSpeed, 4, pGL1, dGL2, "degree", 100, 6, false);
                    }
                    break;
                case 9:
                case 1:
                    line2(cSpeed, 4, pGL2, dGL2, "degree", 300, 4, false);
                    turn1(motor_left, cSpeed, true, 4, "degree", 475, 4, false);
                    move(cSpeed, 4, 1, 1, "degree", 75, 4, false);
                    turn1(motor_left, cSpeed, true, 4, "degree", 475, 4, false);
                    line2(cSpeed, 4, pGL2 * 0.7, dGL2 * 0.7, "degree", 100, 4, false);
                    break;
                }
                break;
            case -3:
            case -9:
                switch (endTemp)
                {
                case 12:
                case 4:
                    line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
                    line2(cSpeed, 4, pGL2, dGL2, "degree", miniDistance2, 1, true);
                    turn2(1, 5, "degree", -spin90, 1, true);
                    line2(cSpeed, 4, pGL2, dGL2, "degree", 150, 4, false);
                    break;
                case 3:
                case 11:
                    line2(cSpeed, 4, pGL2, dGL2, "crossline", 0, 4, false);
                    line2(cSpeed, 4, pGL2, dGL2, "degree", miniDistance2, 1, true);
                    turn2(1, 5, "degree", spin90, 1, true);
                    line2(cSpeed, 4, pGL2, dGL2, "degree", 150, 4, false);
                    break;
                case 9:
                    line2(4, 4, pGL2, dGL2, "degree", 270, 4, false);
                    move(cSpeed, 3, 1, 0.42, "degree", 1720, 1, false); //M6
                    break;
                case 14:
                    line2(4, 4, pGL2, dGL2, "degree", 270, 4, false);
                    move(cSpeed, 3, 0.42, 1, "degree", 1720, 1, false);
                    break;
                }
                break;
            case -4:
            case -10:
                switch (endTemp)
                {
                case 4:
                case 12:
                    if (liftParallel && currentPosition == 3)
                    {
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false, longMotor, liftParallelSpeed, "degree", 250, true);
                        line2(3, 3, pGL2, dGL2, "degree", miniDistance2, 1, true, longMotor, liftParallelSpeed, "degree", 250, true);
                        mediumMotor(longMotor, liftParallelSpeed, "degree", 250, true);
                        turn2(1, 5, "degree", spin90, 1, true);
                        line2(1, 4, pGL1, dGL2, "degree", 100, 3, false);
                        liftParallel = false;
                    }
                    else if (liftParallel)
                    {
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 4, pGL2, dGL2, "degree", 280, 1, true);
                        mediumMotor(longMotor, liftParallelSpeed, "degree", 250, true);
                        turn1(motor_right, -1, false, -4, "degree", turn90, -1, true);
                        line2(1, 4, pGL1, dGL2, "degree", 150, 3, false);
                        liftParallel = false;
                    }
                    else
                    {
                        line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                        line2(3, 3, pGL2, dGL2, "degree", miniDistance2, 1, true);
                        turn2(1, 5, "degree", spin90, 1, true);
                        line2(cSpeed, 4, pGL1, dGL2, "degree", 100, 3, false);
                    }
                    break;
                case 5:
                case 13:
                    line2(1, 3, pGL2, dGL2, "crossline", 0, 3, false);
                    line2(3, 3, pGL2, dGL2, "degree", miniDistance2, 1, true);
                    turn2(1, 5, "degree", -spin90, 1, true);
                    line2(cSpeed, 4, pGL1, dGL2, "degree", 100, 3, false);
                    break;
                case 6:
                case 14:
                    line2(cSpeed, 4, pGL2, dGL2, "degree", 300, 4, false);
                    turn1(motor_right, cSpeed, true, 4, "degree", 475, 4, false);
                    move(cSpeed, 4, 1, 1, "degree", 60, 4, false);
                    turn1(motor_right, cSpeed, true, 4, "degree", 475, 4, false);
                    line2(cSpeed, 4, pGL2 * 0.7, dGL2 * 0.7, "degree", 100, 4, false);
                    break;
                }
                break;
            }
        }
        else if (endPosition > 0)
        {
            //Dreht am Ende, falls die Orientierung noch nicht übereinstimmt
            if (direction(endPosition, endDirection, 16) != driveDirection)
            {
                turn2(1, 5, "degree", spin180, 1, true);
                cout << "turn to endPosition" << endl;
            }
        }
    }

    cout << "Exit Router-Kreis at: " << endPosition << " " << endDirection << endl;
}