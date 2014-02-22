#include "ErrorCumulator.h"
#define FORWORD 0
#define RIGHT 1
#define LEFT 2
ErrorCumulator::ErrorCumulator() {
  this->theta = 0;
  this->deltaX = 0;
  this->deltaY = 0;

  this->turning_error = 0; // for turning left 0.05

  this->compass = new HMC5883LCompass();
  this->compass_reading_0 = 0;
  this->current_mode = FORWORD;

  //TODO

}
void ErrorCumulator::init() {
  this->compass->compassSetup();
}
double ErrorCumulator::adjust_turning_angle(double target_angle) {
  double adjusted_target_angle;
  Serial.print("turning error: "); Serial.println(this->turning_error);
  adjusted_target_angle = target_angle - this->turning_error; // +/- guaranteed
  this->turning_error = 0;
  this->compass_reading_0 = this->compass->getHeading();
  return adjusted_target_angle;
}
/**
@param: adjusted_target_angle, to know the turning direction 
@param: abs_angle_difference, real value - actual value, the +/- has meaning
@deprecated
**/
void ErrorCumulator::record_turning_error(double adjusted_target_angle, double abs_angle_difference) { 
  if(adjusted_target_angle>=0) { // turning left
    this->turning_error += abs_angle_difference;
  }
  else { // turning right
    this->turning_error -= abs_angle_difference;
  }
}
/**

**/
void ErrorCumulator::record_turning_error_compass(double adjusted_target_angle) {
  double actual_angle = this->compass->getHeading() - this->compass_reading_0;
  // actual_angle = -actual_angle; // the direction of compass reading angle is reversed 
  this->turning_error += (actual_angle - adjusted_target_angle);
}

void ErrorCumulator::print_dead_reckoning() {
  Serial.print(F("deltaX: ")); Serial.println(this->deltaX);
  Serial.print(F("deltaY: ")); Serial.println(this->deltaY);
  Serial.print(F("theta: ")); Serial.println(this->theta);
  Serial.print(F("turning erorr: ")); Serial.println(this->turning_error);
}

void ErrorCumulator::reset_dead_reckoning() {
  this->deltaX = 0;
  this->deltaY = 0;
  this->theta = 0;
}

float ErrorCumulator::get_compass_read() {
  return this->compass->getHeading();
}


// Mode Change 
void ErrorCumulator::change_to_forward_mode() {
  if(this->current_mode==FORWORD)
    return ;
  else if(this->current_mode==RIGHT) {

  }
  else if(this->current_mode==LEFT) {

  }
  // end of change 
  this->reset_dead_reckoning();
  this->theta = this->turning_error; // = rather than +=

  this->current_mode=FORWORD;
}

void ErrorCumulator::change_to_right_mode() {
  if(this->current_mode==RIGHT) 
    return ;
  else if(this->current_mode==FORWORD) {
    this->turning_error = this->theta; // theta +/- consistent // = rather than +=
    this->reset_dead_reckoning();
  } 
  else if(this->current_mode==LEFT) {
    // do nothing 
  }

  // end of change
  this->current_mode = RIGHT;
}

void ErrorCumulator::change_to_left_mode() {
  if(this->current_mode==LEFT) 
    return ;
  else if (this->current_mode==FORWORD) {
    this->turning_error += this->theta; // theta +/- consistent
    this->reset_dead_reckoning();
  }
  else if(this->current_mode==RIGHT) {
    // do nothing 
  }
  // end of change 
  this->current_mode = LEFT;
}