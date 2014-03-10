#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include "FrontEye.h"
class Calibrator {
public:
  Calibrator(FrontEye frontEye);
  void calibrate();
private:
  FrontEye frontEye; // to get the sensor reading
  double turned_angle; // to restore original orientation 


  void one_side_calibrate();
};
#endif
