#include <PololuWheelEncoders.h>
#include <PID_v1.h>
//#include <PinChangeInt.h>

//#include <DistanceGP2Y0A02YK.h>
//#include <DistanceGP2Y0A21YK.h>
#include <Servo.h>

/* Serial write for sensing obstacles: 0 = ready to receive data from raspberry pi,  
   1 = center sensor only, 2 = left sensor only, 3 = right sensor only, 
   4 = center and left sensors, 5 = center and right sensors, 6 = left and right sensors,
   7 = all three sensors, 8 = default none sensed
   
   Serial read: 1 = move forward, 2 = turn left, 3 = turn right, 4 = stop */

static boolean informReadyOnce = false;
static boolean countTicksForAngleOrDist = false;

Servo myServo;
//PololuWheelEncoders encoder;

//const float pi = 3.14159;
const int wheelDiameter = 6; 
const int countsPerRevolution = 2249;
const float thresh = 20;
//const float noOfTicksPerCm = 2249/(PI*6);
const float noOfCmPerTick = (PI*6)/2249;
//const float noOfCmPerDegree = (PI*17.2)/360;
//float leftCounter, rightCounter;

// Analog Pins; middle sensor [0], left sensor [1], right sensor [2]
const int sensorPin0 = 0, sensorPin1 = 1, sensorPin2 = 2, sensorPin3 = 3;
const int maxSpeedA = 225; // Max speed for motor based on pwn A (right)
const int maxSpeedB = 225; // Max speed for motor based on pwn B (left)

// Digital Pins
int pwm_right = 3;  // PWM control for motor outputs 1 and 2 is on digital pin 3
int pwm_left = 11;  // PWM control for motor outputs 3 and 4 is on digital pin 11
int dir_right = 12;  // Direction control for motor outputs 1 and 2 is on digital pin 12
int dir_left = 13;  // Direction control for motor outputs 3 and 4 is on digital pin 13
int caseCheck = 8;

//PI variables
//double leftOut, rightOut;
//double err;
//double err2 = 0;
//double Kp = 4 ;
//double Ki = 5;

// 2249 ticks for one full revolution
double leftWheelTickCount = 0;  
double rightWheelTickCount = 0;
float leftTicksForAngleOrDist = 0;
float rightTicksForAngleOrDist = 0;

//int delta_left = 0;
//int delta_right = 0;
//double setPointL = 0;
//double setPointR = 0;
//double setPointMid;
//double actual_speed_left = 0;
//double actual_speed_right = 0;
//double actual_speed;
//double pwm_mid;
//double pwm_left;
//double pwm_right;

//int angleTurn = 0;
//int distanceMove = 0;

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

//unsigned long leftPololuCount = 0;
//unsigned long rightPololuCount = 0;

//int encoderLeftCount;
//int encoderRightCount;

double setMotor;

//static long last_count_left = 0;
//static long last_count_right = 0;

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
//  PCintPort::attachInterrupt(6, incLeftCount, CHANGE);
//  PCintPort::attachInterrupt(7, incLeftCount, CHANGE);
//  PCintPort::attachInterrupt(8, incRightCount, CHANGE);
//  PCintPort::attachInterrupt(9, incRightCount, CHANGE);
  
  PololuWheelEncoders::init(rightEncoderOne,rightEncoderTwo,leftEncoderOne,leftEncoderTwo);
  
  //PID_left.SetMode(AUTOMATIC);
  //PID_right.SetMode(AUTOMATIC);
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
  //PID_mid.SetMode(AUTOMATIC);
  //PID_mid.SetSampleTime(100);
  //PID_mid.SetOutputLimits(0, 225);
  //PID_left.SetSampleTime(100);
  //PID_right.SetSampleTime(100);
  
//  dist1 = sensor1.getDistanceCentimeter();
  //encoder
//  encoder.init(7,6,8,9);
 
  myServo.attach(A4);
 //myServo.write(90);
