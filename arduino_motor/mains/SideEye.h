#ifndef SIDEEYE_H
#define SIDEEYE_H
#include <SharpIR.h>
#define SHORT 1080
#define LONG 20150

class SideEye{
public:
  SideEye(unsigned char left_pin, unsigned char right_pin);
  void init();
  int get_reading_right();
  int get_reading_left();
  void test_readings();
private: 
  SharpIR* sharp_left;
  SharpIR* sharp_right;
  bool is_within_range(SharpIR* sensor);
  int get_reading(SharpIR* sensor);
};
#endif

