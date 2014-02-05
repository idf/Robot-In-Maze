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
  printCounts();

  if( abs(leftPololuCount - rightPololuCount)<30000){ // if not too many errors 
    if(millis() - timing >= SAMPLE_TIME){ // Calculated every 10 ms (Sample time 10 ms)
      leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis() - timing; // time passed by 
      float leftTicks = leftPololuCount - previousLeftTick;
      float rightTicks = rightPololuCount - previousRightTick;

      float leftcm = DISTANCE_PER_TICK_CM * leftTicks;
      float rightcm = DISTANCE_PER_TICK_CM * rightTicks;
      float distanceToTravel = (leftcm + rightcm)/2.0;

      /* http://rossum.sourceforge.net/papers/DiffSteer/DiffSteer.html */
      theta += (rightcm - leftcm) / WHEELS_INTERVAL; // deduced reckoning 
      deltaX += distanceToTravel * cos(theta); 
      deltaY += distanceToTravel * sin(theta);
      
      Serial.print("timez: "); Serial.println(timez);
      leftTicks /= (timez/1000.0);
      rightTicks /= (timez/1000.0); // ms

      InputMid = deltaY / DISTANCE_PER_TICK_CM;
      InputLeft = leftTicks;
      InputRight = rightTicks;
      
      midPID.Compute();
      //SetpointRight = PID_SETPOINT + 0.5 * map(OutputMid,-PID_SETPOINT/2, PID_SETPOINT/2, -PID_SETPOINT, +PID_SETPOINT);
      const int COEFFICIENT; 
      SetpointLeft += COEFFICIENT * map(OutputMid,-PID_SETPOINT/2, PID_SETPOINT/2, -PID_SETPOINT, +PID_SETPOINT);
      rightPID.Compute();
      leftPID.Compute();

      Serial.print("leftTicks: "); Serial.println(leftTicks);
      Serial.print("rightTicks: "); Serial.println(rightTicks);
      Serial.print("InputMid: "); Serial.println(InputMid);
      Serial.print("SetpointRight: "); Serial.println(SetpointRight);
      previousLeftTick = leftPololuCount;
      previousRightTick = rightPololuCount;
      timing = millis();


      if((isM1Forward!=1 && isM1Forward!=-1) || (isM2Forward!=1 && isM2Forward!=-1)) {
        isM1Forward = 1;
        isM2Forward = 1;
      }
      int m1Speed = isM1Forward * map(OutputLeft, PID_LOWER_LIMIT, PID_UPPER_LIMIT, MID_SPEED, MAX_SPEED);
      int m2Speed = isM2Forward * map(OutputRight, PID_LOWER_LIMIT, PID_UPPER_LIMIT, MID_SPEED, MAX_SPEED);
      Serial.print("m1: "); Serial.println(m1Speed);
      Serial.print("m2: "); Serial.println(m2Speed);

      motorShield.setSpeeds(m1Speed, m2Speed);
    }
  }
  else {
    halt();
  }
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
void moveForward(float dist) 
{  
  float noOfTicksForDist = distCentimeter(dist);
  
  // ------ Distance to ticks formula ------- //
  float avgTicksForAngleOrDist = 0;
  
  long leftCount0 = abs(PololuWheelEncoders::getCountsM1());
  long rightCount0 = abs(PololuWheelEncoders::getCountsM2());

  
  while (avgTicksForAngleOrDist < noOfTicksForDist) {
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1()) - leftCount0;
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2()) - rightCount0;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2; // reaching the target

    configureMotor(1, 1);
  }

  
  motorShield.setBrakes(MAX_SPEED, MAX_SPEED);
  delay(40);
  halt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

