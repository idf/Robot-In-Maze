#ifndef GLOBALS_H
#define GLOBALS_H
/* 
M1 on LEFT
M2 on RIGHT 
*/
// Ticking
float leftTicksForAngleOrDist = 0;
float rightTicksForAngleOrDist = 0;

long timing = 0;
long previousLeftTick = 0;
long previousRightTick = 0;

// PID
// variables upper case by documentation 
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;
double SetpointMid, InputMid, OutputMid;

PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 1, 1, 0, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight, 1, 1, 0, DIRECT);
PID midPID(&InputMid, &OutputMid, &SetpointMid, 1, 1, 1, DIRECT);

// Deduced Reckoning 
float deltaHeading, deltaX, deltaY;

DualVNH5019MotorShield motorShield;
#endif