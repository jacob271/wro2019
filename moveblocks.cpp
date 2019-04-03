#include "blocks.h"
#include "timer.h"
#include "stalldetection.h"
// ACTION BLOCKS
//-----------------------------------------------------------------------------

//Drehung mit einem Rad
void turn1(motor_port_t turnMotor, int startSpeed, bool brakeOtherMotor, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop)
{
  bool dec = false;

  //Motoren und Variablen zum Zurücksetzen und Fahren festlegen
  motor_port_t otherMotor;
  int resetOtherMotor;
  int resetTurnMotor;
  if (turnMotor == motor_right)
  {
    resetTurnMotor = resetRightDegree;
    otherMotor = motor_left;
    resetOtherMotor = resetLeftDegree;
  }
  else
  {
    resetTurnMotor = resetLeftDegree;
    otherMotor = motor_right;
    resetOtherMotor = resetRightDegree;
  }

  //Winkel in Grad umrechnen
  double togo;
  int OMbrakeDistance = 0;
  int OMaccDistance = 0;
  if (brakeOtherMotor)
  {
    OMbrakeDistance = abs(startSpeed - endSpeed) / bfTurn1;
  }

  if (mode == "degree")
  {
    wert = abs(1 * wert * (wheelCircumferance * 2) / wheelDiameter); //0.99

    if (stop == false)
    {
      OMaccDistance = abs(endSpeed);
    }
    wert = wert + OMbrakeDistance + OMaccDistance;
    togo = wert;

    dec = true;
  }

  Stopwatch move;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  bool continueMove = true; //Bedingung zum Beenden der Bewegung
  while (continueMove)
  {

    togo = wert - abs(ev3_motor_get_counts(turnMotor) - resetTurnMotor);
    cSpeed = accDec(togo, bfTurn1, afMove, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);

    if (mode == "degree")
    {
      continueMove = togo > 0;
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() <= wert;
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }

    //Turnmotor ansteuern
    if (turnMotor == motor_right)
      ev3_motor_set_power(turnMotor, cSpeed);
    else
      ev3_motor_set_power(turnMotor, cSpeed * (-1));

    //Abbremsen otherMotor am Anfang
    if (OMbrakeDistance - abs(ev3_motor_get_counts(otherMotor) - resetOtherMotor) >= 0) //Falls die verbleibende Bremsstrecke noch größer Null ist
    {
      double otherSpeed = (abs(startSpeed) - (abs(ev3_motor_get_counts(otherMotor) - resetOtherMotor) / OMbrakeDistance) * abs(startSpeed - endSpeed)) * (maxSpeed / abs(maxSpeed));
      //(Startspeed - prozentual zurückgelegte Bresmstrecke * zu bremsende Geschwindidkeit) * vorwärts oder rückwärts aus maxSpeed

      if (otherMotor == motor_right)
        ev3_motor_set_power(otherMotor, otherSpeed);
      else
        ev3_motor_set_power(otherMotor, otherSpeed * -1);
    }
    else if (stop == false) //Beschleunigen otherMotor am Ende
    {
      double otherSpeed = (OMaccDistance - 0 - togo) * (maxSpeed / abs(maxSpeed));
      //Beschelunigt direkt proportional zur verbleienden Strecke des anderen Motors
      if (otherSpeed > 0)
      {
        if (otherMotor == motor_right)
          ev3_motor_set_power(otherMotor, otherSpeed);
        else
          ev3_motor_set_power(otherMotor, otherSpeed * -1);
      }
      else
      {
        ev3_motor_stop(otherMotor, true);
      }
    }
  }

  //Motoren entsprechend der Sollwerte zurücksetzen
  if (turnMotor == motor_right)
  {
    resetMotors("degree", OMbrakeDistance + OMaccDistance, (int)(wert), maxSpeed);
  }
  else
  {
    resetMotors("degree", (int)(wert), (OMbrakeDistance + OMaccDistance), maxSpeed);
  }
  brake(stop, endSpeed); //Motoren anhalten

  //display(counter / (move.getTime() / 1000.));
}

