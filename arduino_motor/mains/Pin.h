#ifndef PIN_H
#define PIN_H
class Pin
{
public:
  /*
  In C++ const objects have internal linkage unless explicitly declared extern 
  so the is no problem with putting a definition in a header.
  */
  // Digital Pins START
  /* 
  Built-in digital pin:
  Default Pin Mapping 
  Arduino Pin VNH5019   Basic Function
  Digital 2   M1INA     Motor 1 *direction input A
  Digital 4   M1INB     Motor 1 *direction input B
  Digital 6   M1EN/DIAG Motor 1 enable input/fault output
  Digital 7   M2INA     Motor 2 *direction input A
  Digital 8   M2INB     Motor 2 *direction input B
  Digital 9   M1PWM     Motor 1 *speed input // cannot configure
  Digital 10  M2PWM     Motor 2 *speed input // cannot configure 
  Digital 12  M2EN/DIAG Motor 2 enable input/fault output

  Analog  A0  M1CS      Motor 1 current sense output
  Analog  A1  M2CS      Motor 2 current sense output
  Analog  A4  Arduino A4 (SDA) -> HMC5883L SDA
  Analog  A5  Arduino A5 (SCL) -> HMC5883L SCL

  */
  //static double const _PI = 3.14159265359;
  static const unsigned char INA1 = 2; // remapped to 5
  static const unsigned char INB1 = 4;
  static const unsigned char EN1DIAG1 = 6;
  static const unsigned char CS1 = A0;  // not used 
  static const unsigned char INA2 = 7;
  static const unsigned char INB2 = 8;
  static const unsigned char EN2DIAG2 = 12;
  static const unsigned char CS2 = A1; // not used 

  static const unsigned char M1_ENCODER_A = 3; // external interrupt pin 2, 3
  //static const unsigned char M1_ENCODER_B = 5; 
  static const unsigned char M2_ENCODER_A = 5; 
  //static const unsigned char M2_ENCODER_B = 13; 


  //static const unsigned char SERVO = 9; // TODO change
  static const unsigned char ULTRA_1_PWM = 11;
  static const unsigned char ULTRA_1_TRIG = 13;
  static const unsigned char ULTRA_2_PWM = 6;
  static const unsigned char ULTRA_2_TRIG = 12;

  static const unsigned char FRONT_IR_LEFT = A0;
  static const unsigned char FRONT_IR_RIGHT = A1;

  static const unsigned char LEFT_IR = A2;
  static const unsigned char RIGHT_IR_LONG = A3; 
};
#endif
