#ifndef HMC5883LCompass_H
#define HMC5883LCompass_H
#include <Wire.h>
#include <HMC5883L.h>
class HMC5883LCompass {
private:
  HMC5883L compass;
public:
  void compassSetup();
  float getHeading();
  void printAxis();
};

#endif
