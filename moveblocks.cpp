#include "blocks.h"
#include "timer.h"
#include "stalldetection.h"
// ACTION BLOCKS
//-----------------------------------------------------------------------------

//Bewegungsblock um mit Rotationssensoren geradeaus zu fahren (verschiedene Endbedingungen)
int move(int startSpeed, int maxSpeed, std::string mode, double value, int endSpeed, bool stop, bool colorSearch)
{
  bool dec = false; //Soll prinzipiell nicht abbremsen
  double togo;
  if (mode == "degree")
  {
    dec = true;
    value = value * wheelConverter;
    togo = value;
  }
  Stopwatch moveTime; //Stopwatch um auf Zeit zu fahren und acceleration zu steuern
  initializeSpeeds(startSpeed, maxSpeed, endSpeed);
  cSpeed = startSpeed;

  int colorCounter[8] = {0};

  bool interrupt = false;
  while (interrupt == false)
  {
    if (mode == "degree")
    {
      interrupt = abs(measureMotorRight() + measureMotorLeft()) / 2 >= value;
    }
    else if (mode == "time")
    {
      interrupt = moveTime.getTime() >= value;
    }
    else
    {
      interrupt = lineDetection(mode);
    }

    cSpeed = accDec(togo, bfMove, afMove, moveTime, startSpeed, maxSpeed, endSpeed, dec);
    motorCorrection(pGain, cSpeed, resetRightDegree, resetLeftDegree);

    if (colorSearch)
    {
      colorCounter[colorDetection_rgb(LSaussen)]++;
      tslp_tsk(4);
    }
  }

  if (stop == true)
  {
    ev3_motor_set_power(motor_left, 0);
    ev3_motor_stop(motor_left, true);
    ev3_motor_set_power(motor_right, 0);
    ev3_motor_stop(motor_right, true);
    cSpeed = 0;
  }

  resetMotors(mode, value, value, maxSpeed);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}












//*

void turn1_rot(motor_port_t turnMotor, int startSpeed, int maxSpeed, double angle, int endSpeed)
{
  angle = 0.975 * angle * (17.6 * 2) / wheelDiameter; //0.98
  int resetTurnMotor;
  if (turnMotor == motor_right)
  {
    resetTurnMotor = resetRightDegree;
    if (maxSpeed > 0)
      resetRightDegree = resetRightDegree + (int)(angle);
    else
      resetRightDegree = resetRightDegree - (int)(angle);
  }
  else
  {
    resetTurnMotor = resetLeftDegree;
    if (maxSpeed > 0)
      resetLeftDegree = resetLeftDegree - (int)(angle);
    else
      resetLeftDegree = resetLeftDegree + (int)(angle);
  }
  motor_port_t othermotor;
  int othermotorreset;
  if (turnMotor == motor_right){
    ev3_motor_stop(motor_left, true);
    othermotor = motor_left;
    othermotorreset = resetLeftDegree;
  }
  else{
    ev3_motor_stop(motor_right, true);
    othermotor = motor_right;
    othermotorreset = resetRightDegree;
  }
  double togo = abs(angle);
  
  initializeSpeeds(maxSpeed,endSpeed,startSpeed);
  cSpeed = startSpeed;
  StallDetection stall;
    
  Stopwatch move;
  bool bremst = false;
  int counter = 0;
  int lasttime = 0;

  std::cout << "startspeed: " << startSpeed << std::endl;
  while (togo > 0) // && !stall.detectStall())
  {
    togo = abs(angle) - abs(ev3_motor_get_counts(turnMotor) - resetTurnMotor) + abs(ev3_motor_get_counts(othermotor)-othermotorreset);
    cSpeed = accDec(togo, bfTurn1, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst); //13
    if (turnMotor == motor_right)
      ev3_motor_set_power(turnMotor, cSpeed);
    else
      ev3_motor_set_power(turnMotor, cSpeed * (-1));
    counter++;
    if (endSpeed - 0-togo > 0){
        if (othermotor == motor_right)
        ev3_motor_set_power(othermotor, endSpeed-0-togo);
      else
        ev3_motor_set_power(othermotor, (endSpeed-0-togo) * (-1));
      ev3_speaker_play_tone(NOTE_F4,100);
    }
    else{      
      int distanceToBrake = (startSpeed) * (bfTurn1);
      int braketogo = distanceToBrake - abs(ev3_motor_get_counts(othermotor)-othermotorreset);
      if (braketogo >= 0)
      {
        ev3_motor_set_power(othermotor,braketogo * (bfTurn1));
        ev3_speaker_play_tone(NOTE_C5,100);
      }
    }
  }

  brakeAtEnd(endSpeed); // Motoren anhalten
  char buffer[10];
  itoa(counter / (move.getTime() / 1000.), buffer, 10);
  ev3_lcd_draw_string(buffer, 20, 50);
}







