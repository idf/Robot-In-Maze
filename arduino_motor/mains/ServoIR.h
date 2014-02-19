#ifndef SERVOIR_H
#define SERVOIR_H
#include <Arduino.h>
#include <Servo.h> // get from dropbox
#include <SharpIR.h> // get from dropbox

class ServoIR
{
public:
  ServoIR(unsigned char servoPin, unsigned char irPin);
  void init();
  void servoScan();
  void servoScan(int turning_angle);

  void print_dist_array();

  int distArray[18];
private:
  unsigned char servoPin;
  unsigned char irPin;
  Servo* servo; 
  SharpIR* sharpShort; 
};

#endif