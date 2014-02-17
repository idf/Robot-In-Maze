#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
int isM1Forward either 1 or -1 or 0
int isM2Forward either 1 or -1 or 0 
*/
void configureMotor(int isM1Forward, int isM2Forward) 
{
  long leftPololuCount;                     
  long rightPololuCount; 
  // printCounts();

  /*
  if(abs(leftPololuCount - rightPololuCount) > 30000){ // if not too many errors 
    halt();
  } 

  else { */
  if(millis() - timing >= Config::SAMPLE_TIME){ // Calculated every 10 ms (Sample time 10 ms)
  leftPololuCount = PololuWheelEncoders::getCountsM1();                       
  rightPololuCount = PololuWheelEncoders::getCountsM2();
  long timez = millis() - timing; // time passed by 
  // incremental
  double leftTicks = abs(leftPololuCount - previousLeftTick); // positive for forward, negative for backward
  double rightTicks = abs(rightPololuCount - previousRightTick);

  // distanceToTravel is incremental
  double leftcm = Config::DISTANCE_PER_TICK_CM * leftTicks;
  double rightcm = Config::DISTANCE_PER_TICK_CM * rightTicks;
  double distanceToTravel = (leftcm + rightcm)/2.0;

  /* http://rossum.sourceforge.net/papers/DiffSteer/DiffSteer.html */
  /*
  Dead reckoning is only for feedback to speed; 
  not yet used for navigation of positioning
  */
  //theta += (rightcm*isM2Forward - leftcm*isM1Forward) / WHEELS_INTERVAL; // deduced reckoning 
  errorCumulator->theta += (rightcm - leftcm) / Config::WHEELS_INTERVAL; // deduced reckoning 
  // added 1 or -1 for turnning
  errorCumulator->deltaX += distanceToTravel * cos(errorCumulator->theta); // deltaX is cumulative
  errorCumulator->deltaY += distanceToTravel * sin(errorCumulator->theta);

  // errorCumulator->print_dead_reckoning();
  Serial.print("timez: "); Serial.println(timez); // time interval affect polling performance
  leftTicks /= (timez/1000.0);
  rightTicks /= (timez/1000.0); // ms
  InputMid = errorCumulator->deltaY / Config::DISTANCE_PER_TICK_CM;
  if(leftTicks>0 && rightTicks>0) { // avoid overflow
    InputLeft = leftTicks;
    InputRight = rightTicks;
  }

  
  midPID.Compute();
  SetpointRight = Config::PID_SETPOINT + map(OutputMid,-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2, -Config::PID_SETPOINT, +Config::PID_SETPOINT);
  rightPID.Compute();
  leftPID.Compute();

  /*
  Serial.print("leftTicks: "); Serial.println(leftTicks);
  Serial.print("rightTicks: "); Serial.println(rightTicks);
  Serial.print("InputMid: "); Serial.println(InputMid);
  Serial.print("SetpointLeft: "); Serial.println(SetpointLeft);
  Serial.print("SetpointRight: "); Serial.println(SetpointRight);
  */
  previousLeftTick = leftPololuCount;
  previousRightTick = rightPololuCount;
  timing = millis();

  int m1Speed = isM1Forward * map(OutputLeft, Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT, Config::MIN_SPEED, Config::MAX_SPEED);
  int m2Speed = isM2Forward * map(OutputRight, Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT, Config::MIN_SPEED, Config::MAX_SPEED);
  /*
  Serial.print("m1: "); Serial.println(m1Speed);
  Serial.print("m2: "); Serial.println(m2Speed);
  */
  motorShield.setSpeeds(m1Speed, m2Speed);
  }
 // }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
This function receive the target_tick as target and run the motor to reach the goal
@return: real tick reached
*/
double reachTickTarget(int isLeftForward, int isRightForward, double target_tick) {
  double avgTicksForAngleOrDist = 0;
  long firstLeftCount = PololuWheelEncoders::getCountsM1();
  long firstRightCount = PololuWheelEncoders::getCountsM2();
  
  while (target_tick - avgTicksForAngleOrDist > 200) { //target_tick - change to 'angle' for other formula
  // the tolerance value affect the turning errors
    double leftTicksForAngleOrDist = PololuWheelEncoders::getCountsM1();
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = PololuWheelEncoders::getCountsM2();
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
    configureMotor(isLeftForward, isRightForward);
  }
  // fading
  setScale(0.2);
  while (target_tick - avgTicksForAngleOrDist > 0) { // tolerance
  // the tolerance value affect the turning errors
    double leftTicksForAngleOrDist = PololuWheelEncoders::getCountsM1();
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = PololuWheelEncoders::getCountsM2();
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // turn right
    configureMotor(isLeftForward, isRightForward);
  }
  setScale(1/0.2);
  Serial.print(F("Ticks statistics: ")); Serial.print(avgTicksForAngleOrDist); Serial.print(F(" / ")); Serial.println(target_tick);
  motorShield.setBrakes(Config::MAX_SPEED, Config::MAX_SPEED);
  delay(100);
  return avgTicksForAngleOrDist;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
depencies: distCentimeter, configureMotor
*/
/*
max dist value = 274.6 cm for every call
(32767/2249*6*PI=274.6)
*/
void moveForward(double dist) 
{  
  const int isLeftForward = 1;
  const int isRightForward = 1;
  double noOfTicksForDist = distCentimeter(dist);
  double realNoOfTicksForDist = reachTickTarget(isLeftForward, isRightForward, noOfTicksForDist);

}
///////////////////////////////////////////////////////////////////////////////////////////////////
// angle in this function is abs
// but in ErrorCumulator, it is +/-
//double noOfTicksForAngle = turnAngleR(angle);
void turnRight(double angle) {
  resetPololuTicks(); 
  errorCumulator->theta = 0; // theta_error
  const int isLeftForward = 1;
  const int isRightForward = -1;

  double adjusted_angle = errorCumulator->adjust_turning_angle(isRightForward*angle);
  adjusted_angle = abs(adjusted_angle);
  double noOfTicksForAngle = adjusted_angle*Config::TICKS_PER_DEGREE;
  double realNoOfTicksForAngle = reachTickTarget(isLeftForward, isRightForward, noOfTicksForAngle);

  errorCumulator->record_turning_error(isRightForward*adjusted_angle, (realNoOfTicksForAngle - noOfTicksForAngle)/Config::TICKS_PER_DEGREE); 
  resetPololuTicks();
  errorCumulator->theta = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// depends on whether turning right and turning left is symmetric
void turnLeft(double angle) {
  resetPololuTicks(); 
  errorCumulator->theta = 0; // theta_error
  const int isLeftForward = -1;
  const int isRightForward = 1;

  double adjusted_angle = errorCumulator->adjust_turning_angle(isRightForward*angle);
  adjusted_angle = abs(adjusted_angle);
  double noOfTicksForAngle = adjusted_angle*Config::TICKS_PER_DEGREE;
  double realNoOfTicksForAngle = reachTickTarget(isLeftForward, isRightForward, noOfTicksForAngle);
  
  errorCumulator->record_turning_error(isRightForward*adjusted_angle, (realNoOfTicksForAngle - noOfTicksForAngle)/Config::TICKS_PER_DEGREE); 
  resetPololuTicks();
  errorCumulator->theta = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
