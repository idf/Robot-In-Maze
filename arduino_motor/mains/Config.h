#ifndef CONFIG_H
#define CONFIG_H
class Config
{
public:
  //static double const _PI = 3.14159265359;
  static int const MAX_SPEED_ANALOG;
  static int const DESIGNED_MAX_SPEED;
  static int const WHEEL_DIAMETER;
  static int const COUNTS_PER_REVOLUTION;
  static double const DISTANCE_PER_TICK_CM;
  static int const SAMPLE_TIME;

  static double const WHEELS_INTERVAL;
  static double const WHEELS_INTERVAL_LEFT;
  static double MAX_SPEED;
  static double TARGET_SPEED;
  static double MIN_SPEED;
  static double PID_UPPER_LIMIT;
  static double PID_LOWER_LIMIT;
  static double PID_SETPOINT;

  static double const TICKS_PER_DEGREE;
  static double const TICKS_PER_DEGREE_LEFT;
};
#endif
