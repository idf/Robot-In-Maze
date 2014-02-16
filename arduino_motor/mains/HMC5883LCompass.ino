/*

routine to be followed in the mains.ino:


//initalization 
void setup(){
  Compass::initalization
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
#include <HMC5883LCompass.h>
HMC5883LCompass::HMC5883LCompass() {
  this->initialization();
  this->setupHMC5883L();
}
void HMC5883LCompass::initialization(){
  //Serial.begin(9600);
  Wire.begin();
  
  compass = HMC5883L(); //new instance of HMC5883L library
  setupHMC5883L(); //setup the HMC5883L
}




void HMC5883LCompass::setupHMC5883L(){
  //Setup the HMC5883L, and check for errors
  int error;  
  error = compass.SetScale(1.3); //Set the scale of the compass.
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so

  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so
}



double HMC5883LCompass::getHeading(){
  //Get the reading from the HMC5883L and calculate the heading
  MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
  double heading = abs(atan2(scaled.YAxis, scaled.XAxis));

  
  // Correct for when signs are reversed.
  // take absolute values for coumulative angle

  //if(heading < 0) heading += 2*PI;
  //if(heading > 2*PI) heading -= 2*PI;


  return heading * RAD_TO_DEG; //radians to degrees
}
