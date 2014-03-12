#include "Calibrator.h"
//public 
Calibrator::Calibrator(FrontEye* frontEye) {
  this->frontEye = frontEye;
}
void Calibrator::calibrate(int situation) {
  this->turned_angle = 0; // clear
  const int CALIBRATE_TIMES = 1;
  if (situation%4==1 || situation%4==0) {
    for(int i=0; i<CALIBRATE_TIMES; i++) {
      turnLeft(90);
      this->one_side_calibrate();
      turnRight(90);
      this->one_side_calibrate();
    }
  }
  else { //(situation%4=2 || situation%4==3) 
    for(int i=0; i<CALIBRATE_TIMES; i++) {
      turnRight(90);
      this->one_side_calibrate();
      turnLeft(90);
      this->one_side_calibrate();
    }
  }
}


void Calibrator::test_calibrate() {
  this->one_side_calibrate();
}

//private
void Calibrator::one_side_calibrate() {
  for(int i=0; i<3; i++) {
    this->calibrate_angle();
    this->calibrate_distance(); // much accurate
  }
}

void Calibrator::calibrate_angle() {
  const double DETECTORS_INTERVAL = 8.2; //cm
  const int ADJUST_ANGLE = 10; // degree
  int left_reading = frontEye->get_ir_reading_left();
  int right_reading = frontEye->get_ir_reading_right();
  if (left_reading>30 || right_reading>30) {
    return;
  }
  // http://www.nongnu.org/avr-libc/user-manual/group__avr__math.html#ga98384ad60834911ec93ac5ae1af4cf0a
  double theta = asin((left_reading - right_reading)/DETECTORS_INTERVAL); // radian
  theta *= RAD_TO_DEG; // degree

  while(abs(theta)>3) {
    if(theta>0) {
      turnLeft(ADJUST_ANGLE);
      turnRight(abs(theta)+ADJUST_ANGLE);
    }
    else {
      turnRight(ADJUST_ANGLE);
      turnLeft(abs(theta)+ADJUST_ANGLE);
    }
    left_reading = frontEye->get_ir_reading_left();
    right_reading = frontEye->get_ir_reading_right();
    theta = asin((left_reading - right_reading)/DETECTORS_INTERVAL)*RAD_TO_DEG; 
    Serial.print(F("theta: ")); Serial.println(theta);
  }

}
void Calibrator::calibrate_distance() {
  const int ADJUST_DISTANCE = 2; // cm
  int left_reading = frontEye->get_ir_reading_left();
  int right_reading = frontEye->get_ir_reading_right();
  double avg_reading = (left_reading+right_reading)/2.0;
  double delta = avg_reading - 10; // actual - expected 

  while(abs(delta)>0.5) {
    if(delta>0) {
      moveBackward(ADJUST_DISTANCE);
      moveForward(abs(delta)+ADJUST_DISTANCE);
    }
    else {
      if(left_reading>ADJUST_DISTANCE+1) {
        moveForward(ADJUST_DISTANCE);
        moveBackward(abs(delta)+ADJUST_DISTANCE);
      }
      else {
        moveBackward(abs(delta));
      }
    }
    left_reading = frontEye->get_ir_reading_left();
    right_reading = frontEye->get_ir_reading_right();
    avg_reading = (left_reading+right_reading)/2.0;
    delta = avg_reading - 10; 
    Serial.print(F("delta: ")); Serial.println(delta);
  }

}
