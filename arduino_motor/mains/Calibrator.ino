#include "Calibrator.h"
//public 


Calibrator::Calibrator(FrontEye* frontEye) {
  this->frontEye = frontEye;
}


void Calibrator::calibrate(int situation) {
  const int CALIBRATE_TIMES = 2;
  this->turned_angle = 0; // clear

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
    while(!this->calibrate_angle()){
      this->calibrate_distance();
    }
    this->calibrate_distance(); // much accurate
  }
}


bool Calibrator::calibrate_angle() {
  const double DETECTORS_INTERVAL = 12.2; //cm
  const int ADJUST_ANGLE = 7 ; // degree

  double theta;
  int left_reading;
  int right_reading;


  // http://www.nongnu.org/avr-libc/user-manual/group__avr__math.html#ga98384ad60834911ec93ac5ae1af4cf0a

  while(true) {
    left_reading = frontEye->get_ir_reading_left();
    right_reading = frontEye->get_ir_reading_right();
    theta = asin((left_reading - right_reading)/DETECTORS_INTERVAL)*RAD_TO_DEG; 

    if (left_reading>14 || right_reading>14 || left_reading<6 || right_reading<6) {
      return false;
    }

    if(abs(theta)<=1) {
      break;
    }

    if(theta>0) {
      if(abs(theta)>8) {
        turnRight(abs(theta));
      }
      else { //avoid small angle problem
        turnLeft(ADJUST_ANGLE); delay(100);
        turnRight(abs(theta)+ADJUST_ANGLE); delay(100);       
      }
    }
    else {
      if(abs(theta)>8) {
        turnLeft(abs(theta));
      }
      else { 
        turnRight(ADJUST_ANGLE); delay(100);
        turnLeft(abs(theta)+ADJUST_ANGLE); delay(100);       
      }
    }

    // testing print 
    //Serial.print(F("theta: ")); Serial.println(theta);
    //frontEye->test_readings();
  }
  return true;
}


void Calibrator::calibrate_distance() {
  const int ADJUST_DISTANCE = 3; // cm

  int left_reading;
  int right_reading;
  double avg_reading;
  double delta;

  while(true) { // break at abs(delta)<=threshold
    left_reading = frontEye->get_ir_reading_left();
    right_reading = frontEye->get_ir_reading_right();
    avg_reading = (left_reading+right_reading)/2.0;
    delta = avg_reading - 10; // actual - expected 

    if(abs(delta)<=0.5) {
      break;
    }

    if(delta>0) {
      //moveBackward(ADJUST_DISTANCE); delay(100);
      //moveForward(abs(delta)+ADJUST_DISTANCE); delay(100);
      moveForward(abs(delta));
    }
    else {
      if(0.5*(left_reading+right_reading)>ADJUST_DISTANCE+3) { // avoid close obstable ahead
        //moveForward(ADJUST_DISTANCE); delay(100);
        //moveBackward(abs(delta)+ADJUST_DISTANCE); delay(100);
        moveBackward(abs(delta)); delay(100);
      }
      else {
        moveBackward(abs(delta)); delay(100);
      }
    }

    // testing print 
    //Serial.print(F("delta: ")); Serial.println(delta);
    //frontEye->test_readings();
  }

}
