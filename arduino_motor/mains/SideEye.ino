#include "SideEye.h"
//public
SideEye::SideEye(unsigned char left_pin, unsigned char right_pin) {
  this->sharp_left = new SharpIR(left_pin, 250, 95, SHORT);//left side
  this->sharp_right = new SharpIR(right_pin, 250, 95, SHORT);//right front
}
void SideEye::init() {
 
}

int SideEye::get_reading_right() {
  return this->get_reading(this->sharp_right);
}

int SideEye::get_reading_left() {
  return this->get_reading(this->sharp_left);
}

void SideEye::test_readings() {
  int dis = this->sharp_right->distance();  // this returns the distance to the object you're measuring
  Serial.print(F("Right SharpIR distance: "));  Serial.println(dis);
  dis = this->sharp_left->distance();
  Serial.print(F("Left SharpIR distance: ")); Serial.println(dis);
  Serial.print(F("Get Right Reading: ")); Serial.println(this->get_reading_right());
  Serial.print(F("Get Left Reading: ")); Serial.println(this->get_reading_left());
}

// private
bool SideEye::is_within_range(SharpIR* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>49 || distance<5) // TODO
      return false;
    delay(20);
  }
  return true;
}

int SideEye::get_reading(SharpIR* sensor) {
  const int OFFSET = 0;
  int dis = sensor->distance() - OFFSET;
  if (this->is_within_range(sensor)) {
    dis = (dis+5)/10*10; 
  }
  else {
    dis = -1;
  }
  return dis;
}