// delay(500);yy
 //myServo.detach(); 
  
  //moveForward(20);
  /*
for(int i=0; i<2; i++) {
  delay(1000);
moveForward(50);
resetPololuTicks();
delay(1000);
turnRight(90);
resetPololuTicks();
delay(1000);
moveForward(50);
resetPololuTicks();
delay(1000);
turnLeft(90);
resetPololuTicks();
delay(1000);
moveForward(50);
resetPololuTicks();
delay(1000);
turnLeft(180);
resetPololuTicks();
}
*/


/*
for(int i=0; i<10; i++) {
  moveForward(20);
  resetPololuTicks();
  delay(1000);
  turnLeft(90);
}

for(int i=0; i<10; i++) {
  moveForward(20);
  resetPololuTicks();
  delay(1000);
  turnRight(90);
}
*/
//moveForward(200);
//turnRight(1440);
//turnRight(180); 
//delay(5000);
//turnRight(90);
}

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
// Infinite loop routine for robot
void loop()
{
  if (informReadyOnce != true) {
    for (int i=0; i<10; i++)
      Serial.write("99/;");
    informReadyOnce = true;
  }
  
  //leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  //rightPololuCount = abs(PololuWheelEncoders::getCountsM2()); 
     
  int sensorValue0 = analogRead(sensorPin0);       // read analog input pin 0
  int sensorValue1 = analogRead(sensorPin1);       // read analog input pin 1 (left sensor from back view)
  int sensorValue2 = analogRead(sensorPin2);       // read analog input pin 2 (right sensor from back view)
  int sensorValue3 = analogRead(sensorPin3);
  
//  runRobotAuto(sensorValue0, sensorValue1, sensorValue2); // run robot with internal logic for obstable avoidance
  if (Serial.available()) {
    executeCommand();
  }
  //Serial.println("outside millis");
  //if((millis()-lastMilli) >= LOOPTIME){
     //lastMilli = millis();
     //Serial.println("in millis");
     //leftMotorConfigure(); 
     //rightMotorConfigure();
  //}
  
  //configureMotor();
//  leftCounter = encoder.getCountsAndResetM1();
//  rightCounter = encoder.getCountsAndResetM2();
   
  
  detectObstacle();
  readSensor(sensorValue0, sensorValue1, sensorValue2, sensorValue3); // prints sensors' readings
  
  //PIController();
}

