#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
float distCentimeter(float centimeter) 
{
  float divisibleTicks;
  if (centimeter <= 10) {
    divisibleTicks = 18.7; 
  } else if (centimeter <= 20) {
    divisibleTicks = 18.65;
  } else if (centimeter <= 35) {
    divisibleTicks = 19.05;
  } else if (centimeter <= 50) {
    divisibleTicks = 18.5;
  } else if (centimeter <= 75) {
    divisibleTicks = 18.9;
  } else {
    divisibleTicks = 18.7;
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
    frontEye->output_reading_ultra(),
    frontEye->output_reading_ir_left(),
    frontEye->output_reading_ir_right(),
    sideEye->output_reading_left(),
    sideEye->output_reading_right(),
    sideEye->output_reading_ultra()
    );
}

