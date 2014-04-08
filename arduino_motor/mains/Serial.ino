#include "Serial.h"
#include "globals.h"
#define SCALE 2
#define EXPLORE_SCALE 1.0
// public
SerialCommnder::SerialCommnder() {
  this->command = 99;
  this->is_started = false;
}

bool SerialCommnder::receive_exec_command() {
  Config::verbose = false;
  
  if(Serial.available()>0) {
    int function_code;
    long parameter;

    long comamnd_int = Serial.parseInt();
    function_code = comamnd_int/100000;
    parameter = comamnd_int%100000;
    parameter /= 100;
    // Serial.flush();
    // Serial.print(F("debug, command received: ")); Serial.print(function_code); Serial.println(parameter);
    return this->exec_command(function_code, parameter);
  }
  return false;
}

int SerialCommnder::get_command() {
  return this->command;
}

void SerialCommnder::send_command_complete(int function_code, int status_code) {
  /*
  aJsonObject* root = aJson.createObject();  
  (avoid huge memory consumption)
  aJson.addItemToObject(root, "function", aJson.createItem(function_code));
  aJson.addItemToObject(root, "status", aJson.createItem(status_code));

  aJson.addNumberToObject(root, "function", function_code);
  aJson.addNumberToObject(root, "status", status_code);

  char* output = aJson.print(root);
  delete output;
  delete root;
  */
  // higher performance
  Serial.print(F("{\"function\":")); Serial.print(function_code);
  Serial.print(F(",\"status\":")); Serial.print(status_code); Serial.println(F("}"));

}

void SerialCommnder::send_ready_signal() {
  if(this->is_started==false) {
    this->send_command_complete(99, 200);
    this->is_started = true;
  }
}

void SerialCommnder::send_sensor_readings(
                                          int value0, 
                                          int value1, 
                                          int value2, 
                                          int value3, 
                                          int value4,
                                          int value10,
                                          int value11) {
  Serial.print(F("{\"sensors\":["));

  Serial.print(F("{\"sensor\":")); Serial.print(0);
  Serial.print(F(",\"value\":")); Serial.print(value0); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(1);
  Serial.print(F(",\"value\":")); Serial.print(value1); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(2);
  Serial.print(F(",\"value\":")); Serial.print(value2); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(3); 
  Serial.print(F(",\"value\":")); Serial.print(value3); Serial.print(F("}")); Serial.print(F(","));

   Serial.print(F("{\"sensor\":")); Serial.print(4); 
  Serial.print(F(",\"value\":")); Serial.print(value4); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(10);
  Serial.print(F(",\"value\":")); Serial.print(value10); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(11);
  Serial.print(F(",\"value\":")); Serial.print(value11); Serial.print(F("}"));
  //end
  Serial.println(F("]}"));
   
}

// private
bool SerialCommnder::exec_command(int function_code, double parameter) {
  if(function_code==0) {
    pidMgr->setScale(SCALE);
    moveForward(parameter); delay(100);
    this->send_command_complete(function_code, 200);
    pidMgr->restore();
    return true;
  }
  else if(function_code==1) {
    turnLeft(parameter); delay(300);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==2) {
    turnRight(parameter); delay(300); // turning is not accurate in shortest path 
    this->send_command_complete(function_code, 200);
    return true;
  }


  else if(function_code==10) { // TODO sensor request
    getSensorReadings();
    this->send_command_complete(function_code, 200);
    return true;
  }


  else if(function_code==20) {
    pidMgr->setScale(EXPLORE_SCALE);
    if (eyes->is_safe_forward(parameter))  
      moveForward(parameter);
    getSensorReadings();
    calibrator->try_calibrate();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==21) {
    turnLeft(parameter);
    getSensorReadings();
    calibrator->try_calibrate();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==22) {
    turnRight(parameter);
    getSensorReadings();
    calibrator->try_calibrate();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==98) {
    calibrator->calibrate(parameter);
    errorCumulator->reset();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else {
    this->send_command_complete(function_code, 405); // Method not Allowed
    return false;
  }
  // TODO
  this->send_command_complete(function_code, 408); // Request Timeout
  return false;
}


//private
void SerialCommnder::routine_clean(int function_code) {

}
