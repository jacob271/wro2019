#include "blocks.h"
#include "timer.h"
#include "stalldetection.h"

// SUPPORT BLOCKS
//-----------------------------------------------------------------------------

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
  switch (abs(level))
  {
  case 1:
    return (10 * batteryFactor * (level / abs(level))); //Anfahren speed
  case 2:
    return (50 * batteryFactor * (level / abs(level)));
  case 3:
    return (70 * batteryFactor * (level / abs(level))); //Standard Drive Speed
  case 4:
    return (100 * batteryFactor * (level / abs(level))); //fast drive speed
  case 5:
    return (80 * batteryFactor * (level / abs(level))); //turn2 speed
  default:
    return (level * batteryFactor);
  }
}

// Rotationsssensorkorrektur für movees
void motorCorrection(double pGain, double cSpeed, int rightreset, int leftreset)
{
  double pCorrection;
  pCorrection = ((ev3_motor_get_counts(motor_left) - leftreset) + (ev3_motor_get_counts(motor_right) - rightreset)) * (abs(pGain * cSpeed) + 0.4);
  ev3_motor_set_power(motor_left, (-1) * (cSpeed + pCorrection));
  ev3_motor_set_power(motor_right, cSpeed - pCorrection);
}

double accDec(int togo, double brakeFactor, int accFactor, Stopwatch move, double startSpeed, int maxSpeed, int endSpeed, bool dec)
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
      cSpeed = std::max(maxSpeed, (int)(startSpeed - move.getTime() / accFactor));
    }
    else
    {
      cSpeed = std::min(maxSpeed, (int)(startSpeed + move.getTime() / accFactor));
    }
  }
  else if (abs(cSpeed) > abs(maxSpeed))
  {
    if (maxSpeed < 0)
    {
      cSpeed = std::max(maxSpeed, (int)(startSpeed + move.getTime() / accFactor));
    }
    else
    {
      cSpeed = std::min(maxSpeed, (int)(startSpeed - move.getTime() / accFactor));
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
    ev3_motor_stop(motor_left, true);
    ev3_motor_set_power(motor_right, 0);
    ev3_motor_stop(motor_right, true);
    cSpeed = 0;
  }
  else
  {
    cSpeed = endSpeed;
  }
}

// Auswerten der Häufigkeitsverteilung für das Erkennen von Schiffen und Lagern
int frequencyDistribution(int colorCounter[])
{
  int temp = 1;
  for (int i = 2; i < 8; i++)
  {
    if (colorCounter[i] > colorCounter[temp] && colorCounter[i] > 1)
      temp = i;
    std::cout << i << "-" << colorCounter[i] << " | ";
  }

  std::cout << "Detected color is " << temp << " and was seen " << colorCounter[temp] << " times." << std::endl;
  return temp;
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
void display(int inhalt){
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

  tslp_tsk(7);

  std::cout << "C:" << rgb.r << " " << rgb.g << " " << rgb.b << " ";
  if (mode == "color")
  {
    if (red < 5 && blue < 5 && green < 5)
      return 0;
    if (red > blue && red > green && red > 10)
    {
      if (red > green * 2)
        return 5;
      else
      {
        return 4;
      }
    }
    if (blue > green && blue > red && blue > 10)
      return 2;
    if (green > blue && green > red && green > 5)
      return 3;
  }
  else if (mode == "bw")
  {
    if (red < 10 && green < 10 && blue < 10)
      return 0;
    if (red > 150 && green > 150 && blue > 150)
      return 7;
    else
    {
      return 1;
    }
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
    std::cout << "lastColor: " << 14 - color << " ";
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
    std::cout << "lastColor: " << 9 - color << " ";
    //Summe der Farben ist immer 9
    return 9 - color;
  }
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