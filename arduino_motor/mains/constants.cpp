// constants.cpp
//

#include "constants.h"
#define LZZ_INLINE inline
int const MAX_SPEED_ANALOG = 255;
int const DESIGNED_MAX_SPEED = 400;
int const WHEEL_DIAMETER = 6;
int const COUNTS_PER_REVOLUTION = 2249/2;
double const DISTANCE_PER_TICK_CM = (PI*WHEEL_DIAMETER)/COUNTS_PER_REVOLUTION;
int const SAMPLE_TIME = 10;
double const WHEELS_INTERVAL = 17.10;
unsigned char const INA1 = 2;
unsigned char const INB1 = 5;
unsigned char const EN1DIAG1 = 6;
unsigned char const CS1 = A0;
unsigned char const INA2 = 7;
unsigned char const INB2 = 8;
unsigned char const EN2DIAG2 = 12;
unsigned char const CS2 = A1;
unsigned char const M1_ENCODER_A = 3;
unsigned char const M1_ENCODER_B = 5;
unsigned char const M2_ENCODER_A = 11;
unsigned char const M2_ENCODER_B = 13;
double MAX_SPEED = 300;
double TARGET_SPEED = 200;
double MIN_SPEED = 100;
double PID_UPPER_LIMIT = COUNTS_PER_REVOLUTION*6*(double(MAX_SPEED)/DESIGNED_MAX_SPEED);
double PID_LOWER_LIMIT = PID_UPPER_LIMIT*(double(MIN_SPEED)/DESIGNED_MAX_SPEED);
double PID_SETPOINT = PID_UPPER_LIMIT*(double(TARGET_SPEED)/DESIGNED_MAX_SPEED);
#undef LZZ_INLINE
