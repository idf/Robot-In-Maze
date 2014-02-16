#ifndef SerialSenderReceiver_H
#define SerialSenderReceiver_H
#include <aJSON.h> // https://github.com/interactive-matter/aJson
class SerialCommnder 
{
public:
  SerialCommnder();
  bool receive_exec_command();
  void send_command_complete(int function_code, int status_code);

  int get_command();

private:
  int command;
  bool exec_command(int function_code, double parameter);
};
#endif