void configureMotorForTurn() {
  //resetPololuTicks();
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  /*Serial.print("leftPololuCount =");
    Serial.print(leftPololuCount);
    Serial.print("\t");
    Serial.print("rightPololuCount =");
    Serial.println(rightPololuCount);*/
  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      //double speed1 =  ((leftPololuCount -prevouslefttick)/1000);

      float leftTime = leftPololuCount - previousLeftTick;
      float rightTime = rightPololuCount - previousRightTick;

      float leftcm = noOfCmPerTick * leftTime;
      float rightcm = noOfCmPerTick * rightTime;
      /*float distanceToTravel = (leftcm + rightcm)/2;
      deltaHeading =  (leftcm-rightcm) / 17.2 + deltaHeading;
      deltaX = distanceToTravel * cos(deltaHeading) + deltaX;
      deltaY = distanceToTravel * sin(deltaHeading) + deltaY;*/
      leftTime = leftTime / (timez);
      leftTime = leftTime * 1000;
      rightTime = rightTime / (timez);
      rightTime = rightTime * 1000;
      //InputMid = deltaX * 10000;

      InputLeft = leftTime;
      InputRight = rightTime;

      //midPID.Compute();
      
      //OutputMid
      //SetpointRight = 2000 + map(OutputMid,-1000,1000,-2000,1400);
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

void configureMotor() {
  //resetPololuTicks();
  //Serial.println(deltaX);
  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  /*Serial.print("leftPololuCount =");
    Serial.print(leftPololuCount);
    Serial.print("\t");
    Serial.print("rightPololuCount =");
    Serial.println(rightPololuCount);*/
  //SetpointMid = 0;
/*  int sensorLeft = analogRead(sensorPin1);
  //int detectedObstacle = 0;
  
  while (sensorLeft > 200) {
    Serial.print("new deltaX = ");
    Serial.println(deltaX);
    SetpointMid = -500;*/
  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      //double speed1 =  ((leftPololuCount -prevouslefttick)/1000);

      float leftTime = leftPololuCount -previousLeftTick;
      float rightTime = rightPololuCount -previousRightTick;

      float leftcm = noOfCmPerTick * leftTime;
      float rightcm = noOfCmPerTick * rightTime;
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
  
  //sensorLeft = analogRead(sensorPin1);
  //detectedObstacle = 1;
  //}
  
  /*if (detectedObstacle = 1) {
    long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
    long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
    currentTicks = (leftPololuCount + rightPololuCount) / 2;
    resetPololuTicks();
    detectedObstacle = 0;
  }*/
  
  /*if (sensorLeft < 200) {
    SetpointMid = 0;
    //Serial.print("new deltaX = ");
    Serial.println(deltaX);
  if(leftPololuCount < 30000){
    if(millis() - timing >= 10){
      leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
      rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
      long timez = millis()-timing;
      //double speed1 =  ((leftPololuCount -prevouslefttick)/1000);

      float leftTime = leftPololuCount -previousLeftTick;
      float rightTime = rightPololuCount -previousRightTick;

      float leftcm = noOfCmPerTick * leftTime;
      float rightcm = noOfCmPerTick * rightTime;
      float distanceToTravel = (leftcm + rightcm)/2;
      deltaHeading =  (leftcm-rightcm) / 17.2 + deltaHeading;
      deltaX = distanceToTravel * cos(deltaHeading) + deltaX;
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

      runMotor(map(OutputRight,0,3400,150,255), map(OutputLeft,0,3400,150,255));
    }
  }
  else{
    halt();
  }
  }*/
}

/*void configureMotor() {
  actual_speed = (encoderLeftCount + encoderRightCount)/2;
  PID_mid.Compute();
  
  encoderLeftCount = abs(PololuWheelEncoders::getCountsM1());
  encoderRightCount = abs(PololuWheelEncoders::getCountsM2());
  
  long differenceCount = encoderRightCount - encoderLeftCount;
  
  if((pwm_mid > 100)){

    //check1 = true;
    //checkStop = false;

    if(differenceCount > 50)
    {
      setMotor = 50;
    }
    else if(differenceCount > 40)
    {
      setMotor = 47;
    }
    else if(differenceCount > 30)
    {
      setMotor = 45;
    }
    else if(differenceCount > 20)
    {
      setMotor = 42;
    }
    else if(differenceCount > 10)
    {
      setMotor = 40;
    }
    else if (differenceCount > 0){
      setMotor = 26;
    }


    else if(differenceCount < -50)
    {
      setMotor = -35;
    }
    else if(differenceCount < -40)
    {
      setMotor = -28;
    }
    else if(differenceCount < -30)
    {
      setMotor = -22;
    }
    else if(differenceCount < -20)
    {
      setMotor = -21;
    }
    else if(differenceCount < -10)
    {
      setMotor = -20;
    }
    else if (differenceCount < 0){
      setMotor =  26;
    }
    else {
      setMotor = 23;
    }
    pwm_left = pwm_mid + setMotor ;
    pwm_right = pwm_mid - setMotor;
    //    Serial.println(myLeftPWM);  
    //    Serial.println(myRightPWM);
    //    Serial.println();
    runMotor(pwm_left, pwm_right);
    //setPWM(pwm_left, pwm_right);
    //delay(50);

  }
  else { 
    pwm_left = 0;
    pwm_right = 0;
    setMotor = 0;  
    
    //halt();
    //resetPololuTicks();
    
    /*fcStart();
    delay (500);
    if((check == true && check1 == true)){ 
      if(checkStop == true){
        myPIDMid.SetMode(MANUAL); 
        Serial.println("OK");
        check = false;
        check1 = false;
        checkStop = false;


      }
      else
      {
        encoderLeftCount = 0 ;
        encoderRightCount = 0 ;
      }
    } 
     

  }
}*/

/*void leftMotorConfigure() {
  //static long last_count_left = 0;    
  delta_left = leftPololuCount -last_count_left;  
  actual_speed_left = (abs(leftPololuCount-last_count_left)*(60*(1000/LOOPTIME)))/2249;//(16*29);          // 16 pulses X 29 gear ratio = 464 counts per output shaft rev      
  PID_left.Compute();     
  if (actual_speed_left < 0 || actual_speed_left > 300){
      actual_speed_left = 0;
  }
  
  Serial.print("actual_speed_left ");
  Serial.println(actual_speed_left);

  //if (delta_left <0)
  //{ 
  //    resetDefaultWheelTicks();
  //}
  
  last_count_left = leftPololuCount;
}

void rightMotorConfigure() {
  //static long last_count_right = 0;  
  delta_right = rightPololuCount-last_count_right;

  actual_speed_right = (abs(rightPololuCount-last_count_right)*(60*(1000/LOOPTIME)))/2249;//(16*29);          // 16 pulses X 29 gear ratio = 464 counts per output shaft rev
  PID_right.Compute();  
  if (actual_speed_right < 0 || actual_speed_right > 300){
      actual_speed_right = 0;
  }
  
  Serial.print("actual_speed_right ");
  Serial.println(actual_speed_right);
  
  //if (delta_right <0)
  //{ 
  //    resetDefaultWheelTicks();
  //}
  
  last_count_right = rightPololuCount;
}*/

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
  /*
  if (middleSensorObstacleDist <= largeFrontSensorRange) {
    if (leftSensorObstacleDist <= smallSensorRange  && rightSensorObstacleDist <= smallSensorRange) {
      Serial.write("7/");
      Serial.print(middleSensorObstacleDist);
      Serial.write(";");
    }
    else if (leftSensorObstacleDist <= smallSensorRange) {
      Serial.write("4/");
      Serial.print(middleSensorObstacleDist);
      Serial.write(";");
    }
    else if (rightSensorObstacleDist <= smallSensorRange) {
        Serial.write("5/");
        Serial.print(middleSensorObstacleDist);
        Serial.write(";");
    }
    else{
      Serial.write("1/");
      Serial.print(middleSensorObstacleDist);
      Serial.write(";");
    }
  }
  else if (leftSensorObstacleDist <= smallSensorRange  && rightSensorObstacleDist <= smallSensorRange) {
    Serial.write("6/;");
  }
  else if(leftSensorObstacleDist <= smallSensorRange) {
    Serial.write("2/;");
  }
  else if (rightSensorObstacleDist <= smallSensorRange) {
    Serial.write("3/;");
  }
  else {
    Serial.write("8/;");
  }
  */
  /*
  // Main(center) sensor senses obstacle
  if (middleSensorObstacleDist <= 25) {
    // Both left and right sensors sense obstacle as well
    if (leftSensorObstacleDist <= smallSensorRange && rightSensorObstacleDist <= smallSensorRange) {
      Serial.write("7/;");
//      caseCheck = 7;
//      halt();
    }

    // Only right sensor senses obstacle as well
    else if (rightSensorObstacleDist <= smallSensorRange) {
      Serial.write("5/;");
//      caseCheck = 5;
//      halt();
    }
    
    // Only left sensor senses obstacle as well
    else if (leftSensorObstacleDist <= smallSensorRange) {
      Serial.write("4/;");
//      caseCheck = 4;
//      halt();
    }
    */
    /* Left and right sensor sense nothing
       i.e. Only main(center) sensor sense obstacle */
       /*
    else {
//      Serial.println("1");
      Serial.write("1/");
      Serial.print(middleSensorObstacleDist);
      Serial.write(";");
//      caseCheck = 1;
//      halt();
    }
  }
  else if (middleSensorObstacleDist <= 80) {
      Serial.write("1/");
      Serial.print(middleSensorObstacleDist);
      Serial.write(";");
  }
  
  // Only left and right sensor senses obstacle
  else if (leftSensorObstacleDist <= smallSensorRange  && rightSensorObstacleDist <= smallSensorRange) {
    Serial.write("6/;");
//    caseCheck = 6;
//    halt();
  }
  
  // Only right sensor senses obstacle
  else if (rightSensorObstacleDist <= smallSensorRange) {
//          Serial.println("3");
    Serial.write("3/;");
//    caseCheck = 3;
//    halt();
  }
  
  // Only left sensor senses obstacle
  else if (leftSensorObstacleDist <= smallSensorRange) {
    Serial.write("2/;");
//    caseCheck = 2;
//    halt();
  }
  
  // No obstacles around
  else {
    Serial.write("8/;");
  }
  */
//  switch(caseCheck) {
//    
//    // Command: Get Distance
//    case 5: {
//      if (rightSensorObstacleDist > 15 || leftSensorObstacleDist > 15)
//        caseCheck = 8;
//      break;
//    }
//    
//    case 6: {
//      if (rightSensorObstacleDist > 15 || leftSensorObstacleDist > 15)
//        caseCheck = 8;
//      break;
//    }
//    
//    case 7: {
//      if (middleSensorObstacleDist > 25 || rightSensorObstacleDist > 15 || leftSensorObstacleDist > 15)
//        caseCheck = 8;
//      break;
//    }
}

