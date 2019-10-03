#include "blocks.h"
#include "timer.h"
#include "stalldetection.h"
// ACTION BLOCKS
//-----------------------------------------------------------------------------

//Drehung mit einem Rad
void turn1(motor_port_t turnMotor, int startSpeed, bool brakeOtherMotor, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop)
{
  bool dec = false;

  resetMotors();

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

  double togo;
  int OMbrakeDistance = 0;
  int OMaccDistance = 0;
  if (brakeOtherMotor)
  {
    OMbrakeDistance = abs(startSpeed - endSpeed) / (bfTurn1 + 1);
  }

  if (mode == "degree")
  {
    wert = abs(wert); //0.99

    if (stop == false)
    {
      OMaccDistance = (abs(endSpeed) - speedLevel(1));
    }
    wert = wert + OMbrakeDistance + OMaccDistance;
    togo = wert;
    cout << "togo: " << togo << endl;
    dec = true;
  }

  Stopwatch move;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  bool continueMove = true; //Bedingung zum Beenden der Bewegung
  while (continueMove)
  {
    tslp_tsk(1);
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
    //motorControl(turnMotor,cSpeed, maxSpeed);

    if (turnMotor == motor_right)
      ev3_motor_set_power(turnMotor, cSpeed);
    else
      ev3_motor_set_power(turnMotor, cSpeed * (-1));

    //Abbremsen otherMotor am Anfang
    if (OMbrakeDistance - abs(ev3_motor_get_counts(otherMotor) - resetOtherMotor) >= 0 && brakeOtherMotor == true) //Falls die verbleibende Bremsstrecke noch größer Null ist
    {
      double otherSpeed = (abs(startSpeed) - (abs(ev3_motor_get_counts(otherMotor) - resetOtherMotor) / OMbrakeDistance) * abs(startSpeed - endSpeed)) * (maxSpeed / abs(maxSpeed));
      //(Startspeed - prozentual zurückgelegte Bresmstrecke * zu bremsende Geschwindidkeit) * vorwärts oder rückwärts aus maxSpeed
      motorControl(otherMotor, otherSpeed, maxSpeed);
      cout << "DEC: " << otherSpeed << endl;
    }

    if (stop == false) //Beschleunigen otherMotor am Ende
    {
      double otherSpeed = speedLevel(1) + (OMaccDistance - 0 - togo); // * (maxSpeed / abs(maxSpeed))

      //Beschelunigt direkt proportional zur verbleienden Strecke des anderen Motors
      if (otherSpeed >= speedLevel(1))
      {
        cout << "ACC: " << otherSpeed << endl;
        motorControl(otherMotor, otherSpeed, maxSpeed);
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
    else
    {
      ev3_motor_stop(otherMotor, true);
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
  if (wert > 0)
  {
    startSpeed = startSpeed * (-1);
    maxSpeed = maxSpeed * (-1);
    cSpeed = cSpeed * (-1);
    endSpeed = endSpeed * (-1);
  }
  resetMotors();
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  int pCorrection = 0;
  Stopwatch move;

  bool dec = false;
  double togo;
  if (mode == "degree")
  {
    //wert = 0.98 * wert * (wheelCircumferance) / wheelDiameter; //0.98
    togo = abs(wert);
    dec = true;
  }

  bool continueMove = true;
  while (continueMove)
  {
    tslp_tsk(1);
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

    cSpeed = accDec(togo, bfTurn2, afTurn2, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);

    pCorrection = pGainTurn2 * (measureMotorRight() - measureMotorLeft());
    ev3_motor_set_power(motor_left, cSpeed + pCorrection);
    ev3_motor_set_power(motor_right, cSpeed - pCorrection);
  }
  brake(stop, endSpeed);
  resetMotors("degree", wert, wert * (-1), maxSpeed);
}

//Add ratio to fulfill arcs
/* 
//Bewegungsblock um mit Rotationssensoren geradeaus zu fahren (verschiedene Endbedingungen)
int move(int startSpeed, int maxSpeed, std::string mode, double wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  bool dec = false; //Soll prinzipiell nicht abbremsen
  double togo = 0;
  if (mode == "degree")
  {
    dec = true;
    togo = wert;
  }
  Stopwatch move;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;
  resetMotors();

  int colorCounter[40] = {0};

  for (int i = 0; i < 40; i++){
    colorCounter[i] = -1;
  }
  

  bool continueMove = true;

  int cCounter = 0;

  while (continueMove)
  {
    tslp_tsk(1);
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
      colorCounter[cCounter] = colorDetection_rgb(searchSensor, searchMode);
      cCounter++;
      tslp_tsk(4);
    }
  }

  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter, searchMode);
  }
  return -1;
}
*/

int move(int startSpeed, int maxSpeed, double leftRatio, double rightRatio, std::string mode, double wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  bool dec = false; //Soll prinzipiell nicht abbremsen
  double togo = 0;
  if (mode == "degree")
  {
    dec = true;
    togo = wert;
  }
  Stopwatch move;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;
  resetMotors();

  //MediumMotor Option
  StallDetection stall;
  stall.init(100);
  ev3_motor_reset_counts(mediumMotor);
  bool continueMediumMotor = true;
  if (mediumMotor == longMotor)
  {
    mediumMotorSpeed = mediumMotorSpeed * (-1);
  }
  int stallCounter = 0;

  int colorCounter[40] = {0};

  for (int i = 0; i < 40; i++)
  {
    colorCounter[i] = -1;
  }

  bool continueMove = true;

  int cCounter = 0;
  while (continueMove)
  {
    tslp_tsk(1);
    if (mode == "degree")
    {
      if (leftRatio == 1)
      {
        continueMove = abs(measureMotorLeft()) <= wert;
        togo = wert - abs(measureMotorLeft());
      }
      else
      {
        continueMove = abs(measureMotorRight()) <= wert;
        togo = wert - abs(measureMotorRight());
      }
    }
    else if (mode == "time")
    {
      continueMove = move.getTime() <= wert;
    }
    else
    {
      continueMove = lineDetection(mode) == false;
    }

    if (continueMediumMotor && abs(mediumMotorSpeed) > 0)
    {
      int motorCounts = abs(ev3_motor_get_counts(mediumMotor));
      stallCounter++;
      if (stallCounter == 10){
        stallCounter = 0;
        stall.measure(motorCounts);
      }
       
      if (mediumMotorMode == "degree")
      {
        continueMediumMotor = motorCounts < mediumMotorWert;
      }
      else
      {
        continueMediumMotor = move.getTime() < mediumMotorWert;
      }

      if (stall.detectStall() && move.getTime() > 200)
      {
        continueMediumMotor = false;
        ev3_speaker_play_tone(NOTE_C4, 30);
      }

      ev3_motor_set_power(mediumMotor, mediumMotorSpeed);
    }
    else
    {
      ev3_motor_stop(mediumMotor, mediumMotorStop);
    }

    cSpeed = accDec(togo, bfMove, afMove, move.getTime(), startSpeed, maxSpeed, endSpeed, dec);
    motorCorrection(pGain, cSpeed, resetRightDegree, resetLeftDegree, leftRatio, rightRatio);

    if (colorSearch)
    {
      colorCounter[cCounter] = colorDetection_rgb(searchSensor, searchMode);
      cCounter++;
      tslp_tsk(4);
    }
  }
  ev3_motor_stop(mediumMotor, true);
  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter, searchMode);
  }
  return -1;
}

//Geradeaus mit mediumMotoren
int move(int startSpeed, int maxSpeed, double leftRatio, double rightRatio, std::string mode, double wert, int endSpeed, bool stop, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  return move(startSpeed, maxSpeed, leftRatio, rightRatio, mode, wert, endSpeed, stop, false, HTr, " ", mediumMotor, mediumMotorSpeed,mediumMotorMode, mediumMotorWert, mediumMotorStop);
}

//Geradeaus ohne Farbscan-Option
int move(int startSpeed, int maxSpeed, double leftRatio, double rightRatio, std::string mode, double wert, int endSpeed, bool stop)
{
  return move(startSpeed, maxSpeed, leftRatio, rightRatio, mode, wert, endSpeed, stop, false, HTr, " ", doubleLever,0," ", 0, true);
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  resetMotors();

  Stopwatch move;
  Stopwatch slowDown;
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;
  if (startSpeed <= speedLevel(1))
  {
    //align(100);
  }
  bool dec = false;
  if (mode == "degree")
  {
    dec = true;
  }
  double lastpError = 0.0;
  bool continueMove = true;
  int colorCounter[40] = {-1};
  for (int i = 0; i < 40; i++)
  {
    colorCounter[i] = -1;
  }
  int cCounter = 0;
  bool resetSlowDown = false;
  double temporalMaxSpeed = cSpeed;
  double lastpErrors[4] = {0};
  int i = 0;
  double slowDownReset = 0;

  //MediumMotor Option
  StallDetection stall;
  stall.init(100);
  ev3_motor_reset_counts(mediumMotor);
  bool continueMediumMotor = true;
  if (mediumMotor == longMotor)
  {
    mediumMotorSpeed = mediumMotorSpeed * (-1);
  }
  int stallCounter = 0;

  int counter = 0;
  while (continueMove)
  {
    counter++;   //Überprüfen der Schleifendurchläufe
    tslp_tsk(1); //Reduziert Schleifendurchläufe auf ca. 500 pro Sekunde
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
      if (continueMove == false)
      {
        ev3_speaker_play_tone(NOTE_F4, 3);
      }
    }

    if (continueMediumMotor && abs(mediumMotorSpeed) > 0)
    {
      int motorCounts = abs(ev3_motor_get_counts(mediumMotor));
      stallCounter++;
      if (stallCounter == 10){
        stallCounter = 0;
        stall.measure(motorCounts);
      }
      if (mediumMotorMode == "degree")
      {
        continueMediumMotor = motorCounts < mediumMotorWert;
      }
      else
      {
        continueMediumMotor = move.getTime() < mediumMotorWert;
      }

      if (stall.detectStall() && move.getTime() > 200)
      {
        continueMediumMotor = false;
        ev3_speaker_play_tone(NOTE_C4, 30);
      }

      ev3_motor_set_power(mediumMotor, mediumMotorSpeed);
    }
    else
    {
      ev3_motor_stop(mediumMotor, mediumMotorStop);
    }

    double pError = ev3_color_sensor_get_reflect(LSr) - ev3_color_sensor_get_reflect(LSl);

    //Bei zu großem Fehler bremst der Roboter ab, ansonsten wird accDec ganz normal ausgeführt
    if (abs(pError) > 20 && abs(cSpeed) > speedLevel(45))
    {
      if (resetSlowDown)
      {
        slowDownReset = slowDown.getTime();
        temporalMaxSpeed = (int)(cSpeed);
      }
      resetSlowDown = false;
      cSpeed = (int)(temporalMaxSpeed - (slowDown.getTime() - slowDownReset) * 0.25 - 0.5);
      startSpeed = (int)(cSpeed);
    }
    else
    {
      resetSlowDown = true;
      cSpeed = accDec(togo, bfLine, afLine, slowDown.getTime() - slowDownReset, startSpeed, maxSpeed, endSpeed, dec);
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
        colorCounter[cCounter] = colorDetection_rgb(searchSensor, searchMode);
        cCounter++;
      }
    }
    lastpError = lastpErrors[i];

    //cout << move.getTime() << " cSpeed: " << cSpeed << " p: " << pCorrection << " d: " << dCorrection << endl;
  }
  ev3_motor_stop(mediumMotor, mediumMotorStop);
  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);
  cout << "lc: " << counter / (move.getTime() / 1000) << endl;

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter, searchMode);
  }
  return -1;
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  return line2(startSpeed, maxSpeed, pGain, dGain, mode, wert, endSpeed, stop, colorSearch, searchSensor, searchMode, longMotor, 0, " ", 0, true);
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  return line2(startSpeed, maxSpeed, pGain, dGain, mode, wert, endSpeed, stop, false, LSl, " ", mediumMotor, mediumMotorSpeed, mediumMotorMode, mediumMotorWert, mediumMotorStop);
}

