#include "blocks.h"
#include "timer.h"
#include "stalldetection.h"

// SUPPORT BLOCKS
//-----------------------------------------------------------------------------

//speedControl

void motorControl(motor_port_t motor, int speed, int maxSpeed){
  double pGain = 0;//1.5;// * speed/100;
  double dGain = 0;//1.5;//0.5 + 1 * speed/100;
  int cPower = abs(ev3_motor_get_power(motor));
  int error = abs(speed) - cPower;
  
  int newSpeed = speed + error*pGain + (speedControlLastError - error) * dGain;
  newSpeed = newSpeed * (maxSpeed/abs(maxSpeed));

  if (motor == motor_left){
      newSpeed = newSpeed * (-1);
  }

   ev3_motor_set_power(motor, newSpeed);

   speedControlD[speedControlIndex] = error;

    speedControlIndex++;
    if (speedControlIndex > 3)
    {
      speedControlIndex = 0;
    }
    speedControlLastError = speedControlD[speedControlIndex];


  //std::cout << speed << " " << cPower  <<" " << error << " " << newSpeed << std::endl;
}

void resetSpeedControl (){
  speedControlD[4] = {0};
  speedControlIndex = 0;
  speedControlLastError = 0.0;
}

// get RGB value of EV3 color sensor
int getRGB(sensor_port_t port, int color)
{
  rgb_raw_t val;
  ev3_color_sensor_get_rgb_raw(port, &val);
  switch (color)
  {
  case 1:
    return val.r;
  case 2:
    return val.g;
  case 3:
    return val.b;
  }
  return 0;
}

// get RGB value of HT NXT color sensor
int getHTRGB(sensor_port_t sensor, int mode)
{
  if (mode == 0)
  {
    uint8_t color = 0;
    bool_t valC = ht_nxt_color_sensor_measure_color(sensor, &color);
    assert(valC);
    return int(color);
  }
  else
  {
    rgb_raw_t rgb;
    bool_t valRgb = ht_nxt_color_sensor_measure_rgb(sensor, &rgb);
    assert(valRgb);
    switch (mode)
    {
    case 1:
      return rgb.r;
    case 2:
      return rgb.g;
    case 3:
      return rgb.b;
    }
  }
  return 0;
}

// Geschwindigkeiten konvertieren
int speedLevel(int level)
{
  /*switch (abs(level))
  {
  case 1:
    return (speedLevel1 * batteryFactor * (level / abs(level))); //Start und EndSpeed
  case 2:
    return (50 * batteryFactor * (level / abs(level))); //miniDistance
  case 3:
    return (60 * batteryFactor * (level / abs(level))); //Standard Drive Speed (move Straight/line follows)
  case 4:
    return (80 * batteryFactor * (level / abs(level))); //fast drive speed and turn1
  case 5:
    return (60 * batteryFactor * (level / abs(level))); //turn2 speed
  case 6:
    return (40 * batteryFactor * (level / abs(level))); //langsamer speed um router und kabel abzusetzen

  default:
    return (int)(level * batteryFactor);
  }
  */

//Normale speeds Wettbewerb 
 switch (abs(level))
  {
  case 1:
    return (int)((18 * batteryFactor) * (level / abs(level))); //Start und EndSpeed //14
  case 2:
    return (int)((46 * batteryFactor) * (level / abs(level))); //miniDistance //43
  case 3:
    return (int)((90 * batteryFactor) * (level / abs(level))); //Standard Drive Speed (move Straight/line follows) //60
  case 4:
    return (int)((76 * batteryFactor) * (level / abs(level))); //fast drive speed and turn1 //73
  case 5:
    return (int)((60 * batteryFactor) * (level / abs(level))); //turn2 speed //56
  case 6:
    return (int)((50* batteryFactor) * (level / abs(level))); //langsamer speed um router und kabel abzusetzen //50

  default:
    return (int)(abs(level) * batteryFactor * (level / abs(level)));
  }
  
  /*boosted
  switch (abs(level))
  {
  case 1:
    return (int)((19 * batteryFactor) * (level / abs(level))); //Start und EndSpeed //14
  case 2:
    return (int)((46 * batteryFactor) * (level / abs(level))); //miniDistance //43
  case 3:
    return (int)((65 * batteryFactor) * (level / abs(level))); //Standard Drive Speed (move Straight/line follows) //60
  case 4:
    return (int)((85 * batteryFactor) * (level / abs(level))); //fast drive speed and turn1 //73
  case 5:
    return (int)((90 * batteryFactor) * (level / abs(level))); //turn2 speed //53
  case 6:
    return (int)((50* batteryFactor) * (level / abs(level))); //langsamer speed um router und kabel abzusetzen //50

  default:
    return (int)(abs(level) * batteryFactor * (level / abs(level)));
  }
  */

}

