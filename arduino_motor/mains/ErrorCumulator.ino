#include "ErrorCumulator.h"
ErrorCumulator::ErrorCumulator() {
  this->theta = 0;
  this->deltaX = 0;
  this->deltaY = 0;

  this->turning_error = 0;

  // this->compass = new HMC5883LCompass();
  // this->compass_reading_0 = 0;

}
double ErrorCumulator::adjust_turning_angle(double target_angle) {
  double adjusted_target_angle;
  Serial.print("turning error: "); Serial.println(this->turning_error);
  adjusted_target_angle = target_angle - this->turning_error; // +/- guaranteed
  this->turning_error = 0;
  // this->compass_reading_0 = this->compass->getHeading();
  return adjusted_target_angle;
}
/**
@param: adjusted_target_angle, to know the turning direction 
@param: angle_difference, real value - actual value, the +/- has meaning
@deprecated
**/
void ErrorCumulator::record_turning_error(double adjusted_target_angle, double angle_difference) { 
  if(adjusted_target_angle>=0) { // turning left
    this->turning_error += angle_difference;
  }
  else {
    this->turning_error -= angle_difference;
  }
}
/**

**/
/*
void ErrorCumulator::record_turning_error_compass(double adjusted_target_angle) {
  double actual_angle = this->compass->getHeading() - this->compass_reading_0;
  actual_angle = -actual_angle; // the direction of compass reading angle is reversed 
  this->turning_error += (actual_angle - adjusted_target_angle);
}
*/
void ErrorCumulator::print_dead_reckoning() {
  Serial.print("deltaX: "); Serial.println(this->deltaX);
  Serial.print("deltaY: "); Serial.println(this->deltaY);
  Serial.print("theta: "); Serial.println(this->theta);
}
