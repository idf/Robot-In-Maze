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

  errorCumulator->print_dead_reckoning();
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
void halt() 
{ 
  motorShield.setSpeeds(0, 0);
  delay(100);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void stopIfFault()
{ 
  if (motorShield.getM1Fault()) {
    Serial.println("M1 fault"); while(1);
  } 
  if (motorShield.getM2Fault()) {
    Serial.println("M2 fault"); while(1);
  }
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
  double noOfTicksForDist = distCentimeter(dist);
  
  // ------ Distance to ticks formula ------- //
  double avgTicksForAngleOrDist = 0;

  
  int leftCount0 = PololuWheelEncoders::getCountsM1();
  int rightCount0 = PololuWheelEncoders::getCountsM2();

  
  while (avgTicksForAngleOrDist < noOfTicksForDist) {
    leftTicksForAngleOrDist = PololuWheelEncoders::getCountsM1() - leftCount0;
    rightTicksForAngleOrDist = PololuWheelEncoders::getCountsM2() - rightCount0;

    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2.0; // reaching the target

    configureMotor(1, 1);
  }
  
  motorShield.setBrakes(Config::MAX_SPEED, Config::MAX_SPEED);
  delay(40);
  //halt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// angle in this function is abs
// but in ErrorCumulator, it is +/-
void turnRight(double angle) {
  resetPololuTicks(); 
  errorCumulator->theta = 0; // theta_error
  const int isLeftForward = 1;
  const int isRightForward = -1;

  //double noOfTicksForAngle = turnAngleR(angle);
  double adjusted_angle = abs(errorCumulator->adjust_turning_angle(angle));
  double noOfTicksForAngle = adjusted_angle*Config::TICKS_PER_DEGREE;
  // ------ Angle to ticks formula ------- //

  double avgTicksForAngleOrDist = 0;
  long firstLeftCount = PololuWheelEncoders::getCountsM1();
  long firstRightCount = PololuWheelEncoders::getCountsM2();
  



  while (noOfTicksForAngle - avgTicksForAngleOrDist > 200) { //noOfTicksForAngle - change to 'angle' for other formula
  // the tolerance value affect the turning errors
    double leftTicksForAngleOrDist = PololuWheelEncoders::getCountsM1();
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = PololuWheelEncoders::getCountsM2();
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); // right backward
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // turn right
    configureMotor(isLeftForward, isRightForward);
  }
  // fading
  setScale(0.4);
  while (noOfTicksForAngle - avgTicksForAngleOrDist > 8) { //noOfTicksForAngle - change to 'angle' for other formula
  // the tolerance value affect the turning errors
    double leftTicksForAngleOrDist = PololuWheelEncoders::getCountsM1();
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = PololuWheelEncoders::getCountsM2();
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); // right backward
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // turn right
    configureMotor(isLeftForward, isRightForward);
  }
  setScale(1/0.4);
  // Turning completed

  // minus sign indicating turning right
  errorCumulator->record_turning_error_compass(-adjusted_angle); 
  // not affect the polling
  Serial.print("Turning right: "); Serial.print(avgTicksForAngleOrDist); Serial.print(" / "); Serial.println(noOfTicksForAngle);
  

  motorShield.setBrakes(Config::MAX_SPEED, Config::MAX_SPEED);
  delay(40);
  resetPololuTicks();
  errorCumulator->theta = 0;
}
