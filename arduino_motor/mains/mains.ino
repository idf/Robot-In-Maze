// use mains instead of main since main is the keyword
#include <DualVNH5019MotorShield.h>
//#include <PololuWheelEncoders.h>
#include <PID_v1.h>  
#include <Ultrasound.h>
// #include <PID_AutoTune_v0.h>

#include "globals.h"
#include "Config.h"
//#include "PinChangeInt.h"
// TAB SIZE 2, whitespaces as tab
///////////////////////////////////////////////////////////////////////////////////////////////////

  int minx = 0;
  int maxx = 0;
  int miny = 0;
  int maxy = 0;



void setup(void)
{
  Serial.begin(9600); // data rate in bits per second 
  motorShield.init();

  //PololuWheelEncoders::init(Pin::M1_ENCODER_A, Pin::M1_ENCODER_B, Pin::M2_ENCODER_A, Pin::M2_ENCODER_B); // 4 pins required
  //servo_ir->init();
  errorCumulator->init();
  frontEye->init(); 
  sideEye->init();


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
  //frontEye->test_readings();
  //Serial.print(F("Get Front Reading: ")); Serial.println(frontEye->get_reading());
  //sideEye->test_readings();
  //Serial.print(F("Get Right Reading: ")); Serial.println(sideEye->get_reading_right());
  //sideEye->test_readings();
  //moveForward(10);
  //printCounts();
  //turnLeft(90);
  //printCounts();
  //errorCumulator->compass->printAxis();

  //delay(1000);

  //turnRight(90);
  //moveForward(80);
  //turnLeft(90);
  //motorShield.setSpeeds(-400, -400);
  //serialCommnder->send_command_complete(1, 200);
  //Serial.println(errorCumulator->get_compass_read());
  //serialCommnder->receive_exec_command();
  //serialCommnder->send_sensor_readings(100, 30, 30);
  getSensorReadings();

  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////



