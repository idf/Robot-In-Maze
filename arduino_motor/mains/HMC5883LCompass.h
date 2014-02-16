#ifndef HMC5883LCompass_H
#define HMC5883LCompass_H
#include <Wire.h>
#include <HMC5883L.h>
class HMC5883LCompass {
private:
  HMC5883L compass;
public:
  HMC5883LCompass();
  float getHeading();
};

#endif