//PI Controller
//void PIController()
//{
//  if(leftCounter >= 0 && rightCounter >= 0)
//  {
//    Serial.print("leftCounter is ");
//    Serial.println(leftCounter);
//    Serial.print("rightCounter is ");
//    Serial.println(rightCounter);
//    
//    err = (leftCounter - rightCounter)/((leftCounter + rightCounter)/2) * 255;
//    Serial.print("err is ");
//    Serial.println(round(err));
//    if (abs(err) < thresh){ // prevent integral 'windup' 
//      err2 = err2 + err; // accumulate the error integral
//    }
//    else {
//      err2 = 0; // zero it if out of bounds
//    }  
//    
//    Serial.print("round(err)/Kp is ");
//    Serial.println(round(err)/Kp);
//    Serial.print("round(err2)/Ki is ");
//    Serial.println(round(err2)/Ki);
//    rightOut += round(err)/Kp + round(err2)/Ki;
//    
//    if(abs(rightOut) >= 255) { // prevent overlimit
//        rightOut = 255;
//        Serial.println(rightOut);
//    }
//      
//    analogWrite(pwm_right, round(rightOut));    
//  }  
//}


//void runRobotAuto(int sensorValue0, int sensorValue1, int sensorValue2) {
//      
//  // Main(center) sensor senses obstable
//  if (sensorValue0 >= 570) {
//    // Both left and right sensors sense obstacle as well
//    if (sensorValue1 >= 250 && sensorValue2 >= 250) {
//      turnLeft(timeDelayByAngle(180));
//      moveForward(timeDelayByCentimeter(0));
//    }
//    
//    // Only right sensor senses obstacle as well
//    else if (sensorValue2 >= 400) {
//      turnLeft(timeDelayByAngle(90));
//      moveForward(timeDelayByAngle(135));
//      turnRight(timeDelayByAngle(90));
//      moveForward(timeDelayByCentimeter(0));
//    }
//    
//    // Only left sensor senses obstacle as well
//    else if (sensorValue1 >= 400) {
//      turnRight(timeDelayByAngle(90));
//      moveForward(timeDelayByAngle(135));
//      turnLeft(timeDelayByAngle(90));
//      moveForward(timeDelayByCentimeter(0));
//    }
//    
//    /* Left and right sensor sense nothing
//       i.e. Only main(center) sensor sense obstacle */
//    else {
//      turnLeft(timeDelayByAngle(90));
//      moveForward(timeDelayByCentimeter(0));
//    }
//  }
//
//  // Only right sensor senses obstacle
//  else if (sensorValue2 >= 630) {
//    turnLeft(timeDelayByAngle(18));
//    moveForward(timeDelayByCentimeter(0));
//  }
//  
//  // Only left sensor senses obstacle
//  else if (sensorValue1 >= 630) {
//    turnRight(timeDelayByAngle(18));
//    moveForward(timeDelayByCentimeter(0));
//  }
//  
//  // No obstacles around
//  else {
//    moveForward(timeDelayByCentimeter(0));
//  }
//}

