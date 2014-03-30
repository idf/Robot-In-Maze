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
void setup(void)
{
  Serial.begin(9600); // data rate in bits per second 
  motorShield.init();
  //PololuWheelEncoders::init(Pin::M1_ENCODER_A, Pin::M1_ENCODER_B, Pin::M2_ENCODER_A, Pin::M2_ENCODER_B); // 4 pins required
  errorCumulator->init();
  eyes->init();
  calibrator->init();
  pidMgr->init();
  resetPololuTicks();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void loop(void)
{ 
  
  //warmingUp(); serialCommnder->receive_exec_command();
  //warmingUp(); pidMgr->setScale(2.5`); moveForward(10); pidMgr->restore(); printCounts(); delay(400); 
  //turnRight(90);  printCounts(); delay(400);

  //calibrator->calibrate(6); delay(10000);
  calibrator->test_calibrate(); delay(1000);
  //eyes->test_readings();
  //getSensorReadings(); delay(1000);

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
    if(Config::verbose){
      Serial.print(F("warmingUp finished, waiting for 1s"));
      delay(1000);
    }
  }
}

