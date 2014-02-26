#ifndef FRONTEYE_H
#define FRONTEYE_H
#include <SharpIR.h>
#include <Ultrasound.h>
#define SHORT 1080
#define LONG 20150

class FrontEye{
public:
  FrontEye();
  void init();
  int get_reading();
  
  void test_readings();
private: 
  int get_ulra_reading();
  int get_ir_reading();
  Ultrasound* ultrasound;
  SharpIR* sharpLong;//front
};
#endif

