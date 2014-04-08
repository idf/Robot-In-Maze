#include "Eyes.h"
#include <SharpIR.h>
#include <Ultrasound.h>
#include "Config.h"
//public 
Eyes::Eyes(
    unsigned char ultra_pin_0_1, 
    unsigned char ultra_pin_0_2, 
    unsigned char ultra_pin_1_1, 
    unsigned char ultra_pin_1_2, 

    unsigned char ir_pin_0, 
    unsigned char ir_pin_1, 
    unsigned char ir_pin_2, 
    unsigned char ir_pin_3

    ) { 
  this->ultrasound[0] = new Ultrasound(ultra_pin_0_1, ultra_pin_0_2);
  this->ultrasound[1] = new Ultrasound(ultra_pin_1_1, ultra_pin_1_2);

  this->sharp[0] = new SharpIR(ir_pin_0, 50, 95, SHORT);
  this->sharp[1] = new SharpIR(ir_pin_1, 50, 95, SHORT);
  this->sharp[2] = new SharpIR(ir_pin_2, 50, 95, SHORT);
  this->sharp[3] = new SharpIR(ir_pin_3, 50, 95, SHORT);

  this->ultra_offsets[0] = 4;
  this->ultra_offsets[1] = 4;

  this->sharp_offsets[0] = 5; // 6;
  this->sharp_offsets[1] = 5; // 6; // 7 April
  this->sharp_offsets[2] = 3; // 5;
  this->sharp_offsets[3] = 5; // 6;

  for(int i=0; i<SHARP_SIZE; i++) {
    this->range[i][0]= 27;
    this->range[i][1]=6;
  }
  
  this->range[3][0] = 37;
  this->range[3][1] = 6;
}
void Eyes::init() {
  for(int i=0; i<ULTRA_SIZE; i++)
    this->ultrasound[i]->init();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int Eyes::output_reading_ultra(int index) {
  int dis = this->get_ultra_reading(index);

  dis -= Config::VIRTUAL_OFFSET;

  if(this->is_within_range(this->ultrasound[index], index)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;
}

int Eyes::output_reading_ir(int index) {
  int dis = this->get_ir_reading(index);

  dis -= Config::VIRTUAL_OFFSET;
  
  if(this->is_within_range(this->sharp[index], index)) 
    dis = (dis+5)/10*10;
  else 
    dis = -1;
  return dis;  
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int Eyes::get_ir_reading_left() {
  return this->get_ir_reading(0);
}
int Eyes::get_ir_reading_right() {
  return this->get_ir_reading(1);
}
int Eyes::output_reading_ir_left() {
  return this->output_reading_ir(0);
}
int Eyes::output_reading_ir_right() {
  return this->output_reading_ir(1);
}
int Eyes::get_ultra_reading_left() {
  return this->get_ultra_reading(0);
}
int Eyes::get_ultra_reading_right() {
  return this->get_ultra_reading(1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void Eyes::test_readings() {
  for(int i=0; i<SHARP_SIZE; i++) {
    int dis = this->sharp[i]->distance();  
    Serial.print(i);
    Serial.print(F(" SharpIR distance: "));  
    Serial.println(dis);   
  }
  for(int i=0; i<ULTRA_SIZE; i++) {
    int dis= this->ultrasound[i]->dist();
    Serial.print(i);
    Serial.print(F(" Ultrasound Distance :"));
    Serial.println(dis);
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Eyes::is_safe_forward(int dis) {
  int reading = this->output_reading_ir(2);
  if(reading>=dis || reading==-1)
    return true;
  return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// private
int Eyes::get_ultra_reading(int index) {
  return this->ultrasound[index]->dist()-this->ultra_offsets[index];
}

int Eyes::get_ir_reading(int index) {
  return this->sharp[index]->distance()-this->sharp_offsets[index];
}


bool Eyes::is_within_range(SharpIR* sensor, int index) {
  for(int i=0; i<5; i++) {
    int distance = sensor->distance();
    if(distance>this->range[index][0] || distance<this->range[index][1]) // TODO
      return false;
    delay(RANGE_TEST_DELAY);
  }
  return true;
}

bool Eyes::is_within_range(Ultrasound* sensor, int index) {
  for(int i=0; i<7; i++) {
    int distance = sensor->dist();
    if(distance>107 || distance<3) // TODO
      return false;
    delay(RANGE_TEST_DELAY);
  }
  return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
