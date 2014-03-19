// use mains instead of main since main is the keyword
#include <DualVNH5019MotorShield.h>
//#include <PololuWheelEncoders.h>
#include <Ultrasound.h>
// #include <PID_AutoTune_v0.h>

#include "globals.h"
#include "Config.h"
//#include "PinChangeInt.h"
// TAB SIZE 2, whitespaces as tab
///////////////////////////////////////////////////////////////////////////////////////////////////

//int minx = 0;
//int maxx = 0;
//int miny = 0;
//int maxy = 0;


void setup(void)
{
  Serial.begin(9600); // data rate in bits per second 
  motorShield.init();
  //PololuWheelEncoders::init(Pin::M1_ENCODER_A, Pin::M1_ENCODER_B, Pin::M2_ENCODER_A, Pin::M2_ENCODER_B); // 4 pins required
  //servo_ir->init();
  errorCumulator->init();
  frontEye->init(); 
  sideEye->init();
  pidMgr->init();
  resetPololuTicks();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{
  // integration mode
  //warmingUp();
  //serialCommnder->receive_exec_command();
  ///*
  //turnLeft(90); delay(400); printCounts(); 
  //*/
  //calibrator->calibrate(6); delay(10000);
  //turnRight(90); delay(400);
  
  //delay(10000);
  //calibrator->test_calibrate();
  //delay(10000);
  //delay(10000);
  
  //getSensorReadings(); delay(1000);


  //delay(1000);
  //Serial.print(F("Get Right Reading: ")); Serial.println(sideEye->output_reading_right());

 
  //moveForward(10);     

  frontEye->test_readings();
  //sideEye->test_readings();


  
  //Serial.print(F("Get Front Reading: ")); Serial.println(frontEye->get_reading());
  //sideEye->test_readings();
  //Serial.print(F("Get Right Reading: ")); Serial.println(sideEye->output_reading_right());
  //sideEye->test_readings();
  //goStraightlineCheckpoint();
  /*
  pidMgr->setScale(1.75);
  moveForward(20);
  printCounts(); delay(500);
  pidMgr->restore();
  */
  /*
  turnRight(90); 
  turnLeft(90);
  printCounts();
  delay(1000);
  */
 
  
  //moveForward(10);
  //printCounts();
  //delay(1000);
  //errorCumulator->compass->printAxis();
  
  //turnLeft(90);
  // testing
  //turnLeft(90);
  //serialCommnder->send_command_complete(1, 200);
  //Serial.println(errorCumulator->get_compass_read());
  



  //getSensorReadings();
  //obstacle_checkpoint();
  //softwareProjectLabSqure();
  
}

void warmingUp() {
  static bool first_time = true;
  if(first_time) {
    for(int i=0; i<1; i++) {
      moveForward(20); delay(100);
      turnRight(90); delay(100);
      moveForward(20); delay(100);
      turnLeft(90); delay(100);
      moveForward(20); delay(100);
    }
    first_time = false;
    delay(10000);
  }
  
}

