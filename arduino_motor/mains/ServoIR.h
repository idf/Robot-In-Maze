#ifndef SERVOIR_H
#define SERVOIR_H
#include <Arduino.h>
#include <Servo.h> // get from dropbox
#include <SharpIR.h> // get from dropbox

#define ARRAY_SIZE 36
/*
  Rotation angle in sensor's coordinates (Front Sensor)

  Robot Forward
  
  y

  |     
  |   /
  |  /
  | / theta
 _|/_______________x
 O|

*/

 /*
 Long distance sensor range: 30cm to 150cm
 Short distance range: 20cm to 80 cm
 */

class ServoIR
{
public:
  int distArray[ARRAY_SIZE];

  ServoIR(unsigned char servoPin, unsigned char irPin);
  void init();

  void servoScan();
  void servoScan(int starting_angle, int turning_angle);
  void print_dist_array();
  void servo_point_read(int angle);

private:
  unsigned char servoPin;
  unsigned char irPin;
  Servo* servo; 
  SharpIR* sharpShort;

  void reset_array();
  double read_accurate_distance();
  void servo_write_adjusted(int angle);
};

#endif