#ifndef SIDEEYE_H
#define SIDEEYE_H
#include <SharpIR.h>
#define SHORT 1080
#define LONG 20150

class SideEye{
public:
  SideEye(unsigned char left_pin, unsigned char right_pin);
  void init();
  int output_reading_right();
  int output_reading_left();
  void test_readings();
private: 
  SharpIR* sharp_left;
  SharpIR* sharp_right;
  bool is_within_range(SharpIR* sensor, int model);
  int output_reading(SharpIR* sensor, int model);
};
#endif

