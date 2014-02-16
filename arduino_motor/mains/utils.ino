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
  float ticksForOneCentimeter = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
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
void printCounts()
{
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  Serial.print("LEFT COUNT (getCountsM1): "); Serial.println(leftPololuCount);
  Serial.println(PololuWheelEncoders::checkErrorM1());
  Serial.print("RIGHT COUNT (getCountsM2): "); Serial.println(rightPololuCount);
  Serial.println(PololuWheelEncoders::checkErrorM2());
}
void printDeadReckoning(double deltaX, double deltaY, double distanceToTravel)
{
  Serial.print("deltaX: "); Serial.println(deltaX);
  Serial.print("deltaY: "); Serial.println(deltaY);
  Serial.print("distanceToTravel: "); Serial.println(distanceToTravel);
};
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
float turnAngle(int angle) {
  float divisibleTicks;
  if (angle <= 90) {
    divisibleTicks = 126.7;  // 127.5
  }
  
  else if (angle <= 180) {
    divisibleTicks = 125.5;  // 64.5
  }

  else if (angle <= 270) {
    divisibleTicks = 126.7;
  } 
  
  else if (angle <= 450) {
    divisibleTicks = 125.6;
  }
  
  else if (angle <= 540) {
    divisibleTicks = 125;
  }
  
  else if (angle <= 630) {
    divisibleTicks = 125;
  }
  else {
    divisibleTicks = 125;
  }
  
  float ticksForOneDegree = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float angleToTurn = ticksForOneDegree * angle;
  return angleToTurn;
}

float turnAngleR(int angle) {
  float divisibleTicks;
  if (angle <= 90) {
    divisibleTicks = 127.5;  // 127.5
  }
  
  else if (angle <= 180) {
    divisibleTicks = 125.5;  // 64.5
  }

  else if (angle <= 270) {
    divisibleTicks = 126.7;
  } 
  
  else if (angle <= 450) {
    divisibleTicks = 125.6;
  }
  
  else if (angle <= 540) {
    divisibleTicks = 125;
  }
  
  else if (angle <= 630) {
    divisibleTicks = 125;
  }
  else {
    divisibleTicks = 125;
  }
  
  float ticksForOneDegree = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float angleToTurn = ticksForOneDegree * angle;
  return angleToTurn;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void setScale(double scale) {
  SetpointLeft *= scale;
  SetpointRight *= scale;
  SetpointMid *= scale; // always 0

  Config::PID_SETPOINT *= scale;
  Config::PID_UPPER_LIMIT *= scale;
  Config::PID_LOWER_LIMIT *= scale;

  Config::MAX_SPEED *= scale;
  Config::TARGET_SPEED *= scale;
  Config::MIN_SPEED *= scale;

  leftPID.SetOutputLimits(Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT);
  rightPID.SetOutputLimits(Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT);
  midPID.SetOutputLimits(-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2);
}
