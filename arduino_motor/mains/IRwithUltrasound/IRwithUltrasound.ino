// # Editor    :Jiang from DFRobot
// # Data      :18.09.2012
 
// # Product name:ultrasonic scanner 
// # Product SKU:SEN0001
// # Version :  0.2
 
// # Description:
// # The Sketch for scanning 180 degree area 4-500cm detecting range
 
// # Connection:
// #       Pin 1 VCC (URM V3.2) -> VCC (Arduino)
// #       Pin 2 GND (URM V3.2) -> GND (Arduino)
// #       Pin 4 PWM (URM V3.2) -> Pin 11 (Arduino)
// #       Pin 6 COMP/TRIG (URM V3.2) -> Pin 13 (Arduino)
// #       TTL mode
#include <SharpIR.h>

#define ir A0
#define s 1080
#define l 20150



//pin mapping
int URPWM = 11; // PWM Output 0－25000US，Every 50US represent 1cm
int URTRIG = 13; // PWM trigger pin
 
unsigned int Distance=0;
uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command

//Sensors
SharpIR sharpLong(A0, 25, 93, l);//front
SharpIR sharp1(A1, 25, 93, s); //right front
SharpIR sharp2(A2, 25, 93, s);//left front
SharpIR sharp3(A3, 25, 93, s);//left side

// ir: the pin where your sensor is attached
// 25: the number of readings the library will make before calculating a mean distance
// 93: the difference between two consecutive measurements to be taken as valid
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)



 
void setup(){                                 // Serial initialization
  Serial.begin(9600);                         // Sets the baud rate to 9600
  PWM_Mode_Setup();
}
 
void loop()
{
 PWM_Mode();
 delay(20);
}                      //PWM mode setup function
 
void PWM_Mode_Setup(){ 
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH
  
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
  
  for(int i=0;i<4;i++){
      Serial.write(EnPwmCmd[i]);
   } 
}
 
void PWM_Mode(){                              // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses
    
    unsigned long DistanceMeasured=pulseIn(URPWM,LOW);
     
    if(DistanceMeasured==50000){              // the reading is invalid.
      Serial.print("Invalid");    
   }
    else{
      Distance=DistanceMeasured/50;           // every 50us low level stands for 1cm
   }
  Serial.print("Ultrasound Distance=");
  Serial.print(Distance);
  Serial.println("cm");
  
  
  int dis=sharpLong.distance();  // this returns the distance to the object you're measuring
  Serial.print("Mean distance long: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  //int dis1=sharp1.distance();  // this returns the distance to the object you're measuring
  //Serial.print("Mean distance right front: ");  // returns it to the serial monitor
  //Serial.println(dis1);
  
  //int dis2=sharp2.distance();  // this returns the distance to the object you're measuring
  //Serial.print("Mean distance left front : ");  // returns it to the serial monitor
  //Serial.println(dis2);
  
  //int dis3=sharp3.distance();  // this returns the distance to the object you're measuring
  //Serial.print("Mean distance left side: ");  // returns it to the serial monitor
  //Serial.println(dis3);
  
  
  
  
  
  delay(500);
  
}
