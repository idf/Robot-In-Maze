#ifndef FRONTEYE_H
#define FRONTEYE_H
#include <SharpIR.h>
#include <Ultrasound.h>
#define SHORT 1080
#define LONG 20150

class FrontEye{
public:
  FrontEye(unsigned char ultra_pin_1, unsigned char ultra_pin_2, unsigned char ir_pin);
  void init();
  int get_reading();
  
  void test_readings();
private: 
  int get_ulra_reading();
  int get_ir_reading();
  bool is_within_range(SharpIR* sensor);
  bool is_within_range(Ultrasound* sensor);
  Ultrasound* ultrasound;
  SharpIR* sharpLong;//front
};
#endif

