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
int previousLeftTick = 0;
int previousRightTick = 0;

// PID
// variables upper case by documentation 
double SetpointLeft, InputLeft, OutputLeft;
double SetpointRight, InputRight, OutputRight;
double SetpointMid, InputMid, OutputMid;

double kp=2, ki=1, kd=1;
PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, kp, ki, kd, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight, kp, ki, kd, DIRECT); // 0.5 possible
PID midPID(&InputMid, &OutputMid, &SetpointMid, 1, 1, 1, DIRECT);

// TODO
PID_ATune aTuneLeft(&InputLeft, &OutputLeft);
PID_ATune aTuneRight(&InputRight, &OutputRight);

// Deduced Reckoning 
double theta, deltaX, deltaY;

DualVNH5019MotorShield motorShield;
#endif
