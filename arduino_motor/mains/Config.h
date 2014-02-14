#ifndef CONFIG_H
#define CONFIG_H
class Config
{
public:
  //static double const _PI = 3.14159265359;
  static int const MAX_SPEED_ANALOG = 255;
  static int const DESIGNED_MAX_SPEED = 400;
  static int const WHEEL_DIAMETER = 6;
  static int const COUNTS_PER_REVOLUTION = 2249/2;
  static double const DISTANCE_PER_TICK_CM;
  static int const SAMPLE_TIME = 10;

  static double const WHEELS_INTERVAL;
  static double MAX_SPEED;
  static double TARGET_SPEED;
  static double MIN_SPEED;
  static double PID_UPPER_LIMIT;
  static double PID_LOWER_LIMIT;
  static double PID_SETPOINT;
};
#endif