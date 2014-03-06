#include "FrontEye.h"
//public
FrontEye::FrontEye(unsigned char ultra_pin_1, unsigned char ultra_pin_2, unsigned char ir_pin_left, unsigned char ir_pin_right) {
  this->ultrasound = new Ultrasound(ultra_pin_1, ultra_pin_2);
  this->sharp_left = new SharpIR(ir_pin_left, 250, 95, SHORT);
  this->sharp_right = new SharpIR(ir_pin_right, 250, 95, SHORT);
}
void FrontEye::init() {
  this->ultrasound->init();
}

int FrontEye::output_reading_ultra() {
  int dis = this->get_ultra_reading();
  if(this->is_within_range(this->ultrasound)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;
}

int FrontEye::output_reading_ir_left() {
  int dis = this->get_ir_reading_left();
  if(this->is_within_range(this->sharp_left)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;
}

int FrontEye::output_reading_ir_right() {
  int dis = this->get_ir_reading_right();
  if(this->is_within_range(this->sharp_right)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;  
}

void FrontEye::test_readings() {
  int dis= this->ultrasound->dist();
  Serial.print("Ultrasound Distance: ");
  Serial.println(dis);
  
  int dis1 = this->sharp_left->distance();  // this returns the distance to the object you're measuring
  Serial.print("Left SharpIR distance: ");  // returns it to the serial monitor
  Serial.println(dis1);

  int dis2 = this->sharp_right->distance(); // this returns the distance to the object you're measuring
  Serial.print("Right SharpIR distance: ");  // returns it to the serial monitor
  Serial.println(dis2);

}

// private
int FrontEye::get_ultra_reading() {
  const int ULTRA_OFFSET = 4; 
  return this->ultrasound->dist()-ULTRA_OFFSET;
}
int FrontEye::get_ir_reading_left() {
  const int IR_OFFSET = 2; // TODO
  return this->sharp_left->distance()-IR_OFFSET;
}

int FrontEye::get_ir_reading_right() {
  const int IR_OFFSET = 2; // TODO
  return this->sharp_right->distance()-IR_OFFSET;
}

bool FrontEye::is_within_range(SharpIR* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>40 || distance<10) // TODO
      return false;
    delay(RANGE_TEST_DELAY);
  }
  return true;
}

bool FrontEye::is_within_range(Ultrasound* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->dist();
    if(distance>90 || distance<5) // TODO
      return false;
    delay(RANGE_TEST_DELAY);
  }
  return true;
}
