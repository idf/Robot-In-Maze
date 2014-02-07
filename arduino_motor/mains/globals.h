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

const double kp=0.5, ki=1, kd=0;
const double kp_mid=1, ki_mid=0.05, kd_mid=0.25;

PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, kp, ki, kd, DIRECT);
PID rightPID(&InputRight, &OutputRight, &SetpointRight, kp, ki, kd, DIRECT); 
PID midPID(&InputMid, &OutputMid, &SetpointMid, kp_mid, ki_mid, kd_mid, DIRECT);

// TODO
PID_ATune aTuneLeft(&InputLeft, &OutputLeft);
PID_ATune aTuneRight(&InputRight, &OutputRight);

// Deduced Reckoning 
double theta, deltaX, deltaY;

DualVNH5019MotorShield motorShield;
#endif
