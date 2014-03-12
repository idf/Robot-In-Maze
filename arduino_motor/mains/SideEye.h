#ifndef SIDEEYE_H
#define SIDEEYE_H
#include <SharpIR.h>
#include <Ultrasound.h>
#define SHORT 1080
//#define LONG 20150
#define RANGE_TEST_DELAY 5

class SideEye{
public:
  SideEye(unsigned char left_pin, unsigned char right_pin, unsigned char ultra_pin_1, unsigned ultra_pin_2);
  void init();
  int output_reading_right();
  int output_reading_left();
  int output_reading_ultra();
  void test_readings();
private: 
  SharpIR* sharp_left;
  SharpIR* sharp_right;
  Ultrasound* ultrasound;

  bool is_within_range_left(SharpIR* sensor, int model);
  bool is_within_range_right(SharpIR* sensor, int model);
  int _output_reading_left(SharpIR* sensor, int model);
  int _output_reading_right(SharpIR* sensor, int model);
  int get_ultra_reading();
  bool is_within_range(Ultrasound* sensor);
};
#endif

