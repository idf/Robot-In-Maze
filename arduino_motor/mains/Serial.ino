#include "Serial.h"
#include <aJSON.h>

SerialCommnder::SerialCommnder() {

}

bool SerialCommnder::receive_exec_command() {
  if(Serial.available()>0) {
    char* json_string = (char*) Serial.readStringUntil('\n').c_str();
    aJsonObject* root = aJson.parse(json_string);
    aJsonObject* function = aJson.getObjectItem(root, "function");
    aJsonObject* parameter = aJson.getObjectItem(root, "parameter");

    this->command = function->valueint; 
    return this->exec_command(function->valueint, parameter->valuefloat);
  }
  return false;
}

int SerialCommnder::get_command() {
  return this->command;
}

bool SerialCommnder::exec_command(int function_code, double parameter) {
  if(function_code==0) {
    //moveForward(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==1) {
    //turnRight(parameter);
    this->send_command_complete(function_code, 200);
    return true;
  }
  else if(function_code==2) {
    //turnLeft(parameter);
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
  Serial.print(F("{\"function\":")); Serial.print(function_code);
  Serial.print(F(",\"status\":")); Serial.print(status_code); Serial.println(F("}"));

}

