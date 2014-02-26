#ifndef SerialSenderReceiver_H
#define SerialSenderReceiver_H
// #include <aJSON.h> // https://github.com/interactive-matter/aJson
class SerialCommnder 
{
public:
  SerialCommnder();
  bool receive_exec_command();
  void send_command_complete(int function_code, int status_code);
  void send_sensor_readings(int front_value, int left_value, int right_value);
  void send_ready_signal();
  int get_command();

private:
  int command;
  bool is_started;
  bool exec_command(int function_code, double parameter);
};
#endif