// Rotationsssensorkorrektur für moves
void motorCorrection(double pGain, int cSpeed, int rightreset, int leftreset, double leftRatio, double rightRatio)
{
  double pCorrection;
  pCorrection = ( (ev3_motor_get_counts(motor_left) - leftreset)/leftRatio + (ev3_motor_get_counts(motor_right) - rightreset)/rightRatio) * (abs(pGain*70)); //*70    +0.4 *cSpeed
  
  motorControl(motor_left, (leftRatio * cSpeed + pCorrection), cSpeed);
  motorControl(motor_right, (rightRatio * cSpeed - pCorrection), cSpeed);
  //ev3_motor_set_power(motor_left, (-1) * speedControl((cSpeed + pCorrection),motor_left));
  //ev3_motor_set_power(motor_right, speedControl((cSpeed - pCorrection),motor_right));
}

double accDec(int togo, double brakeFactor, double accFactor, double zeit, int startSpeed, int maxSpeed, int endSpeed, bool dec)
{
  if (togo * brakeFactor + abs(endSpeed) <= abs(cSpeed) && dec == true)
  {
    cSpeed = togo * brakeFactor + abs(endSpeed);
    if (maxSpeed < 0)
      cSpeed = cSpeed * (-1);
  }
  else if (abs(cSpeed) < abs(maxSpeed))
  {
    if (maxSpeed < 0)
    {
      cSpeed = std::max(maxSpeed, (int)(startSpeed - zeit * accFactor));
    }
    else
    {
      cSpeed = std::min(maxSpeed, (int)(startSpeed + zeit * accFactor));
    }
  }
  else if (abs(cSpeed) > abs(maxSpeed))
  {
    if (maxSpeed < 0)
    {
      cSpeed = std::max(maxSpeed, (int)(startSpeed + zeit * accFactor));
    }
    else
    {
      //cSpeed = std::min(maxSpeed, (int)(startSpeed - zeit * accFactor));
    }
  }
  return cSpeed;
}

// Am Ende einer Bewegung bei Bedarf anhalten
void brake(bool stop, int endSpeed)
{
  if (stop == true)
  {
    ev3_motor_set_power(motor_left, 0);
    ev3_motor_set_power(motor_right, 0);
    ev3_motor_stop(motor_left, true);
    ev3_motor_stop(motor_right, true);
    Stopwatch brake;
    while (brake.getTime()<4){
      ev3_motor_stop(motor_left, true);
      ev3_motor_stop(motor_right, true);
    }
    cSpeed = 0;
  }
  else
  {
    if (abs(cSpeed) < abs(endSpeed))
    {
      cSpeed = endSpeed;
    }
    cSpeed = (int)(cSpeed);
  }
}

//Auswerten der Farberkennung
int frequencyDistribution(int colorCounter[], std::string mode)
{

  int laenge = 40;
  std::cout << "frequencyDistribution " << mode << " " << laenge << std::endl;
  int r = 0; //red
  int b = 0;  //blue/black
  int g = 0;  //green
  int y = 0;  //yellow
  int w = 0;  //white
  int ergebnis = -1;
  int ergebnisN = 0;

  if (mode == "color")
  {
    //Mögliche Werte: 2,3,4,5
    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 2)
        b++;
    }
    std::cout << "b: " << b << std::endl;

    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 3)
        g++;
    }
    std::cout << "g: " << g << std::endl;

    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 5)
        r++;
    }
    std::cout << "r: " << r << std::endl;

    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 4)
        y++;
    }
    std::cout << "y: " << y << std::endl;

    if (r > b && r > g && r > y){
      ergebnis = 5;
      ergebnisN = r;
    }else if(g > r && g > b && g > y){
      ergebnis = 3;
      ergebnisN = g;
    }else if(b > r && b > g && b > y){
      ergebnis = 2;
      ergebnisN = b;
    }else{
      ergebnis = 4;
      ergebnisN = y;
    }
  }
  else
  {
    //Mögliche Werte: 0,1
    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 0)
        b++;
    }
    std::cout << "b: " << b << std::endl;

    for (int i = 0; i < laenge; i++)
    {
      if (colorCounter[i] == 1)
        w++;
    }
    std::cout << "w: " << w << std::endl;

    if (b > w){
      ergebnis = 0;
      ergebnisN = b;
    }else{
      ergebnis = 1;
      ergebnisN = w;
    }
  }

  std::cout << "Detected color: " << ergebnis << " N: " << ergebnisN <<std::endl;

  return ergebnis;
}

// Meldet zurück, ob die Linie nach gewünschten Modus gesehen wurde
bool lineDetection(std::string mode)
{
  if (mode == "blackright")
    return ev3_color_sensor_get_reflect(LSr) < 40; // 25
  else if (mode == "blackleft")
    return ev3_color_sensor_get_reflect(LSl) < 40; // 25
  else if (mode == "blackboth")
    return ev3_color_sensor_get_reflect(LSr) < 40 && ev3_color_sensor_get_reflect(LSl) < 40;
  else if (mode == "blackone")
    return ev3_color_sensor_get_reflect(LSr) < 40 || ev3_color_sensor_get_reflect(LSl) < 40;
  else if (mode == "whiteboth")
    return ev3_color_sensor_get_reflect(LSr) > 50 || ev3_color_sensor_get_reflect(LSl) > 50;
  else if (mode == "whiteright")
    return ev3_color_sensor_get_reflect(LSr) > 50;
  else if (mode == "whiteleft")
    return ev3_color_sensor_get_reflect(LSl) > 50;
  else if (mode == "crossline")
    return (ev3_color_sensor_get_reflect(LSl) + ev3_color_sensor_get_reflect(LSr)) < 100;
  return false;
}

