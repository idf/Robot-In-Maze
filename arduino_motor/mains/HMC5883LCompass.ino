/*

routine to be followed in the mains.ino:


//initalization 
void setup(){

  HMC5883LCompass::HMC5883LCompass()
}

// in main loop.
void loop(){
  
  float heading = Compass::getHeading();
  Serial.println(heading);
  delay(100); //to slow down the serial print

}

*/
#include <Wire.h>
#include <HMC5883L.h>
#include "HMC5883LCompass.h"

void HMC5883LCompass::compassSetup() {
  // Initilization 
  Wire.begin();
  this->compass = HMC5883L(); //new instance of HMC5883L library
  //setupHMC5883L(); //setup the HMC5883L

  //Setup
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);
}

float HMC5883LCompass::getHeading(){
  //Get the reading from the HMC5883L and calculate the heading
  MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  float declinationAngle = 0.00378;
  heading += declinationAngle;
  heading = heading-2*heading;
  
  // Correct for when signs are reversed.
  // take absolute values for coumulative angle

  //if(heading < 0) heading += 2*PI;
  //if(heading > 2*PI) heading -= 2*PI;


  return heading * RAD_TO_DEG; //radians to degrees
}
