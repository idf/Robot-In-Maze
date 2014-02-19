#ifndef SERVOIR_H
#define SERVOIR_H
#include <Arduino.h>
#include <Servo.h> // get from dropbox
#include <SharpIR.h> // get from dropbox

#define ARRAY_SIZE 36
class ServoIR
{
public:
  ServoIR(unsigned char servoPin, unsigned char irPin);
  void init();
  void servoScan();
  void servoScan(int starting_angle, int turning_angle);

  void print_dist_array();

  int distArray[ARRAY_SIZE];
private:
  void servo_write_adjusted(int angle);


  unsigned char servoPin;
  unsigned char irPin;
  Servo* servo; 
  SharpIR* sharpShort;

  void reset_array();
  double read_accurate_distance();
};

#endif