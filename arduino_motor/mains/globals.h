#ifndef GLOBALS_H
#define GLOBALS_H
#include "Pin.h"
#include "Serial.h"
#include "ErrorCumulator.h"
#include "FrontEye.h"
#include "SideEye.h"
#include "Calibrator.h"
#include "PidMgr.h"
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

// TODO
//PID_ATune aTuneLeft(&InputLeft, &OutputLeft);
//PID_ATune aTuneRight(&InputRight, &OutputRight);


DualVNH5019MotorShield motorShield(Pin::INA1, Pin::INB1, Pin::EN1DIAG1, Pin::CS1,  // EN1DIAG1 is not used 
                                  Pin::INA2, Pin::INB2, Pin::EN2DIAG2, Pin::CS2);   // EN2DIAG2 is not used

SerialCommnder* serialCommnder = new SerialCommnder(); 
ErrorCumulator* errorCumulator = new ErrorCumulator();
FrontEye* frontEye = new FrontEye(Pin::ULTRA_1_PWM , Pin::ULTRA_1_TRIG, Pin::FRONT_IR_LEFT, Pin::FRONT_IR_RIGHT);
SideEye* sideEye = new SideEye(Pin::LEFT_IR, Pin::RIGHT_IR, Pin::ULTRA_2_PWM , Pin::ULTRA_2_TRIG);
Calibrator* calibrator = new Calibrator(frontEye);
PidMgr* pidMgr = new PidMgr();
//ServoIR* servo_ir = new ServoIR(Pin::SERVO, Pin::FRONT_IR);
#endif
