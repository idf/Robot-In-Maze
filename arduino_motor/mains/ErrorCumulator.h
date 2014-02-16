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

  double turning_error; // in degree, + counter clockwise, - clockwise; Real Value - Actual Value

  ErrorCumulator();
  double adjust_turning_angle(double target_angle);
  void record_turning_error(double adjusted_target_angle, double angle_difference); // possible split turning right and left
  void record_turning_error_compass(double adjusted_target_angle);

  void print_dead_reckoning();

  HMC5883LCompass* compass; // public for testing purpose
private:
  
  double compass_reading_0;
};
#endif