// Read value from sensors
void readSensor(int sensorValue0, int sensorValue1, int sensorValue2, int sensorValue3) {
  int rightDistance = rightSensorDistanceMeasuredInCM(sensorValue2);
  int leftDistance = leftSensorDistanceMeasuredInCM(sensorValue1);
  int middleDistance = middleSensorDistanceMeasuredInCM(sensorValue0);
  int bottomDistance = bottomSensorDistanceMeasuredInCM(sensorValue3);
  /*
  Serial.print("Middle Sensor: ");
  Serial.print(sensorValue0, DEC);  // prints the value read in decimal
  Serial.print("\t");
  Serial.print("Left Sensor: ");
  Serial.print(sensorValue1, DEC);  // prints the value read in decimal
  Serial.print("\t");
  Serial.print("Right Sensor: ");
  Serial.print(sensorValue2, DEC);  // prints the value read in decimal
  Serial.print("\t");
  Serial.print("Bottom Sensor: ");
  Serial.println(sensorValue3, DEC);  // prints the value read in decimal
  */
}

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
  //resetPololuTicks();
  /*else if(serialData==9){
    int distCovered = getDistance();
      Serial.write("9/");
      Serial.print(distCovered);
      Serial.write(";");
      resetDefaultWheelTicks();
  }*/
  informReadyOnce = false;
  /*
  switch(serialData) {
    
    // Command: Move Foward
    case 1: {
      serialData = readSerial();
      moveForward(serialData);
      break;
    }
    
    // Command: Turn Left
    case 2: {
      serialData = readSerial();
      turnLeft(serialData);
      break;
    }
    
    // Command: Turn Right
    case 3: {
      serialData = readSerial();
      turnRight(serialData);
      break;
    }
    
    // Command: Halt
    case 4: {
      halt();
      break;
    }
    
    // Command: Run Motor
    case 5: {
      runMotor(maxSpeedA, maxSpeedB); 
      break;
    }
    
    // Command: Servo Turn
    case 6: {
      serialData = readSerial();
      servoTurn(serialData);
      break;
    }
    
    // Command: Get Distance
    case 9: {
      int distCovered = getDistance();
      Serial.write("9/");
      Serial.print(distCovered);
      Serial.write(";");
      resetDefaultWheelTicks();
      break;
    }
    
    // No command: continue moving forward
    default: {
//      moveForward(distCentimeter(0));
    }
  }
  */
}

