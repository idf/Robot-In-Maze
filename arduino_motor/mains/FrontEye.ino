#include "FrontEye.h"
//public
FrontEye::FrontEye() {
  this->ultrasound = new Ultrasound(11, 13);
  this->sharpLong = new SharpIR(A0, 250, 95, LONG);
}
void FrontEye::init() {
  this->ultrasound->init();
}

int FrontEye::get_reading() {
  int dis = this->get_ulra_reading();
  dis = (dis+5)/10*10;
  return dis;
}

void FrontEye::test_readings() {
  int dis= this->get_ulra_reading();
  Serial.print("Ultrasound Distance: ");
  Serial.println(dis);
  
  int dis1 = this->get_ir_reading();  // this returns the distance to the object you're measuring
  Serial.print("SharpIR distance long: ");  // returns it to the serial monitor
  Serial.println(dis1);
}

// private
int FrontEye::get_ulra_reading() {
  const int ULTRA_OFFSET = 2;
  return this->ultrasound->dist()-ULTRA_OFFSET;
}
int FrontEye::get_ir_reading() {
  const int IR_OFFSET = 1;
  return this->sharpLong->distance()-IR_OFFSET;
}