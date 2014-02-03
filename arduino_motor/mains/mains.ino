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
  Serial.println("Dual VNH5019 Motor Shield");
  motorShield.init();

  // Encoder
  PololuWheelEncoders::init(RIGHT_ENCODER_A, RIGHT_ENCODER_B, LEFT_ENCODER_A, LEFT_ENCODER_B);
  leftPID.SetMode(AUTOMATIC);
  rightPID.SetMode(AUTOMATIC);
  midPID.SetMode(AUTOMATIC);

  leftPID.SetSampleTime(10);
  rightPID.SetSampleTime(10);
  midPID.SetSampleTime(10);

  SetpointLeft = PID_SETPOINT;
  SetpointRight = PID_SETPOINT;
  SetpointMid = 0;

  leftPID.SetOutputLimits(0, PID_UPPER_LIMIT);
  rightPID.SetOutputLimits(0, PID_UPPER_LIMIT);
  midPID.SetOutputLimits(-PID_SETPOINT/2, PID_SETPOINT/2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
  moveForward(5.0);
  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

  
