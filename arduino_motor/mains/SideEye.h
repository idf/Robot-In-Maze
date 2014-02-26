#ifndef SIDEEYE_H
#define SIDEEYE_H
#include <SharpIR.h>
#define SHORT 1080
#define LONG 20150

class SideEye{
public:
  SideEye();
  void init();
  int get_reading_right();
  
  void test_readings();
private: 
  SharpIR* sharp_left;
  SharpIR* sharp_right;
  bool is_within_range(SharpIR* sensor);
};
#endif

