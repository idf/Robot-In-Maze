// # Connection:
// #       Pin 1 VCC (URM V3.2) -> VCC (Arduino)
// #       Pin 2 GND (URM V3.2) -> GND (Arduino)
// #       Pin 4 PWM (URM V3.2) -> Pin 11 (Arduino)
// #       Pin 6 COMP/TRIG (URM V3.2) -> Pin 13 (Arduino)
// #       TTL mode
#include <SharpIR.h>
#include <Ultrasound.h>
#include <Servo.h>

#define SHORT 1080
#define LONG 20150



//pin mapping
//11 PWM Output 0－25000US，Every 50US represent 1cm
//13 PWM trigger pin
Ultrasound ultrasound(11,13);

//Sensors
SharpIR sharpLong(A0, 25, 93, LONG);//front
SharpIR sharp1(A1, 25, 93, SHORT); //left front
SharpIR sharp2(A2, 25, 93, SHORT);//right front
SharpIR sharp3(A3, 25, 93, SHORT);//left side

// ir: the pin where your sensor is attached
// 25: the number of readings the library will make before calculating a mean distance
// 93: the difference between two consecutive measurements to be taken as valid
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)



 
void setup(){                                 // Serial initialization
  Serial.begin(9600);                         // Sets the baud rate to 9600
  ultrasound.init();
}
 
void loop()
{
 
  
  Serial.print("Ultrasound Distance: ");
  Serial.print(ultrasound.dist());
   Serial.println("cm");
  
  int dis=sharpLong.distance();  // this returns the distance to the object you're measuring
  Serial.print("Mean distance long: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  int dis1=sharp1.distance();  // this returns the distance to the object you're measuring
  Serial.print("Mean distance left front: ");  // returns it to the serial monitor
  Serial.println(dis1);
  
  int dis2=sharp2.distance();  // this returns the distance to the object you're measuring
  Serial.print("Mean distance right front : ");  // returns it to the serial monitor
  Serial.println(dis2);
  
  //int dis3=sharp3.distance();  // this returns the distance to the object you're measuring
  //Serial.print("Mean distance left side: ");  // returns it to the serial monitor
  //Serial.println(dis3);
  
  
  delay(1000);
 
}                      
 

 

  