void turn2(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop)
{

  //Dreht sich nach rechts für wert größer null
  if (wert < 0)
  {
    startSpeed = startSpeed * (-1);
    cSpeed = cSpeed * (-1);
    endSpeed = endSpeed * (-1);
  }

  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  int pCorrection = 0;
  Stopwatch move;

  bool dec = false;
  double togo;
  if (mode == "degree")
  {
    wert = 1 * wert * (wheelCircumferance) / wheelDiameter; //0.98
    togo = abs(wert);
    dec = true;
  }

  bool continueMove = true;
  while (continueMove)
  {
    togo = abs(wert) - (abs(measureMotorLeft()) + abs(measureMotorRight())) / 2;
    if (mode == "degree")
    {
      continueMove = togo > 0;
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() <= abs(wert);
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }

    cSpeed = accDec(togo, bfTurn2, afMove, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);

    pCorrection = pGainTurn2 * (measureMotorRight() - measureMotorLeft());
    ev3_motor_set_power(motor_left, cSpeed + pCorrection);
    ev3_motor_set_power(motor_right, cSpeed - pCorrection);
  }
  brake(stop, endSpeed);
  resetMotors("degree", wert, wert * (-1), maxSpeed);
}

//Bewegungsblock um mit Rotationssensoren geradeaus zu fahren (verschiedene Endbedingungen)
int moveStraight(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  bool dec = false; //Soll prinzipiell nicht abbremsen
  double togo;
  if (mode == "degree")
  {
    dec = true;
    togo = wert;
  }
  Stopwatch move;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  int colorCounter[8] = {0};

  bool continueMove = true;

  while (continueMove)
  {
    if (mode == "degree")
    {
      continueMove = (abs(measureMotorRight() - measureMotorLeft()) / 2) <= wert;
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() <= wert;
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }

    cSpeed = accDec(togo, bfMove, afMove, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);
    motorCorrection(pGain, cSpeed, resetRightDegree, resetLeftDegree);

    if (colorSearch)
    {
      colorCounter[colorDetection_rgb(HTr, searchMode)]++;
      tslp_tsk(4);
    }
  }

  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

//Geradeaus ohne Farbscan-Option
int moveStraight(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop)
{
  moveStraight(startSpeed, maxSpeed, mode, wert, endSpeed, stop, false, HTr, " ");
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  Stopwatch move;
  bool dec = false;

  if (mode == "degree")
  {
    dec = true;
  }
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;
  double lastpCorrection = 0;
  bool continueMove = true;
  int colorCounter[8] = {0};

  while (continueMove)
  {
    int togo = wert - (abs(measureMotorRight() - measureMotorLeft()) / 2);

    if (mode == "degree")
    {
      continueMove = (abs(measureMotorRight() - measureMotorLeft()) / 2) <= wert;
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() < wert;
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }
    cSpeed = accDec(togo, bfMove, afMove, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);

    double pCorrection = ev3_color_sensor_get_reflect(LSr) - ev3_color_sensor_get_reflect(LSl);

    //std::cout << lLeft << " " << lRight << " " << lLeft + lRight << std::endl;

    display(pCorrection);
    ev3_motor_set_power(motor_left, (cSpeed - pCorrection * pGain - (pCorrection - lastpCorrection) * dGain) * (-1));
    ev3_motor_set_power(motor_right, cSpeed + pCorrection * pGain + (pCorrection - lastpCorrection) * dGain);
    lastpCorrection = pCorrection;

    if (colorSearch)
    {
      colorCounter[colorDetection_rgb(searchSensor, searchMode)]++;
      tslp_tsk(5);
    }
  }
  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop)
{
  return line2(startSpeed, maxSpeed, pGain, dGain, mode, wert, endSpeed, stop, false, LSl, " ");
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  Stopwatch move;
  Stopwatch slowDown;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;
  bool dec = false;
  if (mode == "degree")
  {
    dec = true;
  }
  double lastpError = 0.0;
  bool continueMove = true;
  int colorCounter[8] = {0};
  bool resetSlowDown = false;
  int temporalMaxSpeed;
  double lastpErrors[4] = {0};
  int i = 0;
  double slowDownReset = 0;

  int counter = 0;
  while (continueMove)
  {
    counter++;   //Überprüfen der Schleifendurchläufe
    tslp_tsk(1); //Reduziert Schleifendurchläufe auf 500 pro Sekunde
    int togo = wert - (abs(measureMotorRight() - measureMotorLeft()) / 2);

    if (mode == "degree")
    {
      continueMove = (abs(measureMotorRight() - measureMotorLeft()) / 2) <= wert;
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() < wert;
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }

    double pError = LSrMiddle - ev3_color_sensor_get_reflect(followSensor);
    if (rightEdge)
    {
      pError = pError * (-1);
    }

    //Bei zu großem Fehler bremst der Roboter ab, ansonsten wird accDec ganz normal ausgeführt
    if (abs(pError) > 15 && abs(cSpeed) > abs(maxSpeed * 0.3))
    {
      if (resetSlowDown)
      {
        slowDownReset = slowDown.getTime();
        temporalMaxSpeed = cSpeed;
      }
      resetSlowDown = false;
      cSpeed = temporalMaxSpeed - (slowDown.getTime() - slowDownReset) * 0.25 - 0.5;
      startSpeed = cSpeed;
    }
    else
    {
      resetSlowDown = true;
      cSpeed = accDec(togo, bfLine1, afLine1, slowDown.getTime() - slowDownReset, startSpeed, maxSpeed, endSpeed, dec);
    }

    //display(pError);

    double pCorrection = pError * pGain;
    double dCorrection = (pError - lastpError) * dGain;

    ev3_motor_set_power(motor_left, (cSpeed - pCorrection - dCorrection) * (-1));
    ev3_motor_set_power(motor_right, cSpeed + pCorrection + dCorrection);
    lastpErrors[i] = pError;

    i++;
    if (i > 3)
    {
      i = 0;
      if (colorSearch)
      {
        colorCounter[colorDetection_rgb(searchSensor, searchMode)]++;
        //tslp_tsk(5);
      }
    }
    lastpError = lastpErrors[i];

    std::cout << move.getTime() << " cSpeed: " << cSpeed << " p: " << pCorrection << " d: " << dCorrection << std::endl;

    
  }
  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);
  std::cout << "Schleifendurchläufe: " << counter / (move.getTime() / 1000) << std::endl;

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop)
{
  return line1(startSpeed, maxSpeed, pGain, dGain, followSensor, rightEdge, mode, wert, endSpeed, stop, false, LSl, " ");
}

