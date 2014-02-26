#include "FrontEye.h"
FrontEye::FrontEye() {
  this->ultrasound = new Ultrasound(11, 13);
  this->sharpLong = new SharpIR(A0, 25, 93, LONG);
}
void FrontEye::init() {
  this->ultrasound->init();
}

int FrontEye::get_reading() {
  return 0;
}

void FrontEye::test_readings() {
  int dis= this->ultrasound->dist();
  Serial.print("Ultrasound Distance: ");
  Serial.println(dis);
  
  int dis1 = this->sharpLong->distance();  // this returns the distance to the object you're measuring
  Serial.print("Mean distance long: ");  // returns it to the serial monitor
  Serial.println(dis1);
}