/*

//Drehung mit Rotationssensoren
void turn1_rotttttttttttttttttt(motor_port_t turnMotor, int startSpeed, int maxSpeed, double angle, int endSpeed)
{
  angle = 0.975 * angle * (17.6 * 2) / wheelDiameter; //0.98
  int resetTurnMotor;
  if (turnMotor == motor_right)
  {
    resetTurnMotor = resetRightDegree;
    if (maxSpeed > 0)
      resetRightDegree = resetRightDegree + (int)(angle);
    else
      resetRightDegree = resetRightDegree - (int)(angle);
  }
  else
  {
    resetTurnMotor = resetLeftDegree;
    if (maxSpeed > 0)
      resetLeftDegree = resetLeftDegree - (int)(angle);
    else
      resetLeftDegree = resetLeftDegree + (int)(angle);
  }
  motor_port_t othermotor;
  int othermotorreset;
  if (turnMotor == motor_right){
    ev3_motor_stop(motor_left, true);
    othermotor = motor_left;
    othermotorreset = resetLeftDegree;
  }
  else{
    ev3_motor_stop(motor_right, true);
    othermotor = motor_right;
    othermotorreset = resetRightDegree;
  }
  double togo = abs(angle);
  initializeSpeeds(startSpeed,maxSpeed,endSpeed);
  cSpeed = startSpeed;
  StallDetection stall;
  //if (turnMotor == motor_right)
  // cSpeed = cSpeed * (-1);
  
  //std::cout << "angle:  " << (int) (angle) << " " << std::endl;
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;
 


  //ev3_motor_reset_counts(turnMotor);
  //ev3_motor_stop(motor_right, true);
  //ev3_motor_stop(motor_left, true);
  Stopwatch move;
  bool bremst = false;
  int counter = 0;
  int lasttime = 0;
  while (togo > 0) // && !stall.detectStall())
  {
    togo = abs(angle) - abs(ev3_motor_get_counts(turnMotor) - resetTurnMotor) + abs(ev3_motor_get_counts(othermotor)-othermotorreset);
    cSpeed = accDec(togo, bfTurn1, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst); //13
    if (move.getTime() != lasttime)
    {
      stall.measure(abs(ev3_motor_get_counts(turnMotor)));
      if (stall.detectStall())
        break;
    }
    lasttime = move.getTime();
    if (turnMotor == motor_right)
      ev3_motor_set_power(turnMotor, cSpeed);
    else
      ev3_motor_set_power(turnMotor, cSpeed * (-1));
    counter++;
  }

  brakeAtEnd(endSpeed); // Motoren anhalten
  char buffer[10];
  itoa(counter / (move.getTime() / 1000.), buffer, 10);
  ev3_lcd_draw_string(buffer, 20, 50);
}

//Drehung auf zwei Rädern mit Rotationssensoren
void turn2_rot(int startSpeed, int maxSpeed, double angle, int endSpeed)
{

  // Rotationssensoren zurücksetzen

  //int leftdegree = ev3_motor_get_counts(motor_left);
  //int rightdegree = ev3_motor_get_counts(motor_right);

  angle = 0.98 * angle * (17.6) / wheelDiameter;
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
    cSpeed = accDec(togo, bfTurn2, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst); //9
    stall.measure(abs(ev3_motor_get_counts(motor_left)));
    pCorrection = 1.5 * ((ev3_motor_get_counts(motor_right) - resetRightDegree) - (ev3_motor_get_counts(motor_left) - resetLeftDegree));
    ev3_motor_set_power(motor_left, cSpeed + pCorrection);
    ev3_motor_set_power(motor_right, cSpeed - pCorrection);
  }
  brakeAtEnd(endSpeed); // Motoren anhalten
  if (maxSpeed > 0)
  {
    resetRightDegree = resetRightDegree + (int)(angle);
    resetLeftDegree = resetLeftDegree + (int)(angle);
  }
  else
  {
    resetRightDegree = resetRightDegree - (int)(angle);
    resetLeftDegree = resetLeftDegree - (int)(angle);
  }
  //std::cout << "angle:  " << (int) (angle) << " " << std::endl;
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;
}

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

//Move mit Rotationssensoren
int moveRotDegRatio(int startSpeed, int maxSpeed, int distance, double rratio, double lratio, int endSpeed, bool colorSearch)
{
  distance = distance * wheelConverter;
  initializeSpeeds(startSpeed,maxSpeed,endSpeed);
  cSpeed = startSpeed;
  //double localPGain = pGain * speedLevel(abs(maxSpeed));

  //int leftdegree = ev3_motor_get_counts(motor_left); // Rotationssensoren zurücksetzen
  //int rightdegree = ev3_motor_get_counts(motor_right);

  StallDetection stall;
  int togo = distance;
  int colorCounter[8] = {0}; // für Farberkennung der Schiffe
  Stopwatch move;
  double _pGain = pGain;

  int counter = 0;
  bool bremst = false;

  while (togo > 0)
  {
    tslp_tsk(1);

    counter++;
    togo = distance - (abs(ev3_motor_get_counts(motor_left) - resetLeftDegree) / lratio + abs(ev3_motor_get_counts(motor_right) - resetRightDegree) / rratio) / 2;
    cSpeed = accDec(togo, bfMove, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst);
    //if (togo / 3 + (minSpeed*0.5) <= maxSpeed && brake == true)
    double pCorrection;
    pCorrection = ((ev3_motor_get_counts(motor_left) - resetLeftDegree) / lratio + (ev3_motor_get_counts(motor_right) - resetRightDegree) / rratio) * (abs(_pGain * cSpeed) + 0.4);
    ev3_motor_set_power(motor_left, (-1) * (cSpeed + pCorrection) * lratio); //-100.
    ev3_motor_set_power(motor_right, (cSpeed - pCorrection) * rratio);
    int counter2 = counter / (move.getTime() / 1000);
    char msgbuf[10];
    sprintf(msgbuf, "count: %d", counter2);
    ev3_lcd_draw_string(msgbuf, 10, 10);
    if (colorSearch)
    {
      colorCounter[colorDetection_rgb(LSaussen)]++;
      tslp_tsk(4);
    }
  }

  brakeAtEnd(endSpeed); // Motoren anhalten

  resetRightDegree = ev3_motor_get_counts(motor_right);
  resetLeftDegree = ev3_motor_get_counts(motor_left);
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;

  // Durchschnittliche Schleifendurchläufe auf dem Display anzeigen
  char buffer[10];
  itoa(counter / (move.getTime() / 1000.), buffer, 10);
  ev3_lcd_draw_string(buffer, 20, 50);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

//Move mit Rotationssensoren und medium Motoren
int moveRotDegMediumMotor(int startSpeed, int maxSpeed, int distance, int endSpeed, bool colorSearch, motor_port_t motor, int speed, int degree, bool stop)
{
  distance = distance * wheelConverter;
  initializeSpeeds(startSpeed,maxSpeed,endSpeed);
  cSpeed = startSpeed;
  ev3_motor_reset_counts(motor);

  //int leftdegree = ev3_motor_get_counts(motor_left); // Rotationssensoren zurücksetzen
  //int rightdegree = ev3_motor_get_counts(motor_right);

  StallDetection stall;
  int togo = distance;
  int colorCounter[8] = {0}; // für Farberkennung der Schiffe
  Stopwatch move;

  int counter = 0;
  bool bremst = false;

  while (togo > 0)
  {
    tslp_tsk(1);

    counter++;
    togo = distance - (abs(ev3_motor_get_counts(motor_left) - resetLeftDegree) + abs(ev3_motor_get_counts(motor_right) - resetRightDegree)) / 2;
    cSpeed = accDec(togo, bfMove, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst);
    motorCorrection(pGain, 0, iCorrection, cSpeed, resetRightDegree, resetLeftDegree);
    ev3_motor_set_power(motor, speed);

    if (abs(ev3_motor_get_counts(motor)) > degree)
      ev3_motor_stop(motor, stop);

    if (colorSearch)
    {
      colorCounter[colorDetection(LSaussen)]++;
      tslp_tsk(4);
    }
  }

  brakeAtEnd(endSpeed); // Motoren anhalten

  updateRotationSensors(distance, maxSpeed);
  //std::cout<< "distance: " << distance << " "<< std::endl;
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;

  // Durchschnittliche Schleifendurchläufe auf dem Display anzeigen
  char buffer[10];
  itoa(counter / (move.getTime() / 1000.), buffer, 10);
  ev3_lcd_draw_string(buffer, 20, 50);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

//Move mit Rotationssensoren und medium Motoren auf Zeit
int moveRotDegMediumMotorTime(int startSpeed, int maxSpeed, int distance, int endSpeed, bool colorSearch, motor_port_t motor, int speed, int time, bool stop)
{
  distance = distance * wheelConverter;
  initializeSpeeds(startSpeed,maxSpeed,endSpeed);
  cSpeed = startSpeed;
  ev3_motor_reset_counts(motor);

  //int leftdegree = ev3_motor_get_counts(motor_left); // Rotationssensoren zurücksetzen
  //int rightdegree = ev3_motor_get_counts(motor_right);

  StallDetection stall;
  int togo = distance;
  int colorCounter[8] = {0}; // für Farberkennung der Schiffe
  Stopwatch move;

  int counter = 0;
  bool bremst = false;

  while (togo > 0)
  {
    tslp_tsk(1);

    counter++;
    togo = distance - (abs(ev3_motor_get_counts(motor_left) - resetLeftDegree) + abs(ev3_motor_get_counts(motor_right) - resetRightDegree)) / 2;
    cSpeed = accDec(togo, bfMove, move, startSpeed, minSpeed, maxSpeed, endSpeed, stall, bremst);
    motorCorrection(pGain, 0, iCorrection, cSpeed, resetRightDegree, resetLeftDegree);
    ev3_motor_set_power(motor, speed);

    if (move.getTime() > time)
      ev3_motor_stop(motor, stop);

    if (colorSearch)
    {
      colorCounter[colorDetection(LSaussen)]++;
      tslp_tsk(4);
    }
  }

  brakeAtEnd(endSpeed); // Motoren anhalten

  updateRotationSensors(distance, maxSpeed);
  //std::cout << "distance:  " << distance << " " << std::endl;
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;

  char buffer[10];
  itoa(counter / (move.getTime() / 1000.), buffer, 10);
  ev3_lcd_draw_string(buffer, 20, 50);

  if (colorSearch)
  {
    return frequencyDistribution(colorCounter);
  }
  return 0;
}

void mediumMotorDeg(motor_port_t motor, int speed, int degree, bool stop)
{
  ev3_motor_reset_counts(motor);
  while (abs(ev3_motor_get_counts(motor)) < degree)
  {
    ev3_motor_set_power(motor, speed);
  }
  ev3_motor_stop(motor, stop);
}

void mediumMotorTime(motor_port_t motor, int speed, int time, bool stop)
{
  Stopwatch mediumMotor;
  while (mediumMotor.getTime() < time)
  {
    ev3_motor_set_power(motor, speed);
  }
  ev3_motor_stop(motor, stop);
}

void twoMediumMotorsTime(int speed_kuhl, int time_kuhl, int speed_greif, int time_greif)
{
  Stopwatch kuhlMotor;
  Stopwatch greifMotor;
  ev3_motor_set_power(kuhlmotor, speed_kuhl);
  ev3_motor_set_power(tool2, speed_greif);
  while (kuhlMotor.getTime() < time_kuhl && greifMotor.getTime() < time_greif)
  {
    if (greifMotor.getTime() > time_greif)
      ev3_motor_stop(tool2, true);
    if (kuhlMotor.getTime() > time_kuhl)
      ev3_motor_stop(kuhlmotor, true);
  }
}

void old_align(int cSpeed, std::string mode)
{
  cSpeed = speedLevel(cSpeed);
  int rightSpeed = cSpeed;
  int leftSpeed = cSpeed;
  int leftline = 0;
  int rightline = 0;
  int rightmin = 42;
  int rightmax = 58;
  int leftmin = 42;
  int leftmax = 58;
  if (mode == "blue")
  {
    rightmin = 100;
    rightmax = 130;
  }
  Stopwatch align;
  bool linenotseen = true;

  while ((leftline < 10 || rightline < 10) && (align.getTime() < 800 || linenotseen == true))
  {
    int rightval = lightSensor(LSr, mode);
    if (rightval < rightmin)
    {
      rightline = 0;
      rightSpeed = cSpeed / abs(cSpeed) * 30;
      ev3_motor_set_power(motor_right, -rightSpeed);
      if (linenotseen)
      {
        linenotseen = false;
        align.reset();
      }
    }
    if (rightval > rightmin && rightval < rightmax)
    {
      ev3_motor_set_power(motor_right, 0);
      rightline++;
    }
    if (rightval > rightmax)
    {
      rightline = 0;
      ev3_motor_set_power(motor_right, rightSpeed);
    }
    int leftval = lightSensor(LSl, mode);
    if (leftval < leftmin)
    {
      leftline = 0;
      leftSpeed = cSpeed / abs(cSpeed) * 30;
      ev3_motor_set_power(motor_left, leftSpeed);
      if (linenotseen)
      {
        linenotseen = false;
        align.reset();
      }
    }
    if (leftval > leftmin && leftval < leftmax)
    {
      ev3_motor_set_power(motor_left, 0);
      leftline++;
    }
    if (leftval > leftmax)
    {
      leftline = 0;
      ev3_motor_set_power(motor_left, -leftSpeed);
    }
  }
  brakeAtEnd(0);
  resetRightDegree = ev3_motor_get_counts(motor_right);
  resetLeftDegree = ev3_motor_get_counts(motor_left);
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;
  //std::cout << "old align finished after " << align.getTime() / 1000 << " seconds" << std::endl;
}

// Abbremsen aus einer unbestimmten Geschwindigkeit, Rückgabe der benötigten Strecke
int brake()
{
  int maxSpeed = cSpeed;
  int startpos = abs(ev3_motor_get_counts(motor_left) + (-1) * ev3_motor_get_counts(motor_right)) / 2;
  double localPGain = pGain;
  //int degreeright = ev3_motor_get_counts(motor_right);
  //int degreeleft = ev3_motor_get_counts(motor_left);
  std::cout << "Bremse ab von Geschwindigkeit " << cSpeed << std::endl;
  int distanceToBrake = (cSpeed - minSpeed) / (bfMove * 2);
  int togo = distanceToBrake;
  double iCorrection = 0;
  while (togo >= 0)
  {
    cSpeed = std::max(int(togo * (bfMove * 1.3) + minSpeed), minSpeed);
    motorCorrection(pGain, cSpeed, rightreset, leftreset;
    togo = startpos + distanceToBrake - abs(ev3_motor_get_counts(motor_left) + (-1) * ev3_motor_get_counts(motor_right)) / 2;
  }

  brakeAtEnd(0);

  updateRotationSensors(distanceToBrake, maxSpeed);
  
  //std::cout << "distanceToBrake:  " << distanceToBrake << " " << std::endl;
  //std::cout << "motor_left:  " << resetLeftDegree << " " << std::endl;
  //std::cout << "motor_right:  " << resetRightDegree << " " << std::endl;

  std::cout << "Needed " << (ev3_motor_get_counts(motor_left) + ev3_motor_get_counts(motor_right)) / 2 - startpos << " to brake" << std::endl;
  return abs(ev3_motor_get_counts(motor_left) + (-1) * ev3_motor_get_counts(motor_right)) / 2 - startpos;
}

*/