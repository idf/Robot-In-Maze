#include "globals.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
float distCentimeter(float centimeter) 
{
  float divisibleTicks;
  if (centimeter <= 10) {
    divisibleTicks = 18.7; 
  } else if (centimeter <= 20) {
    divisibleTicks = 18.65;
  } else if (centimeter <= 35) {
    divisibleTicks = 19.05;
  } else if (centimeter <= 50) {
    divisibleTicks = 18.5;
  } else if (centimeter <= 75) {
    divisibleTicks = 18.9;
  } else {
    divisibleTicks = 18.7;
  }
  float ticksForOneCentimeter = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float dist = ticksForOneCentimeter * centimeter;
  return dist;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void resetPololuTicks() 
{
  //PololuWheelEncoders::getCountsAndResetM1();
  //PololuWheelEncoders::getCountsAndResetM2();
  leftCnt = 0;
  rightCnt = 0;
  previousLeftTick = 0;
  previousRightTick = 0;

}
///////////////////////////////////////////////////////////////////////////////////////////////////
void printCounts()
{
  long leftPololuCount = leftCnt;//abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = rightCnt;//abs(PololuWheelEncoders::getCountsM2());
  Serial.print(F("LEFT COUNT (getCountsM1): ")); Serial.println(leftPololuCount);
  Serial.print(F("RIGHT COUNT (getCountsM2): ")); Serial.println(rightPololuCount);
  // Serial.print(F("previousLeftTick: ")); Serial.println(previousLeftTick);
  // Serial.print(F("previousRightTick: ")); Serial.println(previousRightTick);


  //Serial.println(PololuWheelEncoders::checkErrorM1());
  //Serial.println(PololuWheelEncoders::checkErrorM2());

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
float turnAngle(int angle) {
  float divisibleTicks;
  if (angle <= 90) {
    divisibleTicks = 126.7;  // 127.5
  }
  
  else if (angle <= 180) {
    divisibleTicks = 125.5;  // 64.5
  }

  else if (angle <= 270) {
    divisibleTicks = 126.7;
  } 
  
  else if (angle <= 450) {
    divisibleTicks = 125.6;
  }
  
  else if (angle <= 540) {
    divisibleTicks = 125;
  }
  
  else if (angle <= 630) {
    divisibleTicks = 125;
  }
  else {
    divisibleTicks = 125;
  }
  
  float ticksForOneDegree = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float angleToTurn = ticksForOneDegree * angle;
  return angleToTurn;
}

float turnAngleR(int angle) {
  float divisibleTicks;
  if (angle <= 90) {
    divisibleTicks = 127.5;  // 127.5
  }
  
  else if (angle <= 180) {
    divisibleTicks = 125.5;  // 64.5
  }

  else if (angle <= 270) {
    divisibleTicks = 126.7;
  } 
  
  else if (angle <= 450) {
    divisibleTicks = 125.6;
  }
  
  else if (angle <= 540) {
    divisibleTicks = 125;
  }
  
  else if (angle <= 630) {
    divisibleTicks = 125;
  }
  else {
    divisibleTicks = 125;
  }
  
  float ticksForOneDegree = Config::COUNTS_PER_REVOLUTION/divisibleTicks;
  float angleToTurn = ticksForOneDegree * angle;
  return angleToTurn;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void setScale(double scale) {
  
  setScaleRight(scale);
  setScaleLeft(scale);
  SetpointMid *= scale; // always 0  
  midPID.SetOutputLimits(-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2);
}
void setScaleLeft(double scale) {
  SetpointLeft *= scale;

  Config::PID_SETPOINT_LEFT *= scale;
  Config::PID_UPPER_LIMIT_LEFT *= scale;
  Config::PID_LOWER_LIMIT_LEFT *= scale;

  Config::MAX_SPEED_LEFT *= scale;
  Config::TARGET_SPEED_LEFT *= scale;
  Config::MIN_SPEED_LEFT *= scale;

  leftPID.SetOutputLimits(Config::PID_LOWER_LIMIT_LEFT, Config::PID_UPPER_LIMIT_LEFT);
}
void setScaleRight(double scale) {
  SetpointRight *= scale;

  Config::PID_SETPOINT_RIGHT *= scale;
  Config::PID_UPPER_LIMIT_RIGHT *= scale;
  Config::PID_LOWER_LIMIT_RIGHT *= scale;


  Config::MAX_SPEED_RIGHT *= scale;
  Config::TARGET_SPEED_RIGHT *= scale;
  Config::MIN_SPEED_RIGHT *= scale;

  rightPID.SetOutputLimits(Config::PID_LOWER_LIMIT_RIGHT, Config::PID_UPPER_LIMIT_RIGHT);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Replaced by const TICKS_PER_DEGREE
double angle_to_ticks(double angle) {
 return (Config::WHEELS_INTERVAL/2)*(PI/180)*(angle/Config::DISTANCE_PER_TICK_CM);
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
void getSensorReadings() {
  serialCommnder->send_sensor_readings(
    frontEye->output_reading_ultra(),
    frontEye->output_reading_ir_left(),
    frontEye->output_reading_ir_right(),
    sideEye->output_reading_left(),
    sideEye->output_reading_right()
    );
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void obstacle_checkpoint() {
  turnLeft(90);
  moveForward(10);

  turnLeft(90);
  moveForward(10);
  turnLeft(90);
  moveForward(10);
  turnRight(90);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void goStraightlineCheckpoint(){
 if (frontEye->output_reading_ultra()!=-1&&frontEye->get_ulra_reading()<=10){
  turnRight(90);
  moveForward(20);
  turnLeft(90);
  moveForward(40);
  turnLeft(90);
  moveForward(20);
  turnRight(90);
  //finish
 }
 moveForward(10);
}


void extensionCheckpoint(){
 if (frontEye->output_reading_ultra()!=-1&&frontEye->get_ulra_reading()<=10){
  turnRight(45);
  moveForward(20);
  turnLeft(90);
  moveForward(40);
  turnLeft(90);
  moveForward(20);
  turnRight(90);
  //finish
 }
 moveForward(10);
}