int line2(int startSpeed, int maxSpeed, double pGain, double dGain, std::string mode, int wert, int endSpeed, bool stop)
{
  return line2(startSpeed, maxSpeed, pGain, dGain, mode, wert, endSpeed, stop, false, LSl, " ", longMotor, 0, " ", 0, true);
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
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
  int colorCounter[40] = {-1};
  for (int i = 0; i < 40; i++)
  {
    colorCounter[i] = -1;
  }
  int cCounter = 0;
  bool resetSlowDown = false;
  double temporalMaxSpeed = cSpeed;
  double lastpErrors[4] = {0};
  int i = 0;
  double slowDownReset = 0;

  //MediumMotor Option
  StallDetection stall;
  stall.init(100);
  ev3_motor_reset_counts(mediumMotor);
  bool continueMediumMotor = true;
  if (mediumMotor == longMotor)
  {
    mediumMotorSpeed = mediumMotorSpeed * (-1);
  }
  int stallCounter = 0;

  int counter = 0;
  cout << "beginline1 : " << run.getTime() << endl;
  while (continueMove)
  {
    counter++;   //Überprüfen der Schleifendurchläufe
    tslp_tsk(1); //Reduziert Schleifendurchläufe
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
      if (continueMove == false)
      {
        ev3_speaker_play_tone(NOTE_F4, 3);
      }
    }

    if (continueMediumMotor && abs(mediumMotorSpeed) > 0)
    {
      int motorCounts = abs(ev3_motor_get_counts(mediumMotor));
      stallCounter++;
      if (stallCounter == 10){
        stallCounter = 0;
        stall.measure(motorCounts);
      }
       
      if (mediumMotorMode == "degree")
      {
        continueMediumMotor = motorCounts < mediumMotorWert;
      }
      else
      {
        continueMediumMotor = move.getTime() < mediumMotorWert;
      }

      if (stall.detectStall() && move.getTime() > 200)
      {
        continueMediumMotor = false;
        ev3_speaker_play_tone(NOTE_C4, 30);
      }

      ev3_motor_set_power(mediumMotor, mediumMotorSpeed);
    }
    else
    {
      ev3_motor_stop(mediumMotor, mediumMotorStop);
    }

    double pError = LSrMiddle - ev3_color_sensor_get_reflect(followSensor);
    if (rightEdge)
    {
      pError = pError * (-1);
    }

    //Bei zu großem Fehler bremst der Roboter ab, ansonsten wird accDec ganz normal ausgeführt
    if (abs(pError) > 15 && abs(cSpeed) > speedLevel(40))
    {
      if (resetSlowDown)
      {
        slowDownReset = slowDown.getTime();
        temporalMaxSpeed = (int)(cSpeed);
      }
      resetSlowDown = false;
      cSpeed = (int)(temporalMaxSpeed - (slowDown.getTime() - slowDownReset) * 0.25 - 0.5);
      startSpeed = cSpeed;
    }
    else
    {
      resetSlowDown = true;
      cSpeed = accDec(togo, bfLine, afLine, slowDown.getTime() - slowDownReset, startSpeed, maxSpeed, endSpeed, dec);
    }

    double pCorrection = pError * pGain;
    double dCorrection = (pError - lastpError) * dGain;

    ev3_motor_set_power(motor_left, (cSpeed - pCorrection - dCorrection) * (-1));
    ev3_motor_set_power(motor_right, cSpeed + pCorrection + dCorrection);

    //Speichert lastpError
    lastpErrors[i] = pError;
    i++;
    if (i > 3)
    {
      i = 0;
      if (colorSearch)
      {
        colorCounter[cCounter] = colorDetection_rgb(searchSensor, searchMode);
        cCounter++;
      }
    }
    lastpError = lastpErrors[i];

    //cout << move.getTime() << " cSpeed: " << cSpeed << " p: " << pCorrection << " d: " << dCorrection << endl;
  }
  cout << "endline1 : " << run.getTime() << endl;
  ev3_motor_stop(mediumMotor, true);
  brake(stop, endSpeed);
  resetMotors(mode, wert, wert, maxSpeed);
  cout << "lc: " << counter / (move.getTime() / 1000) << endl;

 

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter, searchMode);
  }
  return -1;
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, bool colorSearch, sensor_port_t searchSensor, std::string searchMode)
{
  return line1(startSpeed, maxSpeed, pGain, dGain, followSensor, rightEdge, mode, wert, endSpeed, stop, colorSearch, searchSensor, searchMode, longMotor, 0, " ", 0, true);
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop, motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  return line1(startSpeed, maxSpeed, pGain, dGain, followSensor, rightEdge, mode, wert, endSpeed, stop, false, HTr, " ", mediumMotor, mediumMotorSpeed, mediumMotorMode, mediumMotorWert, mediumMotorStop);
}