//void incLeftCount() {
//  leftWheelTickCount++;
//  if (countTicksForAngleOrDist == true) {
//    leftTicksForAngleOrDist++;
//  }
//}
//
//void incRightCount() {
//  rightWheelTickCount++;
//  if (countTicksForAngleOrDist == true) {
//    rightTicksForAngleOrDist++;
//  }
//}
//
//void resetDefaultWheelTicks() {
//  leftWheelTickCount = 0;
//  rightWheelTickCount = 0;
//}

//void resetTicksForAngleOrDistance() {
//  angleTurn = 0;
//  distanceMove = 0;
//  countTicksForAngleOrDist = false;
//  leftTicksForAngleOrDist = 0;
//  rightTicksForAngleOrDist = 0;
//}

void resetPololuTicks() {
  PololuWheelEncoders::getCountsAndResetM1();
  PololuWheelEncoders::getCountsAndResetM2();
}

float getDistance() {
//  int totalAvgNoOfTicks = (leftWheelTickCount + rightWheelTickCount) / 2;
//  int distanceCoveredInCm = totalAvgNoOfTicks / noOfTicksPerCm;
//  return distanceCoveredInCm;

  long leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
  long rightPololuCount = abs(PololuWheelEncoders::getCountsM2());
  float leftcm = ((PI*6)/2249) * leftPololuCount;
  float rightcm = ((PI*6)/2249) * rightPololuCount;
  float averageDist = (leftcm + rightcm) / 2;
  return averageDist;
}

