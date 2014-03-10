#include "Serial.h"
#include "globals.h"
// public
SerialCommnder::SerialCommnder() {
  this->command = 99;
  this->is_started = false;
}

bool SerialCommnder::receive_exec_command() {
  if(Serial.available()>0) {
    int function_code;
    double parameter;

    long comamnd_int = Serial.parseInt();
    function_code = comamnd_int/100000;
    parameter = comamnd_int%100000;
    parameter /= 100;
    // Serial.flush();
    Serial.print(F("debug, command received: ")); Serial.print(function_code); Serial.println(parameter);
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
                                          int front_value, 
                                          int front_left_value, 
                                          int front_right_value, 
                                          int left_value, 
                                          int right_value,
                                          int side_ultra_value) {
  Serial.print(F("{\"sensors\":["));
  // front 
  Serial.print(F("{\"sensor\":")); Serial.print(0);
  Serial.print(F(",\"value\":")); Serial.print(front_value); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(1);
  Serial.print(F(",\"value\":")); Serial.print(front_left_value); Serial.print(F("}")); Serial.print(F(","));

  Serial.print(F("{\"sensor\":")); Serial.print(2);
  Serial.print(F(",\"value\":")); Serial.print(front_right_value); Serial.print(F("}")); Serial.print(F(","));
  // left
  Serial.print(F("{\"sensor\":")); Serial.print(10); 
  Serial.print(F(",\"value\":")); Serial.print(left_value); Serial.print(F("}")); Serial.print(F(","));
  // right
  Serial.print(F("{\"sensor\":")); Serial.print(11);
  Serial.print(F(",\"value\":")); Serial.print(right_value); Serial.print(F("}")); Serial.print(F(","));
  // right ultra
  Serial.print(F("{\"sensor\":")); Serial.print(12);
  Serial.print(F(",\"value\":")); Serial.print(side_ultra_value); Serial.print(F("}"));
  //end
  Serial.println(F("]}"));
   
}

// private
bool SerialCommnder::exec_command(int function_code, double parameter) {
  if(function_code==0) {
    moveForward(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==1) {
    turnLeft(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==2) {
    turnRight(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }


  else if(function_code==10) { // TODO sensor request
    getSensorReadings();
    this->send_command_complete(function_code, 200);
    return true;
  }


  else if(function_code==20) {
    moveForward(parameter);
    getSensorReadings();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==21) {
    turnLeft(parameter);
    getSensorReadings();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==22) {
    turnRight(parameter);
    getSensorReadings();
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==98) {
    calibrator->calibrate(parameter);
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


