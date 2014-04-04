#ifndef EYES_H
#define EYES_H
#include <SharpIR.h>
#include <Ultrasound.h>
#define SHORT 1080
//#define LONG 20150
#define RANGE_TEST_DELAY 7
#define ULTRA_SIZE 2
#define SHARP_SIZE 4
class Eyes{
public:
  Eyes(
    unsigned char ultra_pin_0_1, 
    unsigned char ultra_pin_0_2, 
    unsigned char ultra_pin_1_1, 
    unsigned char ultra_pin_1_2, 
    unsigned char ir_pin_0, 
    unsigned char ir_pin_1, 
    unsigned char ir_pin_2, 
    unsigned char ir_pin_3
    );

  void init();
  // for reading 10 * 
  int output_reading_ultra(int index);
  int output_reading_ir(int index);

  void test_readings();
  
  // for calibrator to use
  // actual reading with offset
  int get_ir_reading_left();
  int get_ir_reading_right();  
  int output_reading_ir_left();
  int output_reading_ir_right();
  int get_ultra_reading_left();
  int get_ultra_reading_right();

  bool is_safe_forward(int dis);

private: 
  int get_ultra_reading(int index); // to clear the check list
  int get_ir_reading(int index);

  bool is_within_range(SharpIR* sensor, int index);
  bool is_within_range(Ultrasound* sensor, int index);

  Ultrasound* ultrasound[ULTRA_SIZE];
  SharpIR* sharp[SHARP_SIZE]; //Foo *array[10]; // array of 10 Foo pointers

  int ultra_offsets[ULTRA_SIZE];
  int sharp_offsets[SHARP_SIZE];
};
#endif

