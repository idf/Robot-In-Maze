#include <DualVNH5019MotorShield.h>
#include <PololuWheelEncoders.h>
#include <PID_v1.h> 
// TAB SIZE 2, whitespaces as tab
///////////////////////////////////////////////////////////////////////////////////////////////////
// M1 on LEFT
// M2 on RIGHT 
// Constants START
const int MAX_SPEED_ANALOG = 255; // Built-in PWM
const int MAX_SPEED = 400;
const int MID_SPEED = 235;
const int SPEED = MAX_SPEED;

const int WHEEL_DIAMETER = 6; // the motor shaft is mount the larger Pololu wheels (60mm)
const int COUNTS_PER_REVOLUTION = 2249; // 48 CPR quadrature encoder on the motor shaft,  which provides 2249 counts per revolution (gear ratio: 47:1)
const float DISTANCE_PER_TICK_CM = (PI*WHEEL_DIAMETER)/COUNTS_PER_REVOLUTION;

const int PID_UPPER_LIMIT = 3400;
const int PID_SETPOINT = 2000;
// Constants END

// Digital Pins START
/* 
Built-in digital pin:
Default Pin Mapping 
Arduino Pin VNH5019   Basic Function
Digital 2   M1INA     Motor 1 *direction input A
Digital 4   M1INB     Motor 1 *direction input B
Digital 6   M1EN/DIAG Motor 1 enable input/fault output
Digital 7   M2INA     Motor 2 *direction input A
Digital 8   M2INB     Motor 2 *direction input B
Digital 9   M1PWM     Motor 1 *speed input
Digital 10  M2PWM     Motor 2 *speed input
Digital 12  M2EN/DIAG Motor 2 enable input/fault output

Analog  A0  M1CS      Motor 1 current sense output
Analog  A1  M2CS      Motor 2 current sense output

*/
// Encoder: two channels for both speed and direction 
// remaining least-significant-bit pins
const int rightEncoderA = 0;
const int rightEncoderB = 1;
const int leftEncoderA = 3;
const int leftEncoderB = 5;

// Digital Pins END

// Analog Pins START

// Analog Pins END

// Ticking
float leftTicksForAngleOrDist = 0;
float rightTicksForAngleOrDist = 0;

long timing = 0;
long previousLeftTick = 0;
long previousRightTick = 0;

// PID
// variables upper case by documentation 
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;
double SetpointMid, InputMid, OutputMid;
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 1, 1, 0, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight, 1, 1, 0, DIRECT);
PID midPID(&InputMid, &OutputMid, &SetpointMid, 1, 1, 1, DIRECT);

// Deduced Reckoning 
float deltaHeading, deltaX, deltaY;

DualVNH5019MotorShield motorShield;
///////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600); // data rate in bits per second 
  Serial.println("Dual VNH5019 Motor Shield");
  motorShield.init();

  // Encoder
  PololuWheelEncoders::init(rightEncoderA, rightEncoderB, leftEncoderA, leftEncoderB);
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
void loop()
{
  moveForward(5.0);
  delay(1000);
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
  
  long firstLeftCount = abs(PololuWheelEncoders::getCountsM1());
  long firstRightCount = abs(PololuWheelEncoders::getCountsM2());

  
  while (avgTicksForAngleOrDist < noOfTicksForDist) {
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
    leftTicksForAngleOrDist = leftTicksForAngleOrDist - firstLeftCount;
    
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
    rightTicksForAngleOrDist = rightTicksForAngleOrDist - firstRightCount;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;

    configureMotor(1, 1);
  }

  
  motorShield.setBrakes(MAX_SPEED, MAX_SPEED);
  delay(40);
  halt();
}

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


      if(isM1Forward!=1 && isM1Forward!=-1 || isM2Forward!=1 && isM2Forward!=-1) {
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
float distCentimeter(float centimeter) 
{
  float divisibleTicks;
  if (centimeter <= 10) {
    divisibleTicks = 19.3; 
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
  float ticksForOneCentimeter = COUNTS_PER_REVOLUTION/divisibleTicks;
  float dist = ticksForOneCentimeter * centimeter;
  return dist;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void resetPololuTicks() 
{
  PololuWheelEncoders::getCountsAndResetM1();
  PololuWheelEncoders::getCountsAndResetM2();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
