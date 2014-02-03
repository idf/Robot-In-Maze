#include <PololuWheelEncoders.h> // read instruction: https://github.com/pololu/libpololu-avr/blob/master/src/README-Arduino.txt
#include <PID_v1.h>  // install from https://github.com/br3ttb/Arduino-PID-Library/
#include <Servo.h>   // Arduino Internal Library // RC (hobby) servo motors. 
#include <PID_AutoTune_v0.h> // install from https://github.com/br3ttb/Arduino-PID-AutoTune-Library
static boolean informReadyOnce = false;
static boolean countTicksForAngleOrDist = false;

Servo myServo;
const int WHEEL_DIAMETER = 6; // the motor shaft is mount the larger Pololu wheels (60mm)
/*
48 CPR quadrature encoder on the motor shaft, 
which provides 2249 counts per revolution (gear ratio: 47:1)
*/
const int COUNTS_PER_REVOLUTION = 2249; 
const float DISTANCE_PER_TICK_CM = (PI*WHEEL_DIAMETER)/COUNTS_PER_REVOLUTION;
const int sensorPin0 = 0, sensorPin1 = 1, sensorPin2 = 2, sensorPin3 = 3;
// analogWrite, max duty cycle
const int MAX_SPEED = 255;
const int MAX_SPEED_A = MAX_SPEED; // Max speed for motor based on pwn A (right)
const int MAX_SPEED_B = MAX_SPEED; // Max speed for motor based on pwn B (left) 

// Digital Pins
int pwm_right = 3;  // PWM control for motor outputs 1 and 2 is on digital pin 3
int pwm_left = 11;  // PWM control for motor outputs 3 and 4 is on digital pin 11
int dir_right = 12;  // Direction control for motor outputs 1 and 2 is on digital pin 12
int dir_left = 13;  // Direction control for motor outputs 3 and 4 is on digital pin 13
int caseCheck = 8;

double leftWheelTickCount = 0;  
double rightWheelTickCount = 0;
float leftTicksForAngleOrDist = 0;
float rightTicksForAngleOrDist = 0;

int LOOPTIME = 100;
unsigned long lastMilli = 0;

long timing = 0;
long previousLeftTick = 0;
long previousRightTick = 0;
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;
double SetpointMid, InputMid, OutputMid;
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft,1,1,0, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight,1,1,0, DIRECT);
PID midPID(&InputMid, &OutputMid, &SetpointMid,1,1,1, DIRECT);

float distanceS, deltaHeading, deltaX, deltaY;

int rightEncoderOne = 6;
int rightEncoderTwo = 7;
int leftEncoderOne = 8;
int leftEncoderTwo = 9;
long currentTicks = 0;

double setMotor;

///////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  // Set sensor control pins to be outputs
  pinMode(sensorPin0, OUTPUT);  
  pinMode(sensorPin1, OUTPUT);
  pinMode(sensorPin2, OUTPUT);
  pinMode(sensorPin3, OUTPUT);

  // Set motor control pins to be outputs
  pinMode(pwm_right, OUTPUT);  
  pinMode(pwm_left, OUTPUT);
  pinMode(dir_right, OUTPUT);
  pinMode(dir_left, OUTPUT);
  
  // Check for each ticks on wheels

  
  PololuWheelEncoders::init(rightEncoderOne,rightEncoderTwo,leftEncoderOne,leftEncoderTwo);
  
  deltaHeading = PI/2;
  leftPID.SetMode(AUTOMATIC);
  rightPID.SetMode(AUTOMATIC);
  midPID.SetMode(AUTOMATIC);
  leftPID.SetSampleTime(10);
  rightPID.SetSampleTime(10);
  midPID.SetSampleTime(10);
  SetpointLeft = 2000;
  SetpointRight = 2000;
  SetpointMid = 0;
  leftPID.SetOutputLimits(0, 3400);
  rightPID.SetOutputLimits(0, 3400);
  midPID.SetOutputLimits(-1000, 1000);
  
  Serial.begin(9600);  // sets the serial port to 9600
  timing = millis();
  myServo.attach(A4);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Uses specified distance(cm) to calculate the distance
