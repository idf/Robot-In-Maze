#include "SideEye.h"
//public
SideEye::SideEye(unsigned char left_pin, unsigned char right_pin, unsigned char ultra_pin_1, unsigned ultra_pin_2) {
  this->sharp_left = new SharpIR(left_pin, 50, 95, SHORT);//left side
  this->sharp_right = new SharpIR(right_pin, 50, 95, SHORT);//right front

  this->ultrasound = new Ultrasound(ultra_pin_1, ultra_pin_2);
}
void SideEye::init() {
  this->ultrasound->init();
}

int SideEye::output_reading_left() {
  return this->_output_reading_left(this->sharp_left, SHORT);
}

int SideEye::output_reading_right() {
  return this->_output_reading_right(this->sharp_right, SHORT);
}

int SideEye::output_reading_ultra() {
  int dis = this->get_ultra_reading();
  if(this->is_within_range(this->ultrasound)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;
}

void SideEye::test_readings() {
  int dis = this->sharp_right->distance();  // this returns the distance to the object you're measuring
  Serial.print(F("Right SharpIR distance: "));  Serial.println(dis);
  dis = this->sharp_left->distance();
  Serial.print(F("Left SharpIR distance: ")); Serial.println(dis);
  dis = this->ultrasound->dist();
  Serial.print(F("Ultrasound distance: ")); Serial.println(dis);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// private
bool SideEye::is_within_range_left(SharpIR* sensor, int model) {
  int upper, lower;
  if(model==SHORT){
    upper = 39;
    lower = 4;
  }
  else {
    upper = 65;
    lower = 22;
  }
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>upper || distance<lower) 
      return false;
    delay(RANGE_TEST_DELAY); // TODO
  }
  return true;
}

bool SideEye::is_within_range_right(SharpIR* sensor, int model) {
  int upper, lower;
  if(model==SHORT){
    upper = 45;
    lower = 3;
  }
  else {
    upper = 65;
    lower = 22;
  }
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>upper || distance<lower) 
      return false;
    delay(RANGE_TEST_DELAY); // TODO
  }
  return true;
}

int SideEye::_output_reading_left(SharpIR* sensor, int model) {
  int OFFSET;
  if (model==SHORT) {
    OFFSET = 10;
  }
  else {
    OFFSET = 4  ; // for LONG
  }

  int dis = sensor->distance() - OFFSET;
  if (this->is_within_range_left(sensor, model)) {
    dis = (dis+5)/10*10; 
  }
  else {
    dis = -1;
  }
  return dis;
}

int SideEye::_output_reading_right(SharpIR* sensor, int model) {
  int OFFSET;
  if (model==SHORT) {
    OFFSET = 2;
  }
  else {
    OFFSET = 4  ; // for LONG
  }

  int dis = sensor->distance() - OFFSET;
  if (this->is_within_range_right(sensor, model)) {
    dis = (dis+5)/10*10; 
  }
  else {
    dis = -1;
  }
  return dis;
}


int SideEye::get_ultra_reading() {
  const int ULTRA_OFFSET = 4;// measured
  return this->ultrasound->dist()-ULTRA_OFFSET;
}

bool SideEye::is_within_range(Ultrasound* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->dist();
    if(distance>107 || distance<2) // TODO
      return false;
    delay(RANGE_TEST_DELAY);
  }
  //Serial.println(F("Ultrasound is_within_range")); //additional delay (wk8 Mon)
  return true;
}

