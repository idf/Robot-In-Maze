#include "PidMgr.h"
#include <PID_v1.h>  

PidMgr::PidMgr() {
  // PID
  // variables upper case by documentation 
  this->leftPID = new PID(&InputLeft, &OutputLeft, &SetpointLeft, kp, ki, kd, DIRECT);
  this->rightPID = new PID(&InputRight, &OutputRight, &SetpointRight, kp, ki, kd, DIRECT);
  this->midPID = new PID(&InputMid, &OutputMid, &SetpointMid, kp_mid, ki_mid, kd_mid, DIRECT);

  this->current_scale = 1.0;
}
void PidMgr::init() {
  this->leftPID->SetMode(AUTOMATIC);
  this->rightPID->SetMode(AUTOMATIC);
  this->midPID->SetMode(AUTOMATIC);

  this->leftPID->SetSampleTime(Config::SAMPLE_TIME); 
  this->rightPID->SetSampleTime(Config::SAMPLE_TIME); 
  this->midPID->SetSampleTime(Config::SAMPLE_TIME);

  this->SetpointLeft = Config::PID_SETPOINT;
  this->SetpointRight = Config::PID_SETPOINT;
  this->SetpointMid = 0;

  // for controlling the speed 
  
  leftPID->SetOutputLimits(Config::PID_LOWER_LIMIT/2, Config::PID_UPPER_LIMIT/2);
  rightPID->SetOutputLimits(Config::PID_LOWER_LIMIT/2, Config::PID_UPPER_LIMIT/2);
  midPID->SetOutputLimits(-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2);
}

void PidMgr::setScale(double scale) {
  this->SetpointLeft *= scale;
  this->SetpointRight *= scale;
  this->SetpointMid *= scale; // always 0

  Config::PID_SETPOINT *= scale;
  Config::PID_UPPER_LIMIT *= scale;
  Config::PID_LOWER_LIMIT *= scale;

  Config::MAX_SPEED *= scale;
  Config::TARGET_SPEED *= scale;
  Config::MIN_SPEED *= scale;

  this->leftPID->SetOutputLimits(Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT);
  this->rightPID->SetOutputLimits(Config::PID_LOWER_LIMIT, Config::PID_UPPER_LIMIT);
  this->midPID->SetOutputLimits(-Config::PID_SETPOINT/2, Config::PID_SETPOINT/2);

  this->current_scale *= scale;
}

void PidMgr::restore() {
  setScale(1.0/this->current_scale);
}

double PidMgr::getCurrentScale() {
  return this->current_scale;
}
