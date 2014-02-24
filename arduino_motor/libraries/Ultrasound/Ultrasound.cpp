#include "Ultrasound.h"
#include "Arduino.h"
Ultrasound::Ultrasound(int URPWM, int URTRIG){
 this->URPWM = URPWM;
 this->URTRIG = URTRIG; 
 
  
}

void Ultrasound::init(){
  
  uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};  
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
  for(int i=0;i<4;i++){
      Serial.write(EnPwmCmd[i]);
   } 
}

int Ultrasound::dist(){
    uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};  
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses 
    unsigned long DistanceMeasured=pulseIn(URPWM,LOW);
    int Distance=0;
    if(DistanceMeasured==50000){              // the reading is invalid.
      Serial.print("Invalid");    
    }
    else{
      Distance=DistanceMeasured/50;           // every 50us low level stands for 1cm
    }
    return Distance;
  
}


