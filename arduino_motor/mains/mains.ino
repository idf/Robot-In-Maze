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
  //serialCommnder->send_ready_signal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
  //warmingUp();
  //resetPololuTicks();
  //frontEye->test_readings();
  //Serial.print(F("Get Front Reading: ")); Serial.println(frontEye->get_reading());
  //sideEye->test_readings();
  //Serial.print(F("Get Right Reading: ")); Serial.println(sideEye->output_reading_right());
  sideEye->test_readings();
  //moveForward(150);     
  //goStraightlineCheckpoint();
  //printCounts();
  //turnRight(90);
  //printCounts();
  //errorCumulator->compass->printAxis();
  
  //turnLeft(90);
  //moveForward(80);
  //turnLeft(90);
  //motorShield.setSpeeds(-400, -400);
  //serialCommnder->send_command_complete(1, 200);
  //Serial.println(errorCumulator->get_compass_read());
  // serialCommnder->receive_exec_command();

  //servo_ir->servoScan(60, 120);
  //servo_ir->print_dist_array();
  //delay(5000); //to slow down the serial print


  //getSensorReadings();
  //obstacle_checkpoint();
  //softwareProjectLabSqure();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void softwareProjectLabSqure() {
  for(int i=0; i<11; i++) {
    moveForward(10);
    printCounts();
  } 

  turnRight(90);
  printCounts();
  for(int i=0; i<6; i++) {
    moveForward(10);
    printCounts();
  }
  turnRight(90);
  printCounts();
}


void warmingUp() {
  static bool first_time = true;
  if(first_time) {
    moveForward(10);
    delay(100);
    moveForward(10);
    delay(10000);
    first_time = false;
  }
}
