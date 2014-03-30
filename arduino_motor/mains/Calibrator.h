#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include "Eyes.h"
#define RAD_TO_DEG 57.2957795
class Calibrator {
public:
  Calibrator(Eyes* frontEye);
  void init();
  void calibrate(int situation);
  void test_calibrate();
  void try_calibrate();

private:
  Eyes* frontEye; // to get the sensor reading
  double turned_angle; // to restore original orientation 


  void one_side_calibrate();
  void one_side_calibrate(int times);
  bool calibrate_angle();
  void calibrate_distance();

  unsigned long last_time_trial;
};
#endif
