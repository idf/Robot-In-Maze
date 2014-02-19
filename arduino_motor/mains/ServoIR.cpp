#include "ServoIR.h"
#include <Arduino.h>
#include <Servo.h> // get from dropbox
#include <SharpIR.h> // get from dropbox
ServoIR::ServoIR(unsigned char servoPin, unsigned char irPin) {
  this->servoPin = servoPin;
  this->irPin = irPin;

  this->sharpShort = new SharpIR(this->irPin, 25, 93, 1080);
  this->servo = new Servo();
}

void ServoIR::init() {
  pinMode(this->irPin, INPUT);
  this->servo->attach(this->servoPin); 
}

void ServoIR::servoScan(int turning_angle) {
  int i=0;
  for(int angle = 0; angle < turning_angle; angle+=10) {                                  
    this->servo->write(angle);
    delay(15);
    this->distArray[i++] = this->sharpShort->distance(); 
  }
}

void ServoIR::servoScan() {
  this->servoScan(180);
}

void ServoIR::print_dist_array() {
  Serial.print("Distance array: ");  // returns it to the serial monitor
  for(int i = 0; i < 18; i++) {
    Serial.print(this->distArray[i]); Serial.print(F("\t"));
   }
   
}