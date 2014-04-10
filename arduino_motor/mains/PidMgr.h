#ifndef PIDMGR_H
#define PIDMGR_H
#include <PID_v1.h>  
class PidMgr {
public:
  PidMgr();
  void setScale(double scale);
  void init();
  void restore();
  double getCurrentScale();

  double SetpointLeft, InputLeft, OutputLeft;
  double SetpointRight, InputRight, OutputRight;
  double SetpointMid, InputMid, OutputMid;

  PID* leftPID; 
  PID* rightPID; 
  PID* midPID; 


private:
  double current_scale;

  static const double kp=0.5, ki=0.25, kd=0;
  static const double kp_mid=0.5, ki_mid=0.05, kd_mid=0.25;
};
#endif