#ifndef HMC5883LCompass_H
#define HMC5883LCompass_H
#include <Wire.h>
#include <HMC5883L.h>
class HMC5883LCompass {
private:
  HMC5883L compass;
  void initialization();
  void setupHMC5883L();
public:
  HMC5883LCompass();
  float getHeading();
};

#endif
