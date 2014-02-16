#ifndef SerialSender_H
#define SerialSender_H
#include "aJSON.h" // https://github.com/interactive-matter/aJson
class SerialSender 
{
public:
  SerialSender();
  void send_action_complete(char* function_name);
private:
  void reset_json();
  aJsonObject* info;
  aJsonObject* root; // = aJson.createArray();

};
///////////////////////////////////////////////////////////////////////////////////////////////////
class SerialReceiver 
{
public:

private:
};
#endif

