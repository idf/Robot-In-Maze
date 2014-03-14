#include "globals.h"
#include "PinChangeInt.h"
#define MAGIC_NUMBER 20 // high V 36 Okay 20 perfect 10 not that good 
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
int isM1Forward either 1 or -1 or 0
int isM2Forward either 1 or -1 or 0 
*/

static  double leftCnt = 0;
static  double rightCnt = 0;

void configureMotor(int isM1Forward, int isM2Forward) 
{
  long leftPololuCount;                     
  long rightPololuCount; 

  //pin change
  PCintPort::attachInterrupt(3, leftCounter, CHANGE);
  PCintPort::attachInterrupt(5, rightCounter,CHANGE);


  /*
  if(abs(leftPololuCount - rightPololuCount) > 30000){ // if not too many errors 
    halt();
  } 

  else { */
  if(millis() - timing >= Config::SAMPLE_TIME){ // Calculated every 10 ms (Sample time 10 ms)
  //leftPololuCount = leftCnt();                       
  //rightPololuCount = rightCnt();
  leftPololuCount = leftCnt;
  rightPololuCount = rightCnt;
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
  errorCumulator->theta += (rightcm - leftcm) / Config::WHEELS_INTERVAL; // deduced reckoning 
  errorCumulator->deltaX += distanceToTravel * cos(errorCumulator->theta); // deltaX is cumulative
  errorCumulator->deltaY += distanceToTravel * sin(errorCumulator->theta);

  // errorCumulator->print_dead_reckoning();
  // Serial.print("timez: "); Serial.println(timez); // time interval affect polling performance
  leftTicks /= (timez/1000.0);
  rightTicks /= (timez/1000.0); // ms
  if(leftPololuCount*previousLeftTick>0 && rightPololuCount*previousRightTick>0) { // avoid overflow
    InputLeft = leftTicks;
    InputRight = rightTicks;
    InputMid = errorCumulator->deltaY / Config::DISTANCE_PER_TICK_CM;
  }

  
  midPID.Compute();
  SetpointRight = Config::PID_SETPOINT  + map(OutputMid,-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2, -Config::PID_SETPOINT, +Config::PID_SETPOINT);
  rightPID.Compute();
  leftPID.Compute();

  

  
  previousLeftTick = leftPololuCount;
  previousRightTick = rightPololuCount;
  timing = millis();

  int m1Speed = isM1Forward * map(OutputLeft, Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT, Config::MIN_SPEED, Config::MAX_SPEED);
  int m2Speed = isM2Forward * map(OutputRight, Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT, Config::MIN_SPEED, Config::MAX_SPEED);

  // if(isM1Forward*isM2Forward>0) delay(200); // replace print // move to reachTarget
  /*
  Serial.print(F("leftTicks: ")); Serial.println(leftTicks);
  Serial.print(F("rightTicks: ")); Serial.println(rightTicks);
  Serial.print(F("InputMid: ")); Serial.println(InputMid);
  Serial.print(F("SetpointLeft: ")); Serial.println(SetpointLeft);
  Serial.print(F("SetpointRight: ")); Serial.println(SetpointRight);
  
  Serial.print(F("m1: ")); Serial.println(m1Speed);
  Serial.print(F("m2: ")); Serial.println(m2Speed);
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
  resetPololuTicks(); // since we only cares about the difference, the starting point better set to 0 to avoid overflow
  double avgTicksForAngleOrDist = 0;
  long firstLeftCount = leftCnt;//leftCnt();
  long firstRightCount = rightCnt;//rightCnt();
  

  while (target_tick - avgTicksForAngleOrDist > 300) { //target_tick - change to 'angle' for other formula // 200 is tested
  // the tolerance value affect the turning errors

    double leftTicksForAngleOrDist = leftCnt;
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = rightCnt;
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 

    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
    configureMotor(isLeftForward, isRightForward);

    /* IMPORTANT */
    //if(isLeftForward*isRightForward>0)
      delay(MAGIC_NUMBER); // long distance problem
  }

  // fading
  setScale(0.35); // small, increase accuracy, too small, cannot move (torque)
  //Serial.println(F("fading"));
  while (target_tick - avgTicksForAngleOrDist > 0) { // tolerance
  // the tolerance value affect the turning errors

    double leftTicksForAngleOrDist = leftCnt;
    leftTicksForAngleOrDist = abs(leftTicksForAngleOrDist - firstLeftCount);

    double rightlTicksForAngleOrDist = rightCnt;
    rightTicksForAngleOrDist = abs(rightlTicksForAngleOrDist - firstRightCount); 
    

    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // turn right
    configureMotor(isLeftForward, isRightForward);
  }
  motorShield.setBrakes(Config::DESIGNED_MAX_SPEED, Config::DESIGNED_MAX_SPEED); 
  setScale(1/0.35);
  Serial.print(F("Ticks statistics: ")); Serial.print(avgTicksForAngleOrDist); Serial.print(F(" / ")); Serial.println(target_tick);
  delay(40); // vital for ticks target
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
  errorCumulator->change_to_forward_mode();
  const int isLeftForward = 1;
  const int isRightForward = 1;

  double noOfTicksForDist = distCentimeter(dist);
  double realNoOfTicksForDist = reachTickTarget(isLeftForward, isRightForward, noOfTicksForDist);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void moveBackward(double dist) 
{  
  errorCumulator->change_to_forward_mode();
  const int isLeftForward = -1;
  const int isRightForward = -1;

  double noOfTicksForDist = distCentimeter(dist);
  double realNoOfTicksForDist = reachTickTarget(isLeftForward, isRightForward, noOfTicksForDist);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// angle in this function is abs
// but in ErrorCumulator, it is +/-
//double noOfTicksForAngle = turnAngleR(angle);
void turnRight(double angle) { 
  errorCumulator->change_to_right_mode();
  const int isLeftForward = 1;
  const int isRightForward = -1;

  double adjusted_angle = errorCumulator->adjust_turning_angle(isRightForward*angle);
  adjusted_angle = abs(adjusted_angle);
  double noOfTicksForAngle = adjusted_angle*Config::TICKS_PER_DEGREE;

  double realNoOfTicksForAngle = reachTickTarget(isLeftForward, isRightForward, noOfTicksForAngle);

  errorCumulator->record_turning_error(isRightForward*adjusted_angle, (realNoOfTicksForAngle - noOfTicksForAngle)/Config::TICKS_PER_DEGREE); 
  //errorCumulator->record_turning_error_compass(isRightForward*adjusted_angle); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// depends on whether turning right and turning left is symmetric
// NOT SYMMETRIC w.r.t. deltaY
void turnLeft(double angle) {
  errorCumulator->change_to_left_mode();
  const int isLeftForward = -1;
  const int isRightForward = 1;

  double adjusted_angle = errorCumulator->adjust_turning_angle(isRightForward*angle);
  adjusted_angle = abs(adjusted_angle);
  double noOfTicksForAngle = adjusted_angle*Config::TICKS_PER_DEGREE_LEFT;
  double realNoOfTicksForAngle = reachTickTarget(isLeftForward, isRightForward, noOfTicksForAngle);
  
  errorCumulator->record_turning_error(isRightForward*adjusted_angle, (realNoOfTicksForAngle - noOfTicksForAngle)/Config::TICKS_PER_DEGREE); 
  //errorCumulator->record_turning_error_compass(isRightForward*adjusted_angle); 

}
///////////////////////////////////////////////////////////////////////////////////////////////////

//ISR for interrupts
void leftCounter()
{
  leftCnt = leftCnt+1;

}

void rightCounter()
{
  rightCnt=rightCnt+1;
  //if(rightCnt == 19) rightCnt+=1;
  
}

