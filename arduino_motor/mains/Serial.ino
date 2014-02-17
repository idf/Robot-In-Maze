#include "Serial.h"

SerialCommnder::SerialCommnder() {
  this->command = 99;
  this->is_started = false;
}

bool SerialCommnder::receive_exec_command() {
  if(Serial.available()>0) {
    int function_code;
    double parameter;
    /*
    String command_string =  Serial.readStringUntil('\n');

    if(command_string.length()!=7)
      return false;

   
    function_code = 10*(command_string[0]-'0');
    function_code += (command_string[1]-'0');

    
    parameter = 100*(command_string[2]-'0');
    parameter += 10*(command_string[3]-'0');
    parameter += (command_string[4]-'0');
    parameter += 1/10.0*(command_string[5]-'0');
    parameter += 1/100.0*(command_string[6]-'0');
  */
    long comamnd_int = Serial.parseInt();
    function_code = comamnd_int/100000;
    parameter = comamnd_int%100000;
    parameter /= 100;
    // Serial.flush();
    // Serial.print("debug, command received: "); Serial.print(function_code); Serial.println(parameter);
    return this->exec_command(function_code, parameter);
  }
  return false;
}

int SerialCommnder::get_command() {
  return this->command;
}

bool SerialCommnder::exec_command(int function_code, double parameter) {
  if(function_code==0) {
    moveForward(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==1) {
    turnRight(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==2) {
    turnLeft(parameter);
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
