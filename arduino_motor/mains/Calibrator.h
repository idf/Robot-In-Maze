#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include "FrontEye.h"
#define RAD_TO_DEG 57.2957795
class Calibrator {
public:
  Calibrator(FrontEye* frontEye);
  void calibrate(int situation);
  void test_calibrate();

private:
  FrontEye* frontEye; // to get the sensor reading
  double turned_angle; // to restore original orientation 


  void one_side_calibrate();
  bool calibrate_angle();
  void calibrate_distance();
};
#endif
