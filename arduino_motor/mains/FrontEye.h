#ifndef FRONTEYE_H
#define FRONTEYE_H
#include <SharpIR.h>
#include <Ultrasound.h>
#define SHORT 1080
#define LONG 20150

class FrontEye{
public:
  FrontEye(unsigned char ultra_pin_1, unsigned char ultra_pin_2, unsigned char ir_pin_left, unsigned char ir_pin_right);
  void init();

  int output_reading_ultra();
  int output_reading_ir_left();
  int output_reading_ir_right();

  void test_readings();
   int get_ulra_reading();
private: 
 
  int get_ir_reading_left();
  int get_ir_reading_right();
  bool is_within_range(SharpIR* sensor);
  bool is_within_range(Ultrasound* sensor);

  Ultrasound* ultrasound;
  SharpIR* sharp_left;
  SharpIR* sharp_right;
};
#endif

