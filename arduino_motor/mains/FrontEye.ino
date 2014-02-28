#include "FrontEye.h"
//public
FrontEye::FrontEye(unsigned char ultra_pin_1, unsigned char ultra_pin_2, unsigned char ir_pin) {
  this->ultrasound = new Ultrasound(ultra_pin_1, ultra_pin_2);
  this->sharpLong = new SharpIR(ir_pin, 250, 95, LONG);
}
void FrontEye::init() {
  this->ultrasound->init();
}

int FrontEye::get_reading() {
  int dis = this->get_ulra_reading();
  if(this->is_within_range(this->ultrasound)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
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
  const int ULTRA_OFFSET = 1; 
  return this->ultrasound->dist()-ULTRA_OFFSET;
}
int FrontEye::get_ir_reading() {
  const int IR_OFFSET = 1;
  return this->sharpLong->distance()-IR_OFFSET;
}

bool FrontEye::is_within_range(SharpIR* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>120 || distance<30) // TODO
      return false;
    delay(20);
  }
  return true;
}

bool FrontEye::is_within_range(Ultrasound* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->dist();
    if(distance>120 || distance<5) // TODO
      return false;
    delay(20);
  }
  return true;
}