// Warten, bis die Taste nach links gedrückt wurde
void waitForButton()
{
  while (!ev3_button_is_pressed(ENTER_BUTTON))
  {
  }
}

//Wert auf Display anzeigen
void display(int inhalt)
{
  char buf[10];
  sprintf(buf, "%d", inhalt);
  ev3_lcd_draw_string(buf, 20, 50);
}

// Konvertierung der HiTechnic Farbwerte in normale EV3 Farbwerte
int colorDetection(sensor_port_t sensor)
{
  int htColor = getHTRGB(sensor, 0);
  std::cout << htColor << " ";
  switch (htColor)
  {
  case 0:
    return 0;
  case 1:
    return 0;
  case 2:
    return 2;
  case 3:
    return 2;
  case 4:
    return 3;
  case 5:
    return 4;
  case 6:
    return 4;
  case 7:
    return 5;
  case 8:
    return 5;
  case 9:
    return 5;
  case 10:
    return 5;
  //case 11:
  //return 2;
  case 13:
    return 3;
  case 14:
    return 5;
  case 17:
    return 0; //5
  default:
    //std::cout << "Keine Farbe erkannt: " << htColor << std::endl; ####
    return 0;
  }
}

// Kompliziertere Farbwerterkennung
int colorDetection_rgb(sensor_port_t sensor, std::string mode)
{
  rgb_raw_t rgb;
  bool_t valRgb = ht_nxt_color_sensor_measure_rgb(sensor, &rgb);
  assert(valRgb);

  int red = rgb.r;
  int green = rgb.g;
  int blue = rgb.b;

  tslp_tsk(6);

  std::cout << mode << " " << red << " " << green<< " " << blue << " ";
  if (mode == "color")
  {
    if (red < 20 && blue < 20 && green < 20)
      return -1;
    if (red > blue && red > green && red > 80)
    {
      if (red > green * 1.65)
        return 5;
      else
      {
        return 4;
      }
    }
    if (blue > green && blue > red && blue > 60)
      return 2;
    if (green > blue && green > red && green > 50)
      return 3;
  }
  else
  {
    //Spezialwünsche von Paul: -1 = kein Objekt, 1 = weiß, 0 = schwarz
    if (red < 16 && green < 16 && blue < 16)
      return -1;
    if (red > 150 && green > 150 && blue > 150)
      return 1;
    if (red > 16 && green > 16 && blue > 16)
      return 0;
  }
  return -1;
}

//Ergänzt das Array mit der letzten verbleibenden Farbe
int findColor(int colors[], std::string mode)
{
  if (mode == "positions")
  {
    int color = 0;
    for (int i = 0; i < 3; i++)
    {
      color = color + colors[i];
    }
    std::cout << "lastColor: " << 14 - color << std::endl;
    //Summe der Farben ist immer 14
    return 14 - color;
  }
  else if (mode == "router")
  {
    int color = 0;
    for (int i = 0; i < 2; i++)
    {
      color = color + colors[i];
    }
    std::cout << "lastColor: " << 1 - color << std::endl;
    //Summe der Farben ist immer 1
    return 1 - color;
  }
  return -1;
}

void initializeSpeeds(int &speed1, int &speed2, int &speed3)
{
  speed1 = speedLevel(speed1);
  speed2 = speedLevel(speed2);
  speed3 = speedLevel(speed3);
}

int measureMotorRight()
{
  return ev3_motor_get_counts(motor_right) - resetRightDegree;
}
int measureMotorLeft()
{
  return ev3_motor_get_counts(motor_left) - resetLeftDegree;
}

void resetMotors(std::string mode, int leftValue, int rightValue, int maxSpeed)
{
  if (mode == "degree")
  {
    if (maxSpeed > 0)
    {
      resetRightDegree = resetRightDegree + rightValue;
      resetLeftDegree = resetLeftDegree - leftValue;
    }
    else
    {
      resetRightDegree = resetRightDegree - rightValue;
      resetLeftDegree = resetLeftDegree + leftValue;
    }
  }
  else
  {
    resetLeftDegree = ev3_motor_get_counts(motor_left);
    resetRightDegree = ev3_motor_get_counts(motor_right);
  }
}

void resetMotors()
{
  resetMotors("total", 0, 0, 0);
}

//Richtet sich mit beiden Sensoren kurz an der Linie aus, damit er zu Beginn des Linienfolgers schon richtig steht.
void align(int duration)
{
  double pCorrection = 0;
  Stopwatch align;
  double alignpGain = 1.0;

  while (align.getTime() < duration)
  {
    pCorrection = ev3_color_sensor_get_reflect(LSr) - ev3_color_sensor_get_reflect(LSl);
    ev3_motor_set_power(motor_left, (-1) * (cSpeed - pCorrection * alignpGain));
    ev3_motor_set_power(motor_right, cSpeed + pCorrection * alignpGain);
  }
  brake(true, 0);
}