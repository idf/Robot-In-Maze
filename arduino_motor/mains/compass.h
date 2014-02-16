#ifndef CONFIG_H
#define CONFIG_H



#include <Wire.h>
#include <HMC5883L.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////


class HMC5883LCompass {

  HMC5883L compass;

public:
  
  void initialization();
  void setupHMC5883L();
  float getHeading();

};

#endif
