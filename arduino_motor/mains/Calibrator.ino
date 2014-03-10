#include "Calibrator.h"
//public 
Calibrator::Calibrator(FrontEye frontEye) {
  this->frontEye = frontEye;
}
void Calibrator::calibrate() {
  this->turned_angle = 0; // clear
}

//private