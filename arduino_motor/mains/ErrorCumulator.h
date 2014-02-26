#ifndef ERRORCUMULATOR_H
#define ERRORCUMULATOR_H
#include "HMC5883LCompass.h"
// The signs of numbers have meaning in ErrorCumulator
class ErrorCumulator {
public:
  // Deduced Reckoning 
  double theta;
  double deltaX;
  double deltaY;

  double turning_error; // in degree, + counter clockwise, - clockwise; orthodox coordinate
  // error = Real Value - Actual Value

  // set up
  ErrorCumulator();
  void init();

  // turning
  double adjust_turning_angle(double target_angle);
  void record_turning_error(double adjusted_target_angle, double angle_difference); // possible split turning right and left
  void record_turning_error_compass(double adjusted_target_angle);

  // auxilary 
  void print_dead_reckoning();
  void reset_dead_reckoning();
  float get_compass_read();
  

  // turning and forward mode switching 
  void change_to_forward_mode();
  void change_to_right_mode();
  void change_to_left_mode();
  HMC5883LCompass* compass;
private:
  //HMC5883LCompass* compass; // public for testing purpose
  double compass_reading_0;
  
  int current_mode; // 00, 01, 02
  void change_to_turning_mode(int mode);
  
  double forward_deltaX;
  double forward_deltaY;
  double forward_theta;
  
};
#endif
