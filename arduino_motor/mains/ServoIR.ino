#include "ServoIR.h"
#include <Arduino.h>
#include <Servo.h> // get from dropbox
#include <SharpIR.h> // get from dropbox
#define ADJUST_ANGLE -8 // left rotation is positive // -8 is due forward // -19 perfect 20 cm

ServoIR::ServoIR(unsigned char servoPin, unsigned char irPin) {
  this->servoPin = servoPin;
  this->irPin = irPin;

  this->sharpShort = new SharpIR(this->irPin, 250, 95, 1080); //SharpIR::SharpIR(int irPin, int avg, int tolerance, int sensorModel) // if (foo>=(_tol*_previousDistance))
  this->servo = new Servo();
}

void ServoIR::init() {
  pinMode(this->irPin, INPUT);
  this->servo->attach(this->servoPin); 
}

// due right: 0; due forward: 90
// non-negative angle

void ServoIR::servoScan(int starting_angle, int turning_angle) {
  this->reset_array();
  this->servo_write_adjusted(starting_angle);
  delay(700);
  int i=0;
  for(int angle = starting_angle; angle <= turning_angle; angle++) { // smoothing
    this->servo_write_adjusted(angle);
    if(angle%5==0) {
      this->distArray[i++] = this->sharpShort->distance();
    }
  }
  delay(700);
  this->servo_write_adjusted(90);
  delay(700);
}

void ServoIR::servoScan() {
  this->servoScan(0, 180);
}

void ServoIR::print_dist_array() {
  Serial.print("Distance array: ");  // returns it to the serial monitor
  for(int i = 0; i < ARRAY_SIZE; i++) {
    Serial.print(this->distArray[i]); Serial.print(F("\t"));
   }
  Serial.println();
}

void ServoIR::reset_array() {
  for(int i = 0; i<ARRAY_SIZE; i++)
    this->distArray[i] = -1;
}

void ServoIR::servo_write_adjusted(int angle) {
  int adjusted_angle = angle + ADJUST_ANGLE;
  this->servo->write(adjusted_angle);
}

void ServoIR::servo_point_read(int angle) {
  this->reset_array();
  this->servo_write_adjusted(angle);
  delay(400);
  this->distArray[0] = this->sharpShort->distance();
}
