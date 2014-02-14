#ifndef CONSTANTS_H
#define CONSTANTS_H
/*
In C++ const objects have internal linkage unless explicitly declared extern 
so the is no problem with putting a definition in a header.
*/
// constants START
const int MAX_SPEED_ANALOG = 255; // Built-in PWM

const int DESIGNED_MAX_SPEED = 400;

const int WHEEL_DIAMETER = 6; // the motor shaft is mount the larger Pololu wheels (60mm)
const int COUNTS_PER_REVOLUTION = 2249/2; // 48 CPR quadrature encoder on the motor shaft,  which provides 2249 counts per revolution (gear ratio: 47:1)
const double DISTANCE_PER_TICK_CM = (PI*WHEEL_DIAMETER)/COUNTS_PER_REVOLUTION;


double MAX_SPEED = 300; // 180 RPM (confirmed)
double TARGET_SPEED = 200; 
double MIN_SPEED = 100;

double PID_UPPER_LIMIT = COUNTS_PER_REVOLUTION*6*(double(MAX_SPEED)/DESIGNED_MAX_SPEED); // 3400
double PID_LOWER_LIMIT = PID_UPPER_LIMIT*(double(MIN_SPEED)/DESIGNED_MAX_SPEED); // added by Danyang
double PID_SETPOINT = PID_UPPER_LIMIT*(double(TARGET_SPEED)/DESIGNED_MAX_SPEED); // 2000

const int SAMPLE_TIME = 10; // 10ms original
// [0, 3400]
const double WHEELS_INTERVAL = 17.23; // cm // affects turning // 17.2 undershotting; 17.3 overshoting 
// constants END

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

*/
// Encoder: two channels for both speed and direction 
// remaining least-significant-bit pins

const unsigned char INA1 = 2; // remapped to 5
const unsigned char INB1 = 5;
const unsigned char EN1DIAG1 = 6;
const unsigned char CS1 = A0;  // not used 
const unsigned char INA2 = 7;
const unsigned char INB2 = 8;
const unsigned char EN2DIAG2 = 12;
const unsigned char CS2 = A1; // not used 

const unsigned char M1_ENCODER_A = 3; // external interrupt pin 2, 3
const unsigned char M1_ENCODER_B = 5; 
const unsigned char M2_ENCODER_A = 11; 
const unsigned char M2_ENCODER_B = 13; 


// Digital Pins END

// Analog Pins START

// Analog Pins END
#endif