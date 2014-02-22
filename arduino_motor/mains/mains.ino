// use mains instead of main since main is the keyword
#include <DualVNH5019MotorShield.h>
#include <PololuWheelEncoders.h>
#include <PID_v1.h> 
// #include <PID_AutoTune_v0.h>

#include "globals.h"
#include "Pin.h"
#include "Config.h"
#include "ErrorCumulator.h"

// TAB SIZE 2, whitespaces as tab
///////////////////////////////////////////////////////////////////////////////////////////////////
void setup(void)
{
  Serial.begin(9600); // data rate in bits per second 
  motorShield.init();
  PololuWheelEncoders::init(Pin::M1_ENCODER_A, Pin::M1_ENCODER_B, Pin::M2_ENCODER_A, Pin::M2_ENCODER_B); // 4 pins required
  errorCumulator->init();
  //servo_ir->init();

  leftPID.SetMode(AUTOMATIC);
  rightPID.SetMode(AUTOMATIC);
  midPID.SetMode(AUTOMATIC);

  leftPID.SetSampleTime(Config::SAMPLE_TIME); 
  rightPID.SetSampleTime(Config::SAMPLE_TIME); 
  midPID.SetSampleTime(Config::SAMPLE_TIME);

  SetpointLeft = Config::PID_SETPOINT;
  SetpointRight = Config::PID_SETPOINT;
  SetpointMid = 0;

  // for controlling the speed 
  
  leftPID.SetOutputLimits(Config::PID_LOWER_LIMIT/2, Config::PID_UPPER_LIMIT/2);
  rightPID.SetOutputLimits(Config::PID_LOWER_LIMIT/2, Config::PID_UPPER_LIMIT/2);
  midPID.SetOutputLimits(-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2);
    
  resetPololuTicks();
  serialCommnder->send_ready_signal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
  //resetPololuTicks();
  moveForward(20);
  moveForward(20);
  moveForward(20);
  //errorCumulator->reset_dead_reckoning();
  turnLeft(90);
  //errorCumulator->reset_dead_reckoning();
  

  //errorCumulator->reset_dead_reckoning();
 
  
  errorCumulator->print_dead_reckoning();
  //turnRight(90);
  // moveForward(80);
  //turnLeft(90);
  //motorShield.setSpeeds(-400, -400);
  //serialCommnder->send_command_complete(1, 200);
  //Serial.println(errorCumulator->get_compass_read);
  //serialCommnder->receive_exec_command();
  //servo_ir->servoScan(60, 120);
  //servo_ir->print_dist_array();
  delay(1000); //to slow down the serial print

  //delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

  

