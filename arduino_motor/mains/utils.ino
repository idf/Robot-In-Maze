#include "constants.h"
#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
float distCentimeter(float centimeter) 
{
  float divisibleTicks;
  if (centimeter <= 10) {
    divisibleTicks = 19.3; 
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
  float ticksForOneCentimeter = COUNTS_PER_REVOLUTION/divisibleTicks;
  float dist = ticksForOneCentimeter * centimeter;
  return dist;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void resetPololuTicks() 
{
  PololuWheelEncoders::getCountsAndResetM1();
  PololuWheelEncoders::getCountsAndResetM2();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

