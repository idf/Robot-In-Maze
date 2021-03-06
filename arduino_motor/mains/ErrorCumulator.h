#ifndef ERRORCUMULATOR_H
#define ERRORCUMULATOR_H
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
  void change_to_backward_mode();
  void change_to_right_mode();
  void change_to_left_mode();
  void reset();
  //HMC5883LCompass* compass;
private:
  //HMC5883LCompass* compass; // public for testing purpose
  double compass_reading_0;
  
  int current_mode; // 00, 01, 02

  double right_deltaX;
  double right_deltaY;
  double right_theta;

  double left_deltaX;
  double left_deltaY;
  double left_theta;

  double forward_deltaX;
  double forward_deltaY;
  double forward_theta;

  double backward_deltaX;
  double backward_deltaY;
  double backward_theta;


  void change_to_turning_mode(int mode);
  float get_diff_angle(float alpha_t, float alpha_0);
  
};
#endif