float distCentimeter(float centimeter) {
  float divisibleTicks;
  if (centimeter <= 10) {
    divisibleTicks = 19.3; 
  }
  
  else if (centimeter <= 20) {
    divisibleTicks = 18.65;
  }
  
  else if (centimeter <= 35) {
    divisibleTicks = 19.05;
  }
  
  else if (centimeter <= 50) {
    divisibleTicks = 18.5;
  }
  
  else if (centimeter <= 75) {
    divisibleTicks = 18.9;
  }
  
  else {
    divisibleTicks = 18.7;
  }
  
  float ticksForOneCentimeter = 2249/divisibleTicks;
  float dist = ticksForOneCentimeter * centimeter;
  return dist;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Uses specified angle(degree) to calculate the turns
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
  
  float ticksForOneDegree = 2249/divisibleTicks;
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
  
  float ticksForOneDegree = 2249/divisibleTicks;
  float angleToTurn = ticksForOneDegree * angle;
  return angleToTurn;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Infinite loop routine for robot
void loop()
{
  if (informReadyOnce != true) {
    for (int i=0; i<10; i++)
      Serial.write("99/;");
    informReadyOnce = true;
  }

     
  int sensorValue0 = analogRead(sensorPin0);       // read analog input pin 0
  int sensorValue1 = analogRead(sensorPin1);       // read analog input pin 1 (left sensor from back view)
  int sensorValue2 = analogRead(sensorPin2);       // read analog input pin 2 (right sensor from back view)
  int sensorValue3 = analogRead(sensorPin3);
  
  if (Serial.available()) {
    executeCommand();
  }

  detectObstacle();
  readSensor(sensorValue0, sensorValue1, sensorValue2, sensorValue3); // prints sensors' readings
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void configureMotorForTurn() {
  //resetPololuTicks();
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      //double speed1 =  ((leftPololuCount -prevouslefttick)/1000);

      float leftTime = leftPololuCount - previousLeftTick;
      float rightTime = rightPololuCount - previousRightTick;

      float leftcm = DISTANCE_PER_TICK_CM * leftTime;
      float rightcm = DISTANCE_PER_TICK_CM * rightTime;

      leftTime = leftTime / (timez);
      leftTime = leftTime * 1000;
      rightTime = rightTime / (timez);
      rightTime = rightTime * 1000;
      

      InputLeft = leftTime;
      InputRight = rightTime;

      
      
      
      
      rightPID.Compute();
      leftPID.Compute();
      previousLeftTick = leftPololuCount;
      previousRightTick = rightPololuCount;
      timing = millis();

      runMotor(map(OutputRight,0,3400,150,255), map(OutputLeft,0,3400,150,255));
    }
  }
  else{
    halt();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void configureMotor() {

  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());

  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      //double speed1 =  ((leftPololuCount -prevouslefttick)/1000);

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
      
      //OutputMid
      SetpointRight = 2000 + map(OutputMid,-1000,1000,-2000,1400);
      rightPID.Compute();
      leftPID.Compute();
      previousLeftTick = leftPololuCount;
      previousRightTick = rightPololuCount;
      timing = millis();

      //runMotor(map(OutputRight,0,3400,50,150), map(OutputLeft,0,3400,50,150));
      runMotor(map(OutputRight,0,3400,150,255), map(OutputLeft,0,3400,150,255));
    }
  }
  else{
    halt();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void detectObstacle() {
  int sensorValue0, sensorValue1, sensorValue2, sensorValue3;
  int meanSensorValue0 = 0, meanSensorValue1 = 0, meanSensorValue2 = 0, meanSensorValue3 = 0;
  int sensorSampleLength = 10;
  int distCoveredBeforeObstacle = 0;
  
  for (int currSample = 0; currSample < sensorSampleLength; currSample++) {
    sensorValue0 = analogRead(sensorPin0);       // read analog input pin 0
    sensorValue1 = analogRead(sensorPin1);       // read analog input pin 1 (left sensor from back view)
    sensorValue2 = analogRead(sensorPin2);       // read analog input pin 2 (right sensor from back view)
    sensorValue3 = analogRead(sensorPin3);
    meanSensorValue0 += sensorValue0;
    meanSensorValue1 += sensorValue1;
    meanSensorValue2 += sensorValue2;
    meanSensorValue3 += sensorValue3;
  }
  
  // Calculate mean of sample sensor values for reliable reading
  meanSensorValue0 /= sensorSampleLength;
  meanSensorValue1 /= sensorSampleLength;
  meanSensorValue2 /= sensorSampleLength;
  meanSensorValue3 /= sensorSampleLength;

//  readSensor(meanSensorValue0, meanSensorValue1, meanSensorValue2); // prints sensors' readings
  
  int middleSensorObstacleDist = middleSensorDistanceMeasuredInCM(meanSensorValue0);
  int leftSensorObstacleDist = leftSensorDistanceMeasuredInCM(meanSensorValue1);
  int rightSensorObstacleDist = rightSensorDistanceMeasuredInCM(meanSensorValue2);
  int bottomSensorObstacleDist = bottomSensorDistanceMeasuredInCM(meanSensorValue3);
  
  int smallSensorRange = 19;
  int smallMIDDLESensorRange = 14;
  int largeFrontSensorRange = 80;
  
  if (middleSensorObstacleDist <= largeFrontSensorRange && bottomSensorObstacleDist<=smallMIDDLESensorRange) {
    if (leftSensorObstacleDist <= smallSensorRange && rightSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220 && meanSensorValue2 >= 220) {// && meanSensorValue1 >= 350 && meanSensorValue2 >= 350
      Serial.write("15/");
    }
    else if (rightSensorObstacleDist <= smallSensorRange && meanSensorValue2 >= 220) {
      Serial.write("14/");
    }
    else if (leftSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220) {
      Serial.write("13/");
    }
    else{
      Serial.write("12/");
    }
    Serial.print(middleSensorObstacleDist);
    Serial.write(";");
  }
  else if (middleSensorObstacleDist <= largeFrontSensorRange) {
    if (leftSensorObstacleDist <= smallSensorRange && rightSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220 && meanSensorValue2 >= 220) {
      Serial.write("11/");
    }
    else if (rightSensorObstacleDist <= smallSensorRange && meanSensorValue2 >= 220) {
      Serial.write("10/");
    }
    else if (leftSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220) {
      Serial.write("9/");
    }
    else{
      Serial.write("8/");
    }
    Serial.print(middleSensorObstacleDist);
    Serial.write(";");
  }
  else if (bottomSensorObstacleDist<=smallMIDDLESensorRange && meanSensorValue3 >= 220) {
    if (leftSensorObstacleDist <= smallSensorRange && rightSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220 && meanSensorValue2 >= 220) {
      Serial.write("7/;");
    }
    else if (rightSensorObstacleDist <= smallSensorRange && meanSensorValue2 >= 220) {
      Serial.write("5/;");
    }
    else if (leftSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220) {
      Serial.write("4/;");
    }
    else{
      Serial.write("1/;");
    }
  }
  else if (leftSensorObstacleDist <= smallSensorRange && rightSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220 && meanSensorValue2 >= 220) {//&& meanSensorValue1 >= 350 && meanSensorValue2 >= 350
    Serial.write("6/;");
  }
  else if(leftSensorObstacleDist <= smallSensorRange && meanSensorValue1 >= 220) {
    Serial.write("2/;");
  }
  else if (rightSensorObstacleDist <= smallSensorRange && meanSensorValue2 >= 220) {
    Serial.write("3/;");
  }
  else {
    Serial.write("16/;");
  }
 
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Read value from sensors
void readSensor(int sensorValue0, int sensorValue1, int sensorValue2, int sensorValue3) {
  int rightDistance = rightSensorDistanceMeasuredInCM(sensorValue2);
  int leftDistance = leftSensorDistanceMeasuredInCM(sensorValue1);
  int middleDistance = middleSensorDistanceMeasuredInCM(sensorValue0);
  int bottomDistance = bottomSensorDistanceMeasuredInCM(sensorValue3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Listen for serial data from raspberry
long readSerial() {
  unsigned long serialData = 0;
  int byteRead = 0;
  
  // Append command data until full command has been sent
  while (byteRead != '/') {
    byteRead = Serial.read(); // Serial read by a byte each time
    
    if (byteRead > 0 && byteRead != '/') {
      char byteChar[1];
      byteChar[0] = byteRead;
      byteRead = atoi(byteChar);
      serialData = serialData * 10 + byteRead;
      //print out the received data
      Serial.println(serialData, DEC);
    }
  }
    
  return serialData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void executeCommand() {
  //resetPololuTicks();
  float timeDelay, multiple;
  long serialData = readSerial();
  
  if(serialData==1){
    serialData = readSerial();
    moveForward(serialData);
    for (int i=0; i<10; i++)
      Serial.write("51/;");
  }
  else if(serialData==2){
    serialData = readSerial();
    turnLeft(serialData);
    for (int i=0; i<10; i++)
      Serial.write("52/;");
  }
  else if(serialData==3){
    serialData = readSerial();
    turnRight(serialData);
    for (int i=0; i<10; i++)
      Serial.write("53/;");
  }
  else if(serialData==4){
    halt();
  }
  else if(serialData==5){
    runMotor(pwm_left, pwm_right); 
  }
  else if(serialData==6){
    if(serialData!=0){
      serialData = readSerial();
      servoTurn(serialData);
    }
  }

  informReadyOnce = false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void resetPololuTicks() {
  PololuWheelEncoders::getCountsAndResetM1();
  PololuWheelEncoders::getCountsAndResetM2();
}

float getDistance() {
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  float leftcm = DISTANCE_PER_TICK_CM * leftPololuCount;
  float rightcm = DISTANCE_PER_TICK_CM * rightPololuCount;
  float averageDist = (leftcm + rightcm) / 2;
  return averageDist;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Run robot at max speed
void runMotor(int moveSpeedA, int moveSpeedB) {
  analogWrite(pwm_right, moveSpeedA);  
  analogWrite(pwm_left, moveSpeedB);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Move forward
void moveForward(float dist) {
  //resetPololuTicks();
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  
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
    
    //avgTicksForAngleOrDist = leftTicksForAngleOrDist;
    configureMotor();
  }

  

  digitalWrite(dir_left, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_right, HIGH);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Turn right
void turnRight(int angle) {
  resetPololuTicks();
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high (right)
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low (left)
  
  float noOfTicksForAngle = turnAngleR(angle);
  
  // ------ Angle to ticks formula ------- //

  

  
  float avgTicksForAngleOrDist = 0;
  long firstLeftCount = abs(PololuWheelEncoders::getCountsM1());
  long firstRightCount = abs(PololuWheelEncoders::getCountsM2());
  
  while (avgTicksForAngleOrDist < noOfTicksForAngle) { //noOfTicksForAngle - change to 'angle' for other formula
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
    leftTicksForAngleOrDist = leftTicksForAngleOrDist - firstLeftCount;
    
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
    rightTicksForAngleOrDist = rightTicksForAngleOrDist - firstRightCount;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
    //avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) * 0.02795;
    configureMotor();
  }
  

  
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
  //reverse(0.2);
  resetPololuTicks();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Turn left
void turnLeft(int angle) {
  resetPololuTicks();
  digitalWrite(dir_right, LOW);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH); //Set motor direction, 3 high, 4 low
  //turnAngle(angle+1);
  
  float noOfTicksForAngle = turnAngle(angle);

  float avgTicksForAngleOrDist = 0;
  
  long firstLeftCount = abs(PololuWheelEncoders::getCountsM1());
  long firstRightCount = abs(PololuWheelEncoders::getCountsM2());
  
  while (avgTicksForAngleOrDist < noOfTicksForAngle) {
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
    leftTicksForAngleOrDist = leftTicksForAngleOrDist - firstLeftCount;
    
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
    rightTicksForAngleOrDist = rightTicksForAngleOrDist - firstRightCount;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
    configureMotor();
  }
  
  
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
  resetPololuTicks();
}

// Reverse backwards
void reverse(float dist) {
  runMotor(100, 100); //Set both motors to run at 100% duty cycle (fast)
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH);  //Set motor direction, 3 high, 4 low
  delay(70);
  
  //resetTicksForAngleOrDistance();
  
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  delay(30);
  halt();
  //resetPololuTicks();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Stop movement
void halt() 
{ 
  analogWrite(pwm_right, 0);
  analogWrite(pwm_left, 0);
  delay(100);
  //resetPololuTicks();
}

float middleSensorDistanceMeasuredInCM(int middleSensor) {
  float distance = (6762 / (middleSensor - 9)) - 4;
  return distance;
}

float leftSensorDistanceMeasuredInCM(int leftSensor) {
  float distance = (6762 / (leftSensor - 9)) - 4;
  return distance;
}

float rightSensorDistanceMeasuredInCM(int rightSensor) {
  float distance = (6762 / (rightSensor - 9)) - 4;
  return distance;
}
float bottomSensorDistanceMeasuredInCM(int btmSensor) {
  float distance = (6762 / (btmSensor - 9)) - 4;
  return distance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Servo Turn
void servoTurn(int angle) {
  if(angle>=0 && angle<5){
    myServo.write(179);
  }
  else if(angle>=5 && angle<15){
    myServo.write(168.5);
  }
  else if(angle>=15 && angle<25){
    myServo.write(158);
  }
  else if(angle>=25 && angle<35){
    myServo.write(147.5);
  }
  else if(angle>=35 && angle<45){
    myServo.write(137);
  }
  else if(angle>=45 && angle<55){
    myServo.write(126.5);
  }
  else if(angle>=55 && angle<65){
    myServo.write(116);
  }
  else if(angle>=65 && angle<75){
    myServo.write(105.5);
  }
  else if(angle>=75 && angle<85){
    myServo.write(95);
  }
  else if(angle>=85 && angle<95){
    myServo.write(83);
  }  
  else if(angle>=95 && angle<105){
    myServo.write(73.6);
  }
  else if(angle>=105 && angle<115){
    myServo.write(64.4);
  }
  else if(angle>=115 && angle<125){
    myServo.write(55.2);
  }
  else if(angle>=125 && angle<135){
    myServo.write(46);
  }
  else if(angle>=135 && angle<145){
    myServo.write(36.8);
  }
  else if(angle>=145 && angle<155){
    myServo.write(27.6);
  }
  else if(angle>=155 && angle<165){
    myServo.write(18.4);
  }
  else if(angle>=165 && angle<175){
    myServo.write(9.2);
  }
  else if(angle>=175 && angle<=180){
    myServo.write(0);
  }
  
}
///////////////////////////////////////////////////////////////////////////////////////////////////