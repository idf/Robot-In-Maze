#ifndef CONSTANTS_H
#define CONSTANTS_H
/*
In C++ const objects have internal linkage unless explicitly declared extern 
so the is no problem with putting a definition in a header.
*/
// constants START
const int MAX_SPEED_ANALOG = 255; // Built-in PWM
const int MAX_SPEED = 400;
const int MID_SPEED = 235;

const int WHEEL_DIAMETER = 6; // the motor shaft is mount the larger Pololu wheels (60mm)
const int COUNTS_PER_REVOLUTION = 2249; // 48 CPR quadrature encoder on the motor shaft,  which provides 2249 counts per revolution (gear ratio: 47:1)
const float DISTANCE_PER_TICK_CM = (PI*WHEEL_DIAMETER)/COUNTS_PER_REVOLUTION;

const int PID_UPPER_LIMIT = 3400;
const int PID_SETPOINT = 2000;
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
Digital 9   M1PWM     Motor 1 *speed input
Digital 10  M2PWM     Motor 2 *speed input
Digital 12  M2EN/DIAG Motor 2 enable input/fault output

Analog  A0  M1CS      Motor 1 current sense output
Analog  A1  M2CS      Motor 2 current sense output

*/
// Encoder: two channels for both speed and direction 
// remaining least-significant-bit pins

// 3 5 11 13
const int M1_ENCODER_A = 3;
const int M1_ENCODER_B = 5;
const int M2_ENCODER_A = 11; 
const int M2_ENCODER_B = 13; 


// Digital Pins END

// Analog Pins START

// Analog Pins END
#endif

