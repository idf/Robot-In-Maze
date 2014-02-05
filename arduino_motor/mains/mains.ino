// use mains instead of main since main is the keyword
#include <DualVNH5019MotorShield.h>
#include <PololuWheelEncoders.h>
#include <PID_v1.h> 
#include "constants.h"
#include "globals.h"
// TAB SIZE 2, whitespaces as tab
///////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
  Serial.begin(9600); // data rate in bits per second 
  motorShield.init();

  // Encoder
  PololuWheelEncoders::init(M1_ENCODER_A, M1_ENCODER_B, M2_ENCODER_A, M2_ENCODER_B);
  leftPID.SetMode(AUTOMATIC);
  rightPID.SetMode(AUTOMATIC);
  midPID.SetMode(AUTOMATIC);

  leftPID.SetSampleTime(SAMPLE_TIME); 
  rightPID.SetSampleTime(SAMPLE_TIME); 
  midPID.SetSampleTime(SAMPLE_TIME);

  SetpointLeft = PID_SETPOINT;
  SetpointRight = PID_SETPOINT;
  SetpointMid = 0;

  // for controlling the speed 
  leftPID.SetOutputLimits(PID_LOWER_LIMIT, PID_UPPER_LIMIT);
  rightPID.SetOutputLimits(PID_LOWER_LIMIT, PID_UPPER_LIMIT);
  midPID.SetOutputLimits(-PID_SETPOINT/2, PID_SETPOINT/2);
  
  resetPololuTicks();

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
  //resetPololuTicks();
  moveForward(25.0);
  //motorShield.setSpeeds(100, 100); printCounts()
  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

  
