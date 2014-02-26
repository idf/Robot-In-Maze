#ifndef GLOBALS_H
#define GLOBALS_H
#include "Pin.h"
#include "Serial.h"
#include "ErrorCumulator.h"
#include "FrontEye.h"
#include "SideEye.h"
//#include "ServoIR.h"
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



const double kp=0.25, ki=0.25, kd=0;
const double kp2=0.25, ki2=0.25, kd2=0;

const double kp_mid=1, ki_mid=0.05, kd_mid=0.25;

PID leftPID(&InputLeft, &OutputLeft, &SetpointLeft, kp, ki, kd, DIRECT);

PID rightPID(&InputRight, &OutputRight, &SetpointRight, kp2, ki2, kd2, DIRECT); 

PID midPID(&InputMid, &OutputMid, &SetpointMid, kp_mid, ki_mid, kd_mid, DIRECT);

// TODO
//PID_ATune aTuneLeft(&InputLeft, &OutputLeft);
//PID_ATune aTuneRight(&InputRight, &OutputRight);

// Deduced Reckoning 
//double theta, deltaX, deltaY;


// MotorShiled
DualVNH5019MotorShield motorShield(Pin::INA1, Pin::INB1, Pin::EN1DIAG1, Pin::CS1, Pin::INA2, Pin::INB2, Pin::EN2DIAG2, Pin::CS2);

SerialCommnder* serialCommnder = new SerialCommnder(); 
ErrorCumulator* errorCumulator = new ErrorCumulator();
FrontEye* frontEye = new FrontEye();
SideEye* sideEye = new SideEye(Pin::LEFT_IR, Pin::RIGHT_IR);
//ServoIR* servo_ir = new ServoIR(Pin::SERVO, Pin::FRONT_IR);
#endif
