#ifndef GLOBALS_H
#define GLOBALS_H
/* 
M1 on LEFT
M2 on RIGHT 
*/
// Ticking
double leftTicksForAngleOrDist = 0;
double rightTicksForAngleOrDist = 0;

long timing = 0;
long previousLeftTick = 0;
long previousRightTick = 0;

// PID
// variables upper case by documentation 
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;
double SetpointMid, InputMid, OutputMid;

PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, 0.5, 1, 0, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight, 0.5, 1, 0, DIRECT);
PID midPID(&InputMid, &OutputMid, &SetpointMid, 1, 1, 1, DIRECT);

// Deduced Reckoning 
double theta, deltaX, deltaY;

DualVNH5019MotorShield motorShield;
#endif