int line1(int startSpeed, int maxSpeed, double pGain, double dGain, sensor_port_t followSensor, bool rightEdge, std::string mode, int wert, int endSpeed, bool stop)
{
  return line1(startSpeed, maxSpeed, pGain, dGain, followSensor, rightEdge, mode, wert, endSpeed, stop, false, HTr, " ", longMotor, 0, " ", 0, true);
}

void mediumMotor(motor_port_t mediumMotor, int mediumMotorSpeed, std::string mediumMotorMode, int mediumMotorWert, bool mediumMotorStop)
{
  StallDetection stall;
  Stopwatch move;
  stall.init(100);
  ev3_motor_reset_counts(mediumMotor);
  bool continueMediumMotor = true;
  mediumMotorSpeed = mediumMotorSpeed * batteryFactor;
  if (mediumMotor == longMotor)
  {
    mediumMotorSpeed = mediumMotorSpeed * (-1);
  }
  int stallCounter = 0;

  while (continueMediumMotor)
  {
    int motorCounts = abs(ev3_motor_get_counts(mediumMotor));

    stallCounter++;
    if (stallCounter == 10){
      stall.measure(motorCounts);
      stallCounter = 0;
    }
      
    if (mediumMotorMode == "degree")
    {
      continueMediumMotor = motorCounts < mediumMotorWert;
    }
    else
    {
      continueMediumMotor = move.getTime() < mediumMotorWert;
    }

    if (stall.detectStall() && move.getTime() > 300)
    {
      continueMediumMotor = false;
      ev3_motor_stop(mediumMotor, mediumMotorStop);
      ev3_speaker_play_tone(NOTE_C4, 10);
    }

    ev3_motor_set_power(mediumMotor, mediumMotorSpeed);
  }
  ev3_motor_stop(mediumMotor, mediumMotorStop);
}