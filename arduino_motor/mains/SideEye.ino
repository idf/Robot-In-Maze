#include "SideEye.h"
//public
SideEye::SideEye() {
  this->sharp_left = new SharpIR(A3, 250, 95, SHORT);//left side
  this->sharp_right = new SharpIR(A2, 250, 95, SHORT);//right front
}
void SideEye::init() {
 
}

int SideEye::get_reading_right() {
  int dis = this->sharp_right->distance();
  delay(10);
  int dis1 = this->sharp_right->distance();

  if (this->is_within_range(this->sharp_right)) {
    dis = (dis+5)/10*10;
  }
  else {
    dis = -1;
  }
  return dis;
}

void SideEye::test_readings() {
  int dis1 = this->sharp_right->distance();  // this returns the distance to the object you're measuring
  Serial.print("Right SharpIR distance: ");  // returns it to the serial monitor
  Serial.println(dis1);
}

bool SideEye::is_within_range(SharpIR* sensor) {
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>49 || distance<0)
      return false;
    delay(20);
  }
  return true;
}
