#include <SharpIR.h>

#define ir A0
#define short 1080
#define long 20150
boolean done=false;


SharpIR sharpLong(ir, 25, 93, long);//front
SharpIR sharp1(A1, 25, 93, short); //right front
SharpIR sharp2(A2, 25, 93, short);//left front
SharpIR sharp3(A3, 25, 93, short);//left side

// ir: the pin where your sensor is attached
// 25: the number of readings the library will make before calculating a mean distance
// 93: the difference between two consecutive measurements to be taken as valid
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)



void setup(){
  
  Serial.begin(9600);
  //pinMode (ir, INPUT);
  
}





void loop(){

  delay(2000);    // it gives you time to open the serial monitor after you upload the sketch
  
 if (done==false){  // it only runs the loop once
  
 

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
  
 


done=false;
  
}

}
  

