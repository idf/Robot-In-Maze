#include "SideEye.h"
//public
SideEye::SideEye(unsigned char left_pin, unsigned char right_pin) {
  this->sharp_left = new SharpIR(left_pin, 250, 95, SHORT);//left side
  this->sharp_right = new SharpIR(right_pin, 250, 95, LONG);//right front
}
void SideEye::init() {
 
}

int SideEye::output_reading_left() {
  return this->output_reading(this->sharp_left, SHORT);
}

int SideEye::output_reading_right() {
  return this->output_reading(this->sharp_right, LONG);
}

void SideEye::test_readings() {
  int dis = this->sharp_right->distance();  // this returns the distance to the object you're measuring
  Serial.print(F("Right SharpIR distance: "));  Serial.println(dis);
  dis = this->sharp_left->distance();
  Serial.print(F("Left SharpIR distance: ")); Serial.println(dis);

}

// private
bool SideEye::is_within_range(SharpIR* sensor, int model) {
  int upper, lower;
  if(model==SHORT){
    upper = 49;
    lower = 5;
  }
  else {
    upper = 120;
    lower = 30;
  }
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>upper || distance<lower) // TODO
      return false;
    delay(20);
  }
  return true;
}

int SideEye::output_reading(SharpIR* sensor, int model) {
  int OFFSET;
  if (model==SHORT) {
    OFFSET = 0;
  }
  else {
    OFFSET = -1;
  }

  int dis = sensor->distance() - OFFSET;
  if (this->is_within_range(sensor, model)) {
    dis = (dis+5)/10*10; 
  }
  else {
    dis = -1;
  }
  return dis;
}
