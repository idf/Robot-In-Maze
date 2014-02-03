#include "constants.h"
#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
int isM1Forward either 1 or -1
int isM2Forward either 1 or -1
*/
void configureMotor(int isM1Forward, int isM2Forward) 
{
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());

  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      float leftTime = leftPololuCount -previousLeftTick;
      float rightTime = rightPololuCount -previousRightTick;

      float leftcm = DISTANCE_PER_TICK_CM * leftTime;
      float rightcm = DISTANCE_PER_TICK_CM * rightTime;
      float distanceToTravel = (leftcm + rightcm)/2;
      deltaHeading =  (leftcm-rightcm) / 17.2 + deltaHeading;
      deltaX = (distanceToTravel * cos(deltaHeading) + deltaX); // + 0.2
      deltaY = distanceToTravel * sin(deltaHeading) + deltaY;
      leftTime = leftTime / (timez);
      leftTime = leftTime * 1000;
      rightTime = rightTime / (timez);
      rightTime = rightTime * 1000;
      InputMid = deltaX * 10000;

      InputLeft = leftTime;
      InputRight = rightTime;

      midPID.Compute();
      
      SetpointRight = PID_SETPOINT + map(OutputMid,-1000,1000,-PID_SETPOINT,1400);
      rightPID.Compute();
      leftPID.Compute();
      previousLeftTick = leftPololuCount;
      previousRightTick = rightPololuCount;
      timing = millis();


      if((isM1Forward!=1 && isM1Forward!=-1) || (isM2Forward!=1 && isM2Forward!=-1)) {
        isM1Forward = 1;
        isM2Forward = 1;
      }
      int m1Speed = isM1Forward * map(OutputLeft,0,PID_UPPER_LIMIT,MID_SPEED,MAX_SPEED);
      int m2Speed = isM2Forward * map(OutputRight,0,PID_UPPER_LIMIT,MID_SPEED,MAX_SPEED);
      motorShield.setSpeeds(m1Speed, m2Speed);
    }
  }
  else{
    halt();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void halt() 
{ 
  motorShield.setSpeeds(0, 0);
  delay(100);
}
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
void moveForward(float dist) 
{  
  float noOfTicksForDist = distCentimeter(dist);
  
  // ------ Distance to ticks formula ------- //
  float avgTicksForAngleOrDist = 0;
  
  long leftCount0 = abs(PololuWheelEncoders::getCountsM1());
  long rightCount0 = abs(PololuWheelEncoders::getCountsM2());

  
  while (avgTicksForAngleOrDist < noOfTicksForDist) {
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
    leftTicksForAngleOrDist = leftTicksForAngleOrDist - leftCount0;
    
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
    rightTicksForAngleOrDist = rightTicksForAngleOrDist - rightCount0;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;

    configureMotor(1, 1);
  }

  
  motorShield.setBrakes(MAX_SPEED, MAX_SPEED);
  delay(40);
  halt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

