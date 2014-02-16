#include "Serial.h"
#include "aJSON.h"
SerialSender::SerialSender() {
  this->root=aJson.createArray();  
}
void SerialSender::send_action_complete(char* function_name) {
  this->reset_json();
  aJson.addItemToObject(this->root, "function", aJson.createItem(function_name));
  aJson.addItemToObject(this->root, "status", aJson.createItem(200));

  char* output = aJson.print(root);
  Serial.println(output);
}

void SerialSender::reset_json() {

}

////////////////////////////Serial Receiver ///////////////////////////////////////////////////////