void mediumMotor(motor_port_t motor, int speed, std::string mode, int wert, bool stop)
{
  ev3_motor_reset_counts(motor);
  Stopwatch move;
  bool continueMove = true;
  while (continueMove)
  {
    if (mode == "degree")
    {
      continueMove = abs(ev3_motor_get_counts(motor)) < wert;
    }
    else
    {
      continueMove = move.getTime() < wert;
    }
    ev3_motor_set_power(motor, speed);
  }
  ev3_motor_stop(motor, stop);
}

/*
//Paulturn mit Rotationssensoren
void paulturn_rot(int startSpeed, int maxSpeed, double angle, double leftratio, double rightratio, int endSpeed)
{
  angle = angle * (17.6) / wheelDiameter;
  double togo = abs(angle);
  initializeSpeeds(startSpeed,maxSpeed,endSpeed);
  cSpeed = startSpeed;
  StallDetection stall;

  int pCorrection = 0;
  Stopwatch move;
  bool bremst = false;

  while (togo > 0)
  {
    togo = abs(angle) - (abs(ev3_motor_get_counts(motor_left) - resetLeftDegree) + abs(ev3_motor_get_counts(motor_right) - resetRightDegree)) / 2;
    cSpeed = accDec(togo, bfTurn2, move, startSpeed, 35, maxSpeed, endSpeed, stall, bremst); //9
    stall.measure(abs(ev3_motor_get_counts(motor_left)));
    pCorrection = 1.5 * ((ev3_motor_get_counts(motor_right) - resetRightDegree) / rightratio - (ev3_motor_get_counts(motor_left) - resetLeftDegree) / leftratio);
    ev3_motor_set_power(motor_left, cSpeed * leftratio + pCorrection);
    ev3_motor_set_power(motor_right, cSpeed * rightratio - pCorrection);
  }
  brakeAtEnd(endSpeed); // Motoren anhalten

  resetRightDegree = ev3_motor_get_counts(motor_right);
  resetLeftDegree = ev3_motor_get_counts(motor_left);
}

*/