// Run robot at max speed
void runMotor(int moveSpeedA, int moveSpeedB) {
  analogWrite(pwm_right, moveSpeedA);  
  analogWrite(pwm_left, moveSpeedB);
}

// Move forward
void moveForward(float dist) {
  //resetPololuTicks();
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  
  float noOfTicksForDist = distCentimeter(dist);
  
  // ------ Distance to ticks formula ------- //
  //float noOfTicksPerCm = 2249/(PI*6); // adjust 6
  //float noOfTicksForDist = noOfTicksPerCm * dist;
  //setPointMid = noOfTicksForDist;
  //countTicksForAngleOrDist = true;
  float avgTicksForAngleOrDist = 0;
  
  long firstLeftCount = abs(PololuWheelEncoders::getCountsM1());
  long firstRightCount = abs(PololuWheelEncoders::getCountsM2());
  
  /*delta_left = firstLeftCount -last_count_left;
  delta_right = firstRightCount-last_count_right;
  
  if (delta_left < 0 || delta_right < 0) {
    last_count_left = 0;
    last_count_right = 0;
    resetPololuTicks();
  }*/
  
  while (avgTicksForAngleOrDist < noOfTicksForDist) {
    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
    leftTicksForAngleOrDist = leftTicksForAngleOrDist - firstLeftCount;
    
    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
    rightTicksForAngleOrDist = rightTicksForAngleOrDist - firstRightCount;
    
    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
    
    //avgTicksForAngleOrDist = leftTicksForAngleOrDist;
    configureMotor();
    
    /*if (currentTicks != 0) {
      avgTicksForAngleOrDist = currentTicks + ((leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2);
    }*/
    //leftPololuCount = abs(PololuWheelEncoders::getCountsM1());                       
    //rightPololuCount = abs(PololuWheelEncoders::getCountsM2()); 
    
  /*  if((millis()-lastMilli) >= LOOPTIME){
      lastMilli = millis();
      //leftMotorConfigure(); 
      //rightMotorConfigure();
      configureMotor();
      runMotor(pwm_left, pwm_right); //Set both motors to run at 100% duty cycle (fast)
    }*/
  }
 //currentTicks = 0;
//  while (avgTicksForAngleOrDist < noOfTicksForDist) {
//    Serial.println("");
//    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
//  }
  
  //resetTicksForAngleOrDistance();
  //setMotor = 0;
  

  digitalWrite(dir_left, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_right, HIGH);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
  
  //if (pwm_left > pwm_right) {
  //int ticksToCompensate = 50;
    //digitalWrite(dir_left, HIGH);  //Set motor direction, 3 high, 4 low
    //runMotor(0,100); // Right
    //delay(30);
  //}
  
  /*else if (pwm_right > pwm_left) {
    digitalWrite(dir_right, HIGH);  //Set motor direction, 3 high, 4 low
    runMotor(100,0); // Right
    delay(30);
  }*/
  //pwm_left = 0;
  //pwm_right = 0;
  //digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  //delay(20);
  //halt();
//  int ticksToCompensate = 50;
//  digitalWrite(dir_right, LOW);  //Set motor direction, 3 high, 4 low
//  runMotor(100,0); // Right
//  delay(70);
  //halt();
  //resetPololuTicks();
  
  //if() {
    /*int leftCount = abs(PololuWheelEncoders::getCountsM1());
    int rightCount = abs(PololuWheelEncoders::getCountsM2());
    Serial.print("left count =");
    Serial.print(leftCount);
    Serial.print("\t");
    Serial.print("right count =");
    Serial.println(rightCount);*/
  //}
}

