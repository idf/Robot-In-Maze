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
  int dis1 = this->sharp_right->distance();  // this returns the distance to the object you're measuring
  Serial.print("Right SharpIR distance: ");  // returns it to the serial monitor
  Serial.println(dis1);
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
  int dis = sensor->distance();
  if (this->is_within_range(sensor)) {
    dis = (dis+5)/10*10;
  }
  else {
    dis = -1;
  }
  return dis;
}
