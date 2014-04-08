#include "ErrorCumulator.h"
#define FORWORD 0
#define RIGHT 1
#define LEFT 2
#define BACKWARD 3
ErrorCumulator::ErrorCumulator() {
  this->theta = 0;
  this->deltaX = 0;
  this->deltaY = 0;

  this->turning_error = 0; // for turning left 0.05

  //this->compass = new HMC5883LCompass();
  this->compass_reading_0 = 0;
  this->current_mode = FORWORD;

  this->forward_deltaX = 0;
  this->forward_deltaY = 0;
  this->forward_theta = 0;

  this->right_deltaX = 0;
  this->right_deltaY = 0;
  this->right_theta = 0;

  this->left_deltaX = 0;
  this->left_deltaY = 0;
  this->left_theta = 0;

  this->backward_deltaX = 0;
  this->backward_deltaY = 0;
  this->backward_theta = 0;

}
void ErrorCumulator::init() {
  //this->compass->compassSetup();
}
double ErrorCumulator::adjust_turning_angle(double target_angle) {
  double adjusted_target_angle;
  if(Config::verbose) {
    Serial.print("turning error: "); Serial.println(this->turning_error);
  }
  adjusted_target_angle = target_angle - this->turning_error; // +/- guaranteed
  this->turning_error = 0;
  //this->compass_reading_0 = this->compass->getHeading();
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
  //double actual_angle = this->get_diff_angle(this->compass->getHeading(), this->compass_reading_0);
  // actual_angle = -actual_angle; // the direction of compass reading angle is reversed 
  //this->turning_error += (actual_angle - adjusted_target_angle);
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
  //return this->compass->getHeading();
}



// Mode Change 
void ErrorCumulator::change_to_forward_mode() {
  if(this->current_mode==FORWORD)
    return ;
  else if(this->current_mode==RIGHT) {
    this->right_deltaX = this->deltaX ;
    this->right_deltaY = this->deltaY;
    this->right_theta = this->theta;
  }
  else if(this->current_mode==LEFT) {
    this->left_deltaX = this->deltaX ;
    this->left_deltaY = this->deltaY;
    this->left_theta = this->theta;
  }
  else if(this->current_mode==BACKWARD) {
    this->backward_deltaX = this->deltaX ;
    this->backward_deltaY = this->deltaY;
    this->backward_theta = this->theta;
  }
  // end of change 
  this->reset_dead_reckoning(); // suppposed to do
  
  this->deltaX = this->forward_deltaX;
  this->deltaY = this->forward_deltaY;
  this->theta = this->forward_theta;
  
  
  // this->theta = this->turning_error; // = rather than +=

  this->current_mode=FORWORD;
}


void ErrorCumulator::change_to_backward_mode() {
  if(this->current_mode==BACKWARD)
    return ;
  else if(this->current_mode==RIGHT) {
    this->right_deltaX = this->deltaX ;
    this->right_deltaY = this->deltaY;
    this->right_theta = this->theta;
  }
  else if(this->current_mode==LEFT) {
    this->left_deltaX = this->deltaX ;
    this->left_deltaY = this->deltaY;
    this->left_theta = this->theta;
  }
  else if(this->current_mode==FORWORD) {
    this->forward_deltaX = this->deltaX ;
    this->forward_deltaY = this->deltaY;
    this->forward_theta = this->theta;
  }
  // end of change 
  this->reset_dead_reckoning(); // suppposed to do
  
  this->deltaX = this->backward_deltaX;
  this->deltaY = this->backward_deltaY;
  this->theta = this->backward_theta;
  
  
  // this->theta = this->turning_error; // = rather than +=

  this->current_mode=BACKWARD;
}

void ErrorCumulator::change_to_right_mode() {
  this->change_to_turning_mode(RIGHT);
}

void ErrorCumulator::change_to_left_mode() {
  this->change_to_turning_mode(LEFT);
}


// private
void ErrorCumulator::change_to_turning_mode(int mode) {
  if(this->current_mode==mode) {
    return;
  }
  else if (this->current_mode==FORWORD) {
    this->forward_deltaX = this->deltaX;
    this->forward_deltaY = this->deltaY;
    this->forward_theta = this->theta;
    
    // this->turning_error += this->theta; // theta +/- consistent
  }
  else if (this->current_mode==BACKWARD) {
    this->backward_deltaX = this->deltaX;
    this->backward_deltaY = this->deltaY;
    this->backward_theta = this->theta;
  }
  else if(this->current_mode==RIGHT) {
    this->right_deltaX = this->deltaX;
    this->right_deltaY = this->deltaY;
    this->right_theta = this->theta;  
  }
  else if(this->current_mode==LEFT) {
    this->left_deltaX = this->deltaX;
    this->left_deltaY = this->deltaY;
    this->left_theta = this->theta;
  }
  
  this->reset_dead_reckoning();
  this->current_mode = mode;

  if(mode==LEFT) {
    this->deltaX = this->left_deltaX;
    this->deltaY = this->left_deltaY;
    this->theta = this->left_theta;
  }
  else if(mode==RIGHT) {
    this->deltaX = this->right_deltaX;
    this->deltaY = this->right_deltaY;
    this->theta = this->right_theta;
  }
}

void ErrorCumulator::reset() {
  this->forward_deltaX = 0;
  this->forward_deltaY = 0;
  this->forward_theta = 0;

  this->right_deltaX = 0;
  this->right_deltaY = 0;
  this->right_theta = 0;

  this->left_deltaX = 0;
  this->left_deltaY = 0;
  this->left_theta = 0;

  this->backward_deltaX = 0;
  this->backward_deltaY = 0;
  this->backward_theta = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/**
return the angle difference within 180 degrees give two angle in the range [-180, +180]
orthodox coordiates
@param: alpha_t, \in [-180, +180]
@param: alpha_0, \in [-180, +180]
**/
float ErrorCumulator::get_diff_angle(float alpha_t, float alpha_0) {
  if(alpha_0>=0 && alpha_t>=0) 
    return alpha_t - alpha_0;
  else if (alpha_0<0 && alpha_t>=0) {
    float delta = alpha_t - alpha_0;
    if(delta>=180) 
      delta -= 360;
    return delta;
  }
  else if(alpha_0>=0 && alpha_t<0) {
    float delta = alpha_t - alpha_0;
    if(delta<-180) 
      delta += 360;
    return delta;
  }
  else  // alphas both negative 
    return alpha_t - alpha_0;
  
}