// Turn right
void turnRight(int angle) {
  resetPololuTicks();
  //runMotor(maxSpeedA, maxSpeedB); //Set both motors to run at 100% duty cycle (fast)
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high (right)
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low (left)
  
  float noOfTicksForAngle = turnAngleR(angle);
  
  // ------ Angle to ticks formula ------- //
  //float noOfTicksPerCm = 2249/(PI*6);
  //float noOfCmPerDegreeForRightTurn = (PI*17.494)/360; // adjust 17.2
  //float totalCmForAngle = noOfCmPerDegreeForRightTurn * angle;
  //float noOfTicksForAngle = noOfTicksPerCm * totalCmForAngle;
  //360*(60/172)/2249/2;
  
  //countTicksForAngleOrDist = true;
  
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
  
//  while (avgTicksForAngleOrDist < noOfTicksForAngle) {
//    Serial.println("");
//    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
//  }
  
  //resetTicksForAngleOrDistance();
  
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
  //reverse(0.2);
  resetPololuTicks();
}

// Turn left
void turnLeft(int angle) {
  resetPololuTicks();
  //runMotor(maxSpeedA, maxSpeedB); //Set both motors to run at 100% duty cycle (fast)
  digitalWrite(dir_right, LOW);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH); //Set motor direction, 3 high, 4 low
  //turnAngle(angle+1);
  
  float noOfTicksForAngle = turnAngle(angle);
  //float noOfTicksPerCm = 2249/(PI*6); // adjust 6
  //float noOfCmPerDegreeForLeftTurn = (PI*17.494)/360; // adjust 17.2
  //float totalCmForAngle = noOfCmPerDegreeForLeftTurn * angle;
  //float noOfTicksForAngle = noOfTicksPerCm * totalCmForAngle;
  
  //countTicksForAngleOrDist = true;
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
  
//  while (avgTicksForAngleOrDist < noOfTicksForAngle) {
//    Serial.println("");
//    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
//  }
  
  //resetTicksForAngleOrDistance();
  
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  delay(40);
  halt();
  //reverse(0.2);
  resetPololuTicks();
}

// Reverse backwards
void reverse(float dist) {
  //resetPololuTicks();
  runMotor(100, 100); //Set both motors to run at 100% duty cycle (fast)
  digitalWrite(dir_right, HIGH); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, HIGH);  //Set motor direction, 3 high, 4 low
  delay(70);
  
//  float noOfTicksForDist = distCentimeter(dist);
//  //countTicksForAngleOrDist = true;
//  float avgTicksForAngleOrDist = 0;
//  
//  int firstLeftCount = abs(PololuWheelEncoders::getCountsM1());
//  int firstRightCount = abs(PololuWheelEncoders::getCountsM2());
//  
//  while (avgTicksForAngleOrDist < noOfTicksForDist) {
//    leftTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM1());
//    leftTicksForAngleOrDist = leftTicksForAngleOrDist - firstLeftCount;
//    
//    rightTicksForAngleOrDist = abs(PololuWheelEncoders::getCountsM2());
//    rightTicksForAngleOrDist = rightTicksForAngleOrDist - firstRightCount;
//    
//    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
//  }
//  
//  while (avgTicksForAngleOrDist < noOfTicksForDist) {
//    Serial.println("");
//    avgTicksForAngleOrDist = (leftTicksForAngleOrDist + rightTicksForAngleOrDist) / 2;
//  }
  
  //resetTicksForAngleOrDistance();
  
  digitalWrite(dir_right, LOW); //Set motor direction, 1 low, 2 high
  digitalWrite(dir_left, LOW);  //Set motor direction, 3 high, 4 low
  delay(30);
  halt();
  //resetPololuTicks();
}

// Stop movement
void halt() 
{ 
  analogWrite(pwm_right, 0);
  analogWrite(pwm_left, 0);
  delay(100);
  //resetPololuTicks();
}

float middleSensorDistanceMeasuredInCM(int middleSensor) {
  //float volts = middleSensor * 0.0050528125; // worked out from graph 65 = theoretical distance / (1/Volts)S - luckylarry.co.uk
  //float distance = 65 * pow(volts, -1.10);
  //return distance;
  
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

