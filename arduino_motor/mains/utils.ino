#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
float distCentimeter(float centimeter) 
{
  float divisibleTicks;
  
  if (centimeter <= 10) {
    divisibleTicks = 18.8; 
  } else if (centimeter <= 20) {
    divisibleTicks = 18.8;
  } else if (centimeter <= 30) {
    divisibleTicks = 18.8;
  } else if (centimeter <= 40) {
    divisibleTicks = 18.8;
  } else if (centimeter <= 50) {
    divisibleTicks = 18.9;
  } else if (centimeter <= 75) {
    divisibleTicks = 19.0;
  } else {
    divisibleTicks = 19.0;
  }
  
  
  float ticksForOneCentimeter = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float dist = ticksForOneCentimeter * centimeter;
  return dist;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void resetPololuTicks() 
{
  //PololuWheelEncoders::getCountsAndResetM1();
  //PololuWheelEncoders::getCountsAndResetM2();
  leftCnt = 0;
  rightCnt = 0;
  previousLeftTick = 0;
  previousRightTick = 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void printCounts()
{
  long leftPololuCount = leftCnt;//abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = rightCnt;//abs(PololuWheelEncoders::getCountsM2());
  Serial.print(F("LEFT COUNT (getCountsM1): ")); Serial.println(leftPololuCount);
  Serial.print(F("RIGHT COUNT (getCountsM2): ")); Serial.println(rightPololuCount);
  // Serial.print(F("previousLeftTick: ")); Serial.println(previousLeftTick);
  // Serial.print(F("previousRightTick: ")); Serial.println(previousRightTick);


  //Serial.println(PololuWheelEncoders::checkErrorM1());
  //Serial.println(PololuWheelEncoders::checkErrorM2());

}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Replaced by const TICKS_PER_DEGREE
double angle_to_ticks(double angle) {
 return (Config::WHEELS_INTERVAL/2)*(PI/180)*(angle/Config::DISTANCE_PER_TICK_CM);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void getSensorReadings() {
  serialCommnder->send_sensor_readings(
    eyes->output_reading_ir(0),
    eyes->output_reading_ir(1),
    eyes->output_reading_ir(2),
    eyes->output_reading_ir(3),
    eyes->output_reading_ir(4),
    eyes->output_reading_ultra(0),
    eyes->output_reading_ultra(1)
    );